#include "CoheteKeyframes.h"
#include "Geometria.h"
#include <gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace {
    struct Pieza { glm::vec3 posicion; float giro; };
    struct Cuadro { float tiempo; Pieza pieza[3]; float fuego[2]; };
    std::vector<Cuadro> cuadros;
    glm::vec3 origen;
    Texture *metal, *lava;
    MeshModel* tobera;
    float inicio=0;
    bool reproduciendo=false;
    int ultimo=-1, vueltaAnterior=-1;
    const char* archivo="Cohete_keyframes.txt";

    float Tiempo(float t) { return reproduciendo ? glm::max(0.0f,t-inicio) : 0.0f; }
    float Suave(float u) { return u*u*(3-2*u); }
    Cuadro Evaluar(float t) {
        if(t<=cuadros.front().tiempo)return cuadros.front();
        size_t i=1;
        while(i<cuadros.size() && cuadros[i].tiempo<t)++i;
        i=std::min(i,cuadros.size()-1);
        const auto& a=cuadros[i-1];const auto& b=cuadros[i];
        float u=Suave(glm::clamp((t-a.tiempo)/(b.tiempo-a.tiempo),0.0f,1.0f));
        Cuadro q=b;
        for(int j=0;j<3;++j) {
            q.pieza[j].posicion=glm::mix(a.pieza[j].posicion,b.pieza[j].posicion,u);
            q.pieza[j].giro=glm::mix(a.pieza[j].giro,b.pieza[j].giro,u);
        }
        for(int j=0;j<2;++j)q.fuego[j]=glm::mix(a.fuego[j],b.fuego[j],u);
        // Los ultimos cinco cuadros forman una orbita cerrada. Solo la capsula repite.
        size_t k=cuadros.size()-5;
        if(t>=cuadros[k].tiempo) {
            float tramo=cuadros[k+1].tiempo-cuadros[k].tiempo;
            float fase=std::fmod(t-cuadros[k].tiempo,4*tramo)/tramo;
            int n=int(fase);float v=fase-n;
            auto p0=cuadros[k+(n+3)%4].pieza[2].posicion;
            auto p1=cuadros[k+n].pieza[2].posicion;
            auto p2=cuadros[k+(n+1)%4].pieza[2].posicion;
            auto p3=cuadros[k+(n+2)%4].pieza[2].posicion;
            q.pieza[2].posicion=.5f*((2.0f*p1)+(-p0+p2)*v+(2.0f*p0-5.0f*p1+4.0f*p2-p3)*v*v+(-p0+3.0f*p1-3.0f*p2+p3)*v*v*v);
            q.pieza[2].giro=glm::mix(cuadros[k+n].pieza[2].giro,cuadros[k+(n+1)%4].pieza[2].giro,Suave(v));
        }
        return q;
    }
    glm::mat4 Base(const Pieza& p) {
        auto m=glm::translate(glm::mat4(1),origen+p.posicion);
        return glm::rotate(m,glm::radians(p.giro),{0,0,1});
    }
    void Cilindro(const glm::mat4& base,float y,float radio,float alto,glm::vec3 color) {
        Dibujar(mCilindro,glm::scale(glm::translate(base,{0,y,0}),{radio,alto,radio}),color);
    }
    void Fuego(const glm::mat4& base,float potencia,float t,float altura) {
        if(potencia<.001f)return;
        float largo=glm::min((5+.4f*std::sin(31*t))*potencia,glm::max(0.0f,altura-.05f));
        if(largo<.001f)return;
        lava->UseTexture();
        auto m=glm::rotate(base,glm::radians(180.0f),{1,0,0});
        Dibujar(mCono,glm::scale(m,{.85f*potencia,largo,.85f*potencia}),{1,.4f,.05f},1.8f);
        metal->UseTexture();
    }
}

bool RecargarCoheteKeyframes() {
    std::ifstream entrada(archivo);std::string linea;std::vector<Cuadro> nuevos;
    int numero=0;
    while(std::getline(entrada,linea)) {
        ++numero;std::istringstream fila(linea);
        fila>>std::ws;if(fila.eof()||fila.peek()=='#')continue;
        Cuadro q{};bool bien=bool(fila>>q.tiempo);
        for(auto& p:q.pieza)bien=bool(fila>>p.posicion.x>>p.posicion.y>>p.posicion.z>>p.giro)&&bien;
        bien=bool(fila>>q.fuego[0]>>q.fuego[1])&&bien;
        bool finitos=std::isfinite(q.tiempo);
        for(auto& p:q.pieza)finitos=finitos&&std::isfinite(p.posicion.x)&&std::isfinite(p.posicion.y)&&std::isfinite(p.posicion.z)&&std::isfinite(p.giro);
        for(float f:q.fuego)finitos=finitos&&std::isfinite(f)&&f>=0&&f<=1;
        if(!bien||!finitos||q.tiempo<0||(!nuevos.empty()&&q.tiempo<=nuevos.back().tiempo)) {
            printf("KEYFRAMES ERROR linea %d; se conserva la animacion anterior.\n",numero);return false;
        }
        nuevos.push_back(q);
    }
    if(nuevos.size()<7||nuevos.front().tiempo!=0) {printf("KEYFRAMES ERROR: revisar %s (al menos 7 cuadros, inicio 0).\n",archivo);return false;}
    size_t k=nuevos.size()-5;float paso=nuevos[k+1].tiempo-nuevos[k].tiempo;
    for(size_t i=k+1;i<nuevos.size();++i)if(std::abs(nuevos[i].tiempo-nuevos[i-1].tiempo-paso)>.001f) {
        printf("KEYFRAMES ERROR: la orbita requiere intervalos iguales.\n");return false;
    }
    if(glm::length(nuevos.back().pieza[2].posicion-nuevos[k].pieza[2].posicion)>.001f) {
        printf("KEYFRAMES ERROR: la capsula debe cerrar su orbita.\n");return false;
    }
    cuadros.swap(nuevos);ultimo=-1;vueltaAnterior=-1;
    printf("KEYFRAMES CARGADOS: %zu desde %s\n",cuadros.size(),archivo);
    for(size_t i=0;i<cuadros.size();++i) {
        const auto& q=cuadros[i];printf("KF %02zu t=%5.2f | propulsor Y=%6.2f | etapa Y=%6.2f | capsula (%6.2f,%6.2f,%6.2f)\n",i,q.tiempo,q.pieza[0].posicion.y,q.pieza[1].posicion.y,q.pieza[2].posicion.x,q.pieza[2].posicion.y,q.pieza[2].posicion.z);
    }
    return true;
}

void CargarCoheteKeyframes(glm::vec3 plataforma) {
    origen=plataforma;metal=CargarTextura("Textures/metal.png");lava=CargarTextura("Textures/lava.png");
    tobera=CrearTroncoCono(.48f,24);
    if(!RecargarCoheteKeyframes())printf("Cohete: falta un TXT valido; no se dibuja hasta recargar con F8.\n");
}
void IniciarCoheteKeyframes(float t) {
    inicio=t;reproduciendo=true;ultimo=-1;vueltaAnterior=-1;
    printf("KEYFRAMES REPRODUCIR desde 0.00 s\n");
}
void ActualizarCoheteKeyframes(float t) {
    if(cuadros.empty()||!reproduciendo)return;
    float local=Tiempo(t);
    while(ultimo+1<int(cuadros.size())&&local>=cuadros[ultimo+1].tiempo) {
        ++ultimo;printf("KEYFRAME REPRODUCIDO %02d / t=%.2f s\n",ultimo,cuadros[ultimo].tiempo);
    }
    size_t k=cuadros.size()-5;
    if(local>=cuadros[k].tiempo) {
        int vuelta=int((local-cuadros[k].tiempo)/(cuadros.back().tiempo-cuadros[k].tiempo));
        if(vuelta!=vueltaAnterior){vueltaAnterior=vuelta;printf("CAPSULA EN ORBITA: vuelta %d, etapas en el suelo\n",vuelta+1);}
    }
}
void DibujarCoheteKeyframes(float t) {
    if(cuadros.empty())return;
    auto q=Evaluar(Tiempo(t));metal->UseTexture();UsarMaterial(matMetal);
    auto a=Base(q.pieza[0]);
    Dibujar(tobera,glm::scale(a,{1.15f,1.2f,1.15f}),{.28f,.3f,.34f});
    Cilindro(a,3.6f,1.5f,4.8f,{.9f,.9f,.93f});
    Cilindro(a,1.3f,1.52f,.4f,{.8f,.18f,.12f});
    for(int i=0;i<4;++i) {
        auto m=glm::rotate(a,glm::radians(90.0f*i),{0,1,0});
        m=glm::translate(m,{1.65f,1.8f,0});
        Dibujar(mCubo,glm::scale(m,{1.3f,2.5f,.22f}),{.8f,.18f,.12f});
    }
    Fuego(a,q.fuego[0],Tiempo(t),q.pieza[0].posicion.y-1);
    auto b=Base(q.pieza[1]);
    Cilindro(b,2,1.5f,4,{.8f,.85f,.9f});
    Cilindro(b,.2f,1.52f,.4f,{.2f,.3f,.45f});
    Fuego(b,q.fuego[1],Tiempo(t),q.pieza[1].posicion.y);
    auto c=Base(q.pieza[2]);
    Cilindro(c,.4f,1.5f,.8f,{.5f,.55f,.6f});
    Dibujar(mCono,glm::scale(glm::translate(c,{0,.8f,0}),{1.5f,3,1.5f}),{.9f,.2f,.2f});
    Dibujar(mEsfera,glm::scale(glm::translate(c,{0,1.1f,1.1f}),{.45f,.45f,.18f}),{.15f,.6f,.85f},.2f);
}
void CamaraCoheteKeyframes(float t,glm::vec3& ojo,glm::vec3& objetivo) {
    float u=Suave(glm::clamp(Tiempo(t)/12,0.0f,1.0f));
    ojo=origen+glm::mix(glm::vec3(30,19,43),glm::vec3(60,42,85),u);
    objetivo=origen+glm::mix(glm::vec3(0,8,0),glm::vec3(-3,34,0),u);
    if(!cuadros.empty()) {
        float acercamiento=Suave(glm::clamp((Tiempo(t)-20)/4,0.0f,1.0f));
        auto capsula=origen+Evaluar(Tiempo(t)).pieza[2].posicion;
        ojo=glm::mix(ojo,capsula+glm::vec3(18,12,24),acercamiento);
        objetivo=glm::mix(objetivo,capsula+glm::vec3(0,1,0),acercamiento);
    }
}

