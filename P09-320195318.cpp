/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
bool avanza;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float movCoche = 0.0f;
float movOffset = 0.01f;

Window mainWindow;
std::vector<MeshModel*> meshList;
std::vector<Shader> shaderList;
// circuito para la nave
MeshModel circuitoMallas[3];
std::vector<glm::vec3> puntosCircuito; // centro del recorrido

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;



Model Rover_M;
// satelite
Model CuerpoSatelite_M;
Model AlaDer1_M;
Model AlaDer2_M;
Model AlaIzq1_M;
Model AlaIzq2_M;


//naev
Model Hippocrates_M;
Texture hippocratesEmision, plasmaAzulTexture, naveDifusaP9;
Shader shaderNaveP9;

// agregado del modelo del avion
Model P9AvionCuerpo, P9AvionCara, P9AvionAlas;

//agregado de los modelos del rover
Model P9Separado_Rover_M;                 // cuerpo principal
Model P9Separado_BaseBrazo_M;             // base pegada al cuerpo
Model P9Separado_BrazoSup1_M;
Model P9Separado_BrazoSup2_M;
Model P9Separado_BrazoSup3_M;

Model P9Separado_PataDer1_M;
Model P9Separado_PataDer2_M;
Model P9Separado_PataIzq1_M;
Model P9Separado_PataIzq2_M;

Model P9Separado_RuedaDer1_M;
Model P9Separado_RuedaDer2_M;
Model P9Separado_RuedaDer3_M;
Model P9Separado_RuedaIzq1_M;
Model P9Separado_RuedaIzq2_M;
Model P9Separado_RuedaIzq3_M;

//fin agregado




Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	MeshModel* obj5 = new MeshModel();
	obj5->CreateMeshModel(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	MeshModel* obj6 = new MeshModel();
	obj6->CreateMeshModel(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	MeshModel* obj7 = new MeshModel();
	obj7->CreateMeshModel(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número


    // circuito, dos rectas de 80 unidades y dos curvas de radio 24
    // centro en Z=-95; ancho 10 y el piso original permanece en Y=-2
    const int muestrasPorTramo = 64;
    const float piCircuito = 180.0f * toRadians;
    puntosCircuito.clear();
    for (int tramo=0; tramo<4; ++tramo)
    {
        for (int i=0; i<muestrasPorTramo; ++i)
        {
            const float u = static_cast<float>(i) / muestrasPorTramo;
            glm::vec3 centro(0.0f, -1.85f, 0.0f);
            if (tramo == 0)
            {
                centro.x = -40.0f + 80.0f * u;
                centro.z = -119.0f;
                // Subida y bajada unidas
                const float elevacion = std::sin(piCircuito * u);
                centro.y += 8.0f * elevacion * elevacion;
            }
            else if (tramo == 1)
            {
                const float angulo = -0.5f * piCircuito + piCircuito * u;
                centro.x = 40.0f + 24.0f * std::cos(angulo);
                centro.z = -95.0f + 24.0f * std::sin(angulo);
            }
            else if (tramo == 2)
            {
                centro.x = 40.0f - 80.0f * u;
                centro.z = -71.0f;
            }
            else
            {
                const float angulo = 0.5f * piCircuito + piCircuito * u;
                centro.x = -40.0f + 24.0f * std::cos(angulo);
                centro.z = -95.0f + 24.0f * std::sin(angulo);
            }
            puntosCircuito.push_back(centro);
        }
    }
    const int totalCircuito = static_cast<int>(puntosCircuito.size());
    std::vector<glm::vec3> lateralesCircuito;
    for (int i=0; i<totalCircuito; ++i)
    {
        const glm::vec3 tangente = puntosCircuito[(i+1)%totalCircuito]
            - puntosCircuito[(i+totalCircuito-1)%totalCircuito];
        lateralesCircuito.push_back(glm::normalize(glm::vec3(tangente.z,0.0f,-tangente.x)));
    }
    std::vector<GLfloat> verticesCircuito[3];
    std::vector<unsigned int> indicesCircuito[3];
    for (int i=0; i<totalCircuito; ++i)
    {
        const int j=(i+1)%totalCircuito;
        // Banda 0, asfalto, 1 y 2: bordes, 3 y 4: paredes hasta el piso
        // Banda 5, una marca de salida 
        for (int banda=0; banda<6; ++banda)
        {
            if (banda == 5 && i != 2*muestrasPorTramo+12) continue;
            const int grupo = banda == 0 ? 0 : ((banda == 3 || banda == 4) ? 2 : 1);
            glm::vec3 esquina[4];
            if (banda == 3 || banda == 4)
            {
                const float lado = banda == 3 ? -5.0f : 5.0f;
                esquina[0] = puntosCircuito[i] + lateralesCircuito[i]*lado;
                esquina[1] = puntosCircuito[j] + lateralesCircuito[j]*lado;
                esquina[2] = esquina[1]; esquina[2].y = -2.0f;
                esquina[3] = esquina[0]; esquina[3].y = -2.0f;
                if (banda == 3)
                {
                    const glm::vec3 temporal = esquina[0]; esquina[0]=esquina[1]; esquina[1]=temporal;
                    const glm::vec3 temporal2 = esquina[2]; esquina[2]=esquina[3]; esquina[3]=temporal2;
                }
            }
            else
            {
                const float izquierda = banda == 2 ? 4.75f : -5.0f;
                const float derecha = banda == 1 ? -4.75f : 5.0f;
                const glm::vec3 elevacionMarca(0.0f, banda == 0 ? 0.0f : 0.02f, 0.0f);
                esquina[0] = puntosCircuito[i] + lateralesCircuito[i]*izquierda + elevacionMarca;
                esquina[1] = puntosCircuito[j] + lateralesCircuito[j]*izquierda + elevacionMarca;
                esquina[2] = puntosCircuito[j] + lateralesCircuito[j]*derecha + elevacionMarca;
                esquina[3] = puntosCircuito[i] + lateralesCircuito[i]*derecha + elevacionMarca;
            }
            const glm::vec3 normal = glm::normalize(glm::cross(esquina[1]-esquina[0], esquina[2]-esquina[0]));
            const unsigned int base = static_cast<unsigned int>(verticesCircuito[grupo].size()/8);
            for (int k=0; k<4; ++k)
            {
                verticesCircuito[grupo].insert(verticesCircuito[grupo].end(), {
                    esquina[k].x, esquina[k].y, esquina[k].z,
                    (k==1 || k==2) ? 1.0f : 0.0f, k>=2 ? 1.0f : 0.0f,
                    normal.x, normal.y, normal.z });
            }
            indicesCircuito[grupo].insert(indicesCircuito[grupo].end(), {
                base,base+1,base+2, base,base+2,base+3 });
        }
    }
    for (int grupo=0; grupo<3; ++grupo)
        circuitoMallas[grupo].CreateMeshModel(verticesCircuito[grupo].data(), indicesCircuito[grupo].data(),
            static_cast<unsigned int>(verticesCircuito[grupo].size()),
            static_cast<unsigned int>(indicesCircuito[grupo].size()));
    // FIN CONSTRUCCION CIRCUITO.
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverModificado.obj");
    //texturas del avion
    {
        GLint alineacionAvion = 4;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &alineacionAvion);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        P9AvionCuerpo.LoadModel("Models/avion_p7_cuerpo.obj");
        P9AvionCara.LoadModel("Models/avion_p7_cara.obj");
        P9AvionAlas.LoadModel("Models/avion_p7_alas.obj");
        glPixelStorei(GL_UNPACK_ALIGNMENT, alineacionAvion);
    }
    // fin texturas avion
	//agrega las 15 peizas fbx
	P9Separado_Rover_M = Model();
	P9Separado_Rover_M.LoadModel("Models/rovermodificado.fbx");

	P9Separado_BaseBrazo_M = Model();
	P9Separado_BaseBrazo_M.LoadModel("Models/basepegadacuerpo.fbx");

	P9Separado_BrazoSup1_M = Model();
	P9Separado_BrazoSup1_M.LoadModel("Models/brazosup1.fbx");

	P9Separado_BrazoSup2_M = Model();
	P9Separado_BrazoSup2_M.LoadModel("Models/brazosup2.fbx");

	P9Separado_BrazoSup3_M = Model();
	P9Separado_BrazoSup3_M.LoadModel("Models/brazosup3.fbx");

	P9Separado_PataDer1_M = Model();
	P9Separado_PataDer1_M.LoadModel("Models/patader1.fbx");

	P9Separado_PataDer2_M = Model();
	P9Separado_PataDer2_M.LoadModel("Models/patader2.fbx");

	P9Separado_PataIzq1_M = Model();
	P9Separado_PataIzq1_M.LoadModel("Models/pataizq1.fbx");

	P9Separado_PataIzq2_M = Model();
	P9Separado_PataIzq2_M.LoadModel("Models/pataizq2.fbx");

	P9Separado_RuedaDer1_M = Model();
	P9Separado_RuedaDer1_M.LoadModel("Models/ruedader1.fbx");

	P9Separado_RuedaDer2_M = Model();
	P9Separado_RuedaDer2_M.LoadModel("Models/ruedader2.fbx");

	P9Separado_RuedaDer3_M = Model();
	P9Separado_RuedaDer3_M.LoadModel("Models/ruedader3.fbx");

	P9Separado_RuedaIzq1_M = Model();
	P9Separado_RuedaIzq1_M.LoadModel("Models/ruedaizq1.fbx");

	P9Separado_RuedaIzq2_M = Model();
	P9Separado_RuedaIzq2_M.LoadModel("Models/ruedaizq2.fbx");

	P9Separado_RuedaIzq3_M = Model();
	P9Separado_RuedaIzq3_M.LoadModel("Models/ruedaizq3.fbx");

	//fin

	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.3f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;


	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

    // Animacion del rover
    // Piso, vertices X +/-10 multiplicados por la escala 30 = +/-300
    const GLfloat limiteRoverIzq = -300.0f + 7.0f;
    const GLfloat limiteRoverDer =  300.0f - 7.0f;
    const GLfloat velocidadRover = 25.0f; // unidades por segundo
    const GLfloat radioRuedaRover = 1.4f * 0.85f; // radio aproximado a escala de escena
    GLfloat posicionRoverX = 12.0f;
    GLfloat giroLlantasRover = 0.0f;
    avanza = true; // true: aumenta X; false: retrocede sin girar el cuerpo
    // Ejercicio 2, vuelo del avion ovalado
    // Animacion compleja: traslacion y rotacion calculadas con ecuaciones.
    const GLfloat radioAvionX = 40.0f;
    const GLfloat radioAvionZ = 28.0f;
    const GLfloat velocidadAvion = 10.0f; 
    const GLfloat alturaAvion = 18.0f;
    GLfloat faseAvion = 0.0f; // radianes
    shaderNaveP9.CreateFromFiles("shaders/nave_p9.vert", "shaders/nave_p9.frag");
    glm::mat4 matrizHippocrates = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f,6.0f,-22.0f));
    matrizHippocrates = glm::scale(matrizHippocrates, glm::vec3(0.65f));
    Hippocrates_M.LoadModel("Models/hippocrates.obj");
    naveDifusaP9 = Texture("Textures/hippocrates_diffuse.png");
    naveDifusaP9.LoadTextureA();
    hippocratesEmision = Texture("Textures/hippocrates_emission.png");
    hippocratesEmision.LoadTextureA();
    hippocratesEmision.UseTexture();
    GLint texturaEmisionHippocrates = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &texturaEmisionHippocrates);
    glm::vec3 motorPuntualHippocrates = glm::vec3(matrizHippocrates * glm::vec4(1.8f,0.0f,-5.05f,1.0f));
    glm::vec3 motorSpotHippocrates = glm::vec3(matrizHippocrates * glm::vec4(-1.8f,0.0f,-5.05f,1.0f));
    glm::vec3 direccionMotorHippocrates = glm::normalize(glm::mat3(matrizHippocrates) * glm::vec3(0.0f,0.0f,-1.0f));

    // textura rgba de plasma para ambos motores
    plasmaAzulTexture = Texture("Textures/plasma_azul.png");
    plasmaAzulTexture.LoadTextureA();
    plasmaAzulTexture.UseTexture();
    GLint texturaPlasmaAzul = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &texturaPlasmaAzul);
    MeshModel mallaPlasma;
    GLfloat verticesPlasma[] = {
        -0.55f,0.0f, 0.0f, 0.0f,0.0f, 0.0f,1.0f,0.0f,
         0.55f,0.0f, 0.0f, 1.0f,0.0f, 0.0f,1.0f,0.0f,
         0.55f,0.0f,-3.5f, 1.0f,1.0f, 0.0f,1.0f,0.0f,
        -0.55f,0.0f,-3.5f, 0.0f,1.0f, 0.0f,1.0f,0.0f
    };
    unsigned int indicesPlasma[] = {0,1,2,0,2,3};
    mallaPlasma.CreateMeshModel(verticesPlasma, indicesPlasma, 32, 6);
    glm::mat4 matricesPlasma[2] = {
        glm::translate(matrizHippocrates, glm::vec3(-1.8f,0.0f,-5.06f)),
        glm::translate(matrizHippocrates, glm::vec3( 1.8f,0.0f,-5.06f))
    };
    // Luces de la practica 8
    static_assert(MAX_POINT_LIGHTS >= 5 && MAX_SPOT_LIGHTS >= 6, "Capacidad de luces insuficiente");
    const unsigned int indiceFaroRover = spotLightCount++;
    spotLights[indiceFaroRover] = SpotLight(0,0,1, 0.05f,2.0f,
        0,0,0, -1,-0.8f,0, 1,0.05f,0.01f,30);
    const unsigned int indiceFaroAvionInferior = spotLightCount++;
    const unsigned int indiceFaroAvionFrontal = spotLightCount++;
    const unsigned int indiceSpotNave = spotLightCount++;
    spotLights[indiceFaroAvionFrontal] = SpotLight(1,1,1, 0.05f,2.0f,
        0,0,0, 0,0,1, 1,0.02f,0.002f,35);
    spotLights[indiceSpotNave] = SpotLight(0.65f,0.20f,1, 0.02f,3.0f,
        0,0,0, 0,0,-1, 1,0.045f,0.0075f,40);
    const unsigned int indiceLuzBrazo = pointLightCount++;
    const unsigned int indicePuntualNave = pointLightCount++;
    const unsigned int indicePlasma = pointLightCount;
    pointLightCount += 2;
    const glm::vec3 coloresFaroAvion[6] = {
        {1,0,0}, {1,1,0}, {0,1,0}, {0,1,1}, {0,0,1}, {1,0,1}
    };

    // nave en circuito
    std::vector<glm::vec3> recorridoNave, direccionesNave;
    std::vector<double> distanciasNave;
    const int puntosBaseNave = static_cast<int>(puntosCircuito.size());
    const int subdivisionesNave = 8;
    double longitudCircuitoNave = 0.0;
    for (int i=0; i<puntosBaseNave; ++i)
    {
        const glm::vec3 p0 = puntosCircuito[(i+puntosBaseNave-1)%puntosBaseNave];
        const glm::vec3 p1 = puntosCircuito[i];
        const glm::vec3 p2 = puntosCircuito[(i+1)%puntosBaseNave];
        const glm::vec3 p3 = puntosCircuito[(i+2)%puntosBaseNave];
        for (int k=0; k<subdivisionesNave; ++k)
        {
            const float t = static_cast<float>(k)/subdivisionesNave;
            const glm::vec3 a = -p0 + p2;
            const glm::vec3 b = 2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3;
            const glm::vec3 c = -p0 + 3.0f*p1 - 3.0f*p2 + p3;
            const glm::vec3 punto = 0.5f*(2.0f*p1 + a*t + b*t*t + c*t*t*t);
            const glm::vec3 direccion = glm::normalize(0.5f*(a + 2.0f*b*t + 3.0f*c*t*t));
            if (!recorridoNave.empty()) longitudCircuitoNave += glm::length(punto-recorridoNave.back());
            recorridoNave.push_back(punto);
            direccionesNave.push_back(direccion);
            distanciasNave.push_back(longitudCircuitoNave);
        }
    }
    // repite el inicio del circuito
    longitudCircuitoNave += glm::length(recorridoNave.front()-recorridoNave.back());
    recorridoNave.push_back(recorridoNave.front());
    direccionesNave.push_back(direccionesNave.front());
    distanciasNave.push_back(longitudCircuitoNave);
    const double velocidadNaveCircuito = 12.0; 
    const float alturaSobrePistaNave = 2.0f; 
    double distanciaNaveCircuito = distanciasNave[140*subdivisionesNave];
    // Carga del satelite
    CuerpoSatelite_M.LoadModel("Models/Satelite/cuerposatelite.fbx");
	AlaDer1_M.LoadModel("Models/Satelite/alader1.fbx");
	AlaDer2_M.LoadModel("Models/Satelite/alader2.fbx");
	AlaIzq1_M.LoadModel("Models/Satelite/alaizq1.fbx");
	AlaIzq2_M.LoadModel("Models/Satelite/alaizq2.fbx");

	glm::vec3 posicionSatelite = glm::vec3(32.0f, 22.0f, 45.0f);
	GLfloat giroAlaDer1 = 0.0f, giroAlaDer2 = 0.0f;
	GLfloat giroAlaIzq1 = 0.0f, giroAlaIzq2 = 0.0f;
    // recorrido satelite
    double faseRecorridoSatelite = 0.0;
    double faseAlasSatelite = 0.0;
    const double vueltaSatelite = 6.283185307179586;
    const GLfloat radioRecorridoSatelite = 14.0f;

	    lastTime = static_cast<GLfloat>(glfwGetTime());
	////Loop mientras no se cierra la ventanaf
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
        const GLfloat deltaTimeRover = deltaTime; // segundos reales, antes del ajuste original
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
        // se comparte el tiempo ya usado por rover y avion.
        distanciaNaveCircuito = std::fmod(distanciaNaveCircuito
            + velocidadNaveCircuito * deltaTimeRover, longitudCircuitoNave);
        size_t tramoNave = 0;
        while (tramoNave+1 < distanciasNave.size()-1
            && distanciasNave[tramoNave+1] <= distanciaNaveCircuito)
            ++tramoNave;
        const float avanceTramoNave = static_cast<float>(
            (distanciaNaveCircuito-distanciasNave[tramoNave]) /
            (distanciasNave[tramoNave+1]-distanciasNave[tramoNave]));
        const glm::vec3 posicionNaveCircuito = glm::mix(
            recorridoNave[tramoNave], recorridoNave[tramoNave+1], avanceTramoNave)
            + glm::vec3(0.0f, alturaSobrePistaNave, 0.0f);
        const glm::vec3 frenteNaveCircuito = glm::normalize(glm::mix(
            direccionesNave[tramoNave], direccionesNave[tramoNave+1], avanceTramoNave));
        // El eje local +Z es la nariz
        const glm::vec3 derechaNaveCircuito = glm::normalize(glm::cross(
            glm::vec3(0.0f,1.0f,0.0f), frenteNaveCircuito));
        const glm::vec3 arribaNaveCircuito = glm::normalize(glm::cross(
            frenteNaveCircuito, derechaNaveCircuito));
        glm::mat4 orientacionNaveCircuito(1.0f);
        orientacionNaveCircuito[0] = glm::vec4(derechaNaveCircuito,0.0f);
        orientacionNaveCircuito[1] = glm::vec4(arribaNaveCircuito,0.0f);
        orientacionNaveCircuito[2] = glm::vec4(frenteNaveCircuito,0.0f);
        matrizHippocrates = glm::translate(glm::mat4(1.0f), posicionNaveCircuito)
            * orientacionNaveCircuito;
        matrizHippocrates = glm::scale(matrizHippocrates, glm::vec3(0.65f));
        // Los motores heredan la traslacion y la inclinacion de la nave
        matricesPlasma[0] = glm::translate(matrizHippocrates, glm::vec3(-1.8f,0.0f,-5.06f));
        matricesPlasma[1] = glm::translate(matrizHippocrates, glm::vec3( 1.8f,0.0f,-5.06f));
        // fin de la animacion de la nave
        // Avanzar / retroceder mediante una bandera y condicionales
        const GLfloat posicionAnteriorRover = posicionRoverX;
        if (avanza)
        {
            posicionRoverX += velocidadRover * deltaTimeRover;
            if (posicionRoverX >= limiteRoverDer)
            {
                posicionRoverX = limiteRoverDer;
                avanza = false;
            }
        }
        else
        {
            posicionRoverX -= velocidadRover * deltaTimeRover;
            if (posicionRoverX <= limiteRoverIzq)
            {
                posicionRoverX = limiteRoverIzq;
                avanza = true;
            }
        }
        // Al aumentar X la rueda gira hacia -Z, al retroceder invierte el giro
        const GLfloat distanciaRover = posicionRoverX - posicionAnteriorRover;
        giroLlantasRover -= (distanciaRover / radioRuedaRover) / toRadians;
        giroLlantasRover = std::fmod(giroLlantasRover, 360.0f);

		angulovaria += 0.5f*deltaTime;

		//rover avanza
		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
;
		}
	

		//Recibir eventos del usuario
		glfwPollEvents();
        // animacion del satelite
        faseRecorridoSatelite = std::fmod(faseRecorridoSatelite
            + (vueltaSatelite / 30.0) * deltaTimeRover, vueltaSatelite);
        faseAlasSatelite = std::fmod(faseAlasSatelite
            + (vueltaSatelite / 12.0) * deltaTimeRover, vueltaSatelite);
        // Inicio permite volver al comienzo de la animacion automatica.
        if (mainWindow.getsKeys()[GLFW_KEY_HOME])
        {
            faseRecorridoSatelite = 0.0;
            faseAlasSatelite = 0.0;
        }
        posicionSatelite = glm::vec3(
            18.0f + radioRecorridoSatelite * static_cast<float>(std::cos(faseRecorridoSatelite)),
            22.0f + radioRecorridoSatelite * static_cast<float>(std::sin(faseRecorridoSatelite)),
            45.0f);
        // Plegado de las alas
        // Se reutilizan las cuatro articulaciones y sus matrices padre/hijo
        const GLfloat plegadoSatelite = 0.5f * (1.0f - static_cast<float>(std::cos(faseAlasSatelite)));
        giroAlaDer1 = giroAlaIzq1 = 20.0f * plegadoSatelite;
        giroAlaDer2 = giroAlaIzq2 = 30.0f * plegadoSatelite;

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
            GLfloat tiempoVueloPendiente = deltaTimeRover;
            while (tiempoVueloPendiente > 0.0f)
            {
                const GLfloat pasoVuelo = glm::min(tiempoVueloPendiente, 1.0f / 60.0f);
                const GLfloat dxInicial = -radioAvionX * std::sin(faseAvion);
                const GLfloat dzInicial =  radioAvionZ * std::cos(faseAvion);
                const GLfloat longitudInicial = std::sqrt(dxInicial * dxInicial + dzInicial * dzInicial);
                const GLfloat faseMedia = faseAvion + 0.5f * velocidadAvion * pasoVuelo / longitudInicial;
                const GLfloat dxMedio = -radioAvionX * std::sin(faseMedia);
                const GLfloat dzMedio =  radioAvionZ * std::cos(faseMedia);
                faseAvion += velocidadAvion * pasoVuelo / std::sqrt(dxMedio * dxMedio + dzMedio * dzMedio);
                faseAvion = std::fmod(faseAvion, 360.0f * toRadians);
                tiempoVueloPendiente -= pasoVuelo;
            }

            const glm::vec3 posicionAvion(
                5.0f + radioAvionX * std::cos(faseAvion),
                alturaAvion,
                -10.0f + radioAvionZ * std::sin(faseAvion));
            // La nariz del modelo apunta a +Z
            const GLfloat direccionAvionX = -radioAvionX * std::sin(faseAvion);
            const GLfloat direccionAvionZ =  radioAvionZ * std::cos(faseAvion);
            const GLfloat rumboAvion = std::atan2(direccionAvionX, direccionAvionZ);
            const GLfloat longitudTangente = std::sqrt(
                direccionAvionX * direccionAvionX + direccionAvionZ * direccionAvionZ);
            const GLfloat curvaturaAvion = (radioAvionX * radioAvionZ) /
                (longitudTangente * longitudTangente * longitudTangente);
            // alabeo hacia el interior
            const GLfloat alabeoAvion = std::atan(
                velocidadAvion * velocidadAvion * curvaturaAvion / 9.81f);
            glm::mat4 padreAvion = glm::translate(glm::mat4(1.0f), posicionAvion);
            padreAvion = glm::rotate(padreAvion, rumboAvion, glm::vec3(0.0f, 1.0f, 0.0f));
            padreAvion = glm::rotate(padreAvion, alabeoAvion, glm::vec3(0.0f, 0.0f, 1.0f));
            padreAvion = glm::scale(padreAvion, glm::vec3(0.90f));
        // Matriz compartida por el rover y sus luces 
        glm::mat4 roverPadre = glm::translate(glm::mat4(1.0f), glm::vec3(posicionRoverX,-2.0f,12.0f));
        roverPadre = glm::scale(roverPadre, glm::vec3(0.85f));
        glm::vec3 posicionLuz = glm::vec3(roverPadre * glm::vec4(-5.2f,4.2f,0.0f,1));
        glm::vec3 direccionLuz = glm::normalize(glm::mat3(roverPadre) * glm::vec3(-1,-0.8f,0));
        spotLights[indiceFaroRover].SetFlash(posicionLuz,direccionLuz);
        // El brazo importado permanece en reposo
        posicionLuz = glm::vec3(roverPadre * glm::vec4(3.9757690f,12.1092786f,-0.9108811f,1));
        pointLights[indiceLuzBrazo] = PointLight(0,1,0, 0.03f,1.2f,
            posicionLuz.x,posicionLuz.y,posicionLuz.z, 1,0.09f,0.032f);

        posicionLuz = glm::vec3(padreAvion * glm::vec4(0,-0.8f,0,1));
        direccionLuz = glm::normalize(glm::mat3(padreAvion) * glm::vec3(0,-1,0));
        const glm::vec3 colorFaro = coloresFaroAvion[mainWindow.getColorFaroAvion()];
        spotLights[indiceFaroAvionInferior] = SpotLight(colorFaro.r,colorFaro.g,colorFaro.b, 0.02f,1.4f,
            posicionLuz.x,posicionLuz.y,posicionLuz.z, direccionLuz.x,direccionLuz.y,direccionLuz.z,
            1,0.09f,0.032f,30);
        posicionLuz = glm::vec3(padreAvion * glm::vec4(0,-0.1f,2,1));
        direccionLuz = glm::normalize(glm::mat3(padreAvion) * glm::vec3(0,0,1));
        spotLights[indiceFaroAvionFrontal].SetFlash(posicionLuz,direccionLuz);

        motorPuntualHippocrates = glm::vec3(matrizHippocrates * glm::vec4(1.8f,0,-5.05f,1));
        motorSpotHippocrates = glm::vec3(matrizHippocrates * glm::vec4(-1.8f,0,-5.05f,1));
        direccionMotorHippocrates = glm::normalize(glm::mat3(matrizHippocrates) * glm::vec3(0,0,-1));
        pointLights[indicePuntualNave] = PointLight(0.65f,0.20f,1, 0.03f,2.0f,
            motorPuntualHippocrates.x,motorPuntualHippocrates.y,motorPuntualHippocrates.z, 1,0.09f,0.032f);
        spotLights[indiceSpotNave].SetFlash(motorSpotHippocrates,direccionMotorHippocrates);
        for (int motor=0; motor<2; ++motor)
        {
            posicionLuz = glm::vec3(matricesPlasma[motor] * glm::vec4(0,0,0,1));
            pointLights[indicePlasma+motor] = PointLight(0,0.25f,1, 0.02f,1.5f,
                posicionLuz.x,posicionLuz.y,posicionLuz.z, 1,0.09f,0.032f);
        }
        PointLight puntualesActivas[MAX_POINT_LIGHTS];
        unsigned int puntualesActivasCount = 0;
        for (unsigned int i=0; i<pointLightCount; ++i)
        {
            if (i==indiceLuzBrazo && !mainWindow.getLuzPinza()) continue;
            if (i==indicePuntualNave && !mainWindow.getPuntualNave()) continue;
            if (i>=indicePlasma && !mainWindow.getPlasma(static_cast<int>(i-indicePlasma))) continue;
            puntualesActivas[puntualesActivasCount++] = pointLights[i];
        }
        SpotLight spotsActivos[MAX_SPOT_LIGHTS];
        unsigned int spotsActivosCount = 0;
        for (unsigned int i=0; i<spotLightCount; ++i)
        {
            if (i==indiceFaroRover && !mainWindow.getFaroRover()) continue;
            if ((i==indiceFaroAvionInferior || i==indiceFaroAvionFrontal) && !mainWindow.getFarosAvion()) continue;
            if (i==indiceSpotNave && !mainWindow.getSpotNave()) continue;
            spotsActivos[spotsActivosCount++] = spotLights[i];
        }

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(puntualesActivas, puntualesActivasCount);
		shaderList[0].SetSpotLights(spotsActivos, spotsActivosCount);


		//reinicializa las variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMeshModel();
        //satelite
        {
            GLint programaSatelite=0, unidadSatelite=0, texturaSatelite=0;
            GLfloat modeloAnteriorSatelite[16], colorAnteriorSatelite[3], offsetAnteriorSatelite[2];
            GLfloat especularAnteriorSatelite=0.0f, brilloAnteriorSatelite=0.0f;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaSatelite);
            glGetUniformfv(programaSatelite, uniformModel, modeloAnteriorSatelite);
            glGetUniformfv(programaSatelite, uniformColor, colorAnteriorSatelite);
            glGetUniformfv(programaSatelite, uniformTextureOffset, offsetAnteriorSatelite);
            glGetUniformfv(programaSatelite, uniformSpecularIntensity, &especularAnteriorSatelite);
            glGetUniformfv(programaSatelite, uniformShininess, &brilloAnteriorSatelite);
            glGetIntegerv(GL_ACTIVE_TEXTURE, &unidadSatelite);
            glActiveTexture(GL_TEXTURE0); glGetIntegerv(GL_TEXTURE_BINDING_2D, &texturaSatelite);
            glm::mat4 model(1.0f);
            glm::vec3 color(1.0f);
            glUniform2f(uniformTextureOffset,0.0f,0.0f);
            plainTexture.UseTexture();
            Material_opaco.UseMaterial(uniformSpecularIntensity,uniformShininess);
//satelite, todo el conjunto hereda su traslacion en X, Y y Z
		glm::mat4 satelitePadre = glm::translate(glm::mat4(1.0f), posicionSatelite);
		satelitePadre = glm::scale(satelitePadre, glm::vec3(0.65f));
		GLfloat escalaSateliteFBX = 0.16809055f;
		glm::vec3 pivoteAlaDer1 = glm::vec3(1.91174454f, 0.65587784f, 0.58370453f);
		glm::vec3 pivoteAlaDer2 = glm::vec3(7.38424561f, 2.50917572f, 0.56575607f);
		glm::vec3 pivoteAlaIzq1 = glm::vec3(-1.91173248f, 0.65587784f, 0.58370453f);
		glm::vec3 pivoteAlaIzq2 = glm::vec3(-7.38423401f, 2.50917572f, 0.56575607f);

		color = glm::vec3(0.75f, 0.78f, 0.82f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::scale(satelitePadre, glm::vec3(escalaSateliteFBX));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoSatelite_M.RenderModel();

		color = glm::vec3(0.08f, 0.25f, 0.55f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glm::mat4 alaPadreDer1 = glm::translate(satelitePadre, pivoteAlaDer1);
		alaPadreDer1 = glm::rotate(alaPadreDer1, glm::radians(giroAlaDer1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(alaPadreDer1, glm::vec3(escalaSateliteFBX));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaDer1_M.RenderModel();

		glm::mat4 alaPadreDer2 = glm::translate(alaPadreDer1, pivoteAlaDer2 - pivoteAlaDer1);
		alaPadreDer2 = glm::rotate(alaPadreDer2, glm::radians(giroAlaDer2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(alaPadreDer2, glm::vec3(escalaSateliteFBX));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaDer2_M.RenderModel();

		glm::mat4 alaPadreIzq1 = glm::translate(satelitePadre, pivoteAlaIzq1);
		alaPadreIzq1 = glm::rotate(alaPadreIzq1, glm::radians(-giroAlaIzq1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(alaPadreIzq1, glm::vec3(escalaSateliteFBX));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaIzq1_M.RenderModel();

		glm::mat4 alaPadreIzq2 = glm::translate(alaPadreIzq1, pivoteAlaIzq2 - pivoteAlaIzq1);
		alaPadreIzq2 = glm::rotate(alaPadreIzq2, glm::radians(-giroAlaIzq2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(alaPadreIzq2, glm::vec3(escalaSateliteFBX));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaIzq2_M.RenderModel();

		
            glUniformMatrix4fv(uniformModel,1,GL_FALSE,modeloAnteriorSatelite);
            glUniform3fv(uniformColor,1,colorAnteriorSatelite);
            glUniform2fv(uniformTextureOffset,1,offsetAnteriorSatelite);
            glUniform1f(uniformSpecularIntensity,especularAnteriorSatelite);
            glUniform1f(uniformShininess,brilloAnteriorSatelite);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(texturaSatelite));
            glActiveTexture(static_cast<GLenum>(unidadSatelite));
        }
        //fin saltelite
        //circuito
        {
            GLint programaPista=0, unidadPista=0, texturaPista=0;
            GLfloat modeloPistaAnterior[16], colorPistaAnterior[3], offsetPistaAnterior[2];
            GLfloat especularPista=0.0f, brilloPista=0.0f;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaPista);
            glGetUniformfv(programaPista, uniformModel, modeloPistaAnterior);
            glGetUniformfv(programaPista, uniformColor, colorPistaAnterior);
            glGetUniformfv(programaPista, uniformTextureOffset, offsetPistaAnterior);
            glGetUniformfv(programaPista, uniformSpecularIntensity, &especularPista);
            glGetUniformfv(programaPista, uniformShininess, &brilloPista);
            glGetIntegerv(GL_ACTIVE_TEXTURE, &unidadPista);
            glActiveTexture(GL_TEXTURE0); glGetIntegerv(GL_TEXTURE_BINDING_2D, &texturaPista);
            const glm::mat4 modeloCircuito(1.0f); // vertices ya ubicados en la escena
            glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(modeloCircuito));
            glUniform2f(uniformTextureOffset,0.0f,0.0f);
            plainTexture.UseTexture();
            Material_opaco.UseMaterial(uniformSpecularIntensity,uniformShininess);
            const glm::vec3 coloresCircuito[3] = {
                glm::vec3(0.38f,0.38f,0.40f),
                glm::vec3(0.92f,0.92f,0.88f),
                glm::vec3(0.28f,0.28f,0.29f)
            };
            for (int grupo=0; grupo<3; ++grupo)
            {
                glUniform3fv(uniformColor,1,glm::value_ptr(coloresCircuito[grupo]));
                circuitoMallas[grupo].RenderMeshModel();
            }
            glUniformMatrix4fv(uniformModel,1,GL_FALSE,modeloPistaAnterior);
            glUniform3fv(uniformColor,1,colorPistaAnterior);
            glUniform2fv(uniformTextureOffset,1,offsetPistaAnterior);
            glUniform1f(uniformSpecularIntensity,especularPista);
            glUniform1f(uniformShininess,brilloPista);
            glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(texturaPista));
            glActiveTexture(static_cast<GLenum>(unidadPista));
        }
        //fin dibujo del circuito



		//Instancia del rover
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche+.0f, -1.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();
		// agregado del recorrido automatico hacia las orillas
		{
            glm::mat4 model(1.0f), modelaux(1.0f);
            glm::vec3 color(1.0f);
            const GLfloat giroBaseBrazo = 0.0f;
            const GLfloat giroBrazo1 = 0.0f;
            const GLfloat giroBrazo2 = 0.0f;
            const GLfloat giroBrazo3 = 0.0f;
            const GLfloat giroPataDer1 = 0.0f;
            const GLfloat giroPataDer2 = 0.0f;
            const GLfloat giroPataIzq1 = 0.0f;
            const GLfloat giroPataIzq2 = 0.0f;
            const GLfloat giroRuedaDer1 = giroLlantasRover;
            const GLfloat giroRuedaDer2 = giroLlantasRover;
            const GLfloat giroRuedaDer3 = giroLlantasRover;
            const GLfloat giroRuedaIzq1 = giroLlantasRover;
            const GLfloat giroRuedaIzq2 = giroLlantasRover;
            const GLfloat giroRuedaIzq3 = giroLlantasRover;
            const GLfloat giros = 0.0f;
            plainTexture.UseTexture();
            Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//transformacion general del rover dentro de la escena
        // roverPadre ya se calculo junto con sus luces antes del dibujo.

		//cuerpo
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		model = roverPadre;
		//rotacion que trae el cuerpo desde Blender al exportarlo en FBX.
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_Rover_M.RenderModel();

		//brazo
		//pivote base del brazo: (1.7011536, 5.9816516, -1.1000017)
		glm::vec3 pivoteBrazo1 = glm::vec3(1.7011536f, 5.9816516f, -1.1000017f);
		glm::vec3 pivoteBrazo2 = glm::vec3(5.2812018f, 7.6468542f, -1.0205271f);
		glm::vec3 pivoteBrazo3 = glm::vec3(3.9757690f, 12.1092786f, -0.9108811f);

		color = glm::vec3(0.65f, 0.65f, 0.65f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//jerarquia: cuerpo->base->brazo1->brazo2->brazo3.
		//modelaux acumula las articulaciones: cada hijo hereda todos los giros anteriores.
		//Base
		modelaux = glm::translate(roverPadre, pivoteBrazo1);
		modelaux = glm::rotate(modelaux, glm::radians(giroBaseBrazo), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(modelaux, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_BaseBrazo_M.RenderModel();

		//brazo 1: hereda la base y agrega su giro en el pivote compartido.
		modelaux = glm::rotate(modelaux, glm::radians(giroBrazo1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(modelaux, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_BrazoSup1_M.RenderModel();

		//brazo 2: avanzar desde el pivote del brazo 1 hasta el suyo.
		modelaux = glm::translate(modelaux, pivoteBrazo2 - pivoteBrazo1);
		modelaux = glm::rotate(modelaux, glm::radians(giroBrazo2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(modelaux, glm::radians(-29.6691909f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_BrazoSup2_M.RenderModel();

		//brazo 3: hereda el brazo 2 y agrega su desplazamiento relativo.
		modelaux = glm::translate(modelaux, pivoteBrazo3 - pivoteBrazo2);
		modelaux = glm::rotate(modelaux, glm::radians(giroBrazo3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(modelaux, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_BrazoSup3_M.RenderModel();

		//patas y ruedas

		glm::vec3 pivotePataDer1 = glm::vec3(2.0543056f, 4.1915887f, 3.0576987f);
		glm::vec3 pivotePataDer2 = glm::vec3(-3.0361563f, 3.3108429f, 3.0040000f);
		glm::vec3 pivotePataIzq1 = glm::vec3(2.0543079f, 4.1915875f, -3.0617017f);
		glm::vec3 pivotePataIzq2 = glm::vec3(-2.9279413f, 3.3210410f, -2.9994592f);

		//las patas 1 giran lateralmente alrededor de Y, con signos opuestos
		//para abrir hacia fuera a ambos lados. Las patas 2 conservan su giro en Z.
		glm::mat4 pataPadreDer1 = glm::translate(roverPadre, pivotePataDer1);
		pataPadreDer1 = glm::rotate(pataPadreDer1, glm::radians(-giroPataDer1), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 pataPadreDer2 = glm::translate(roverPadre, pivotePataDer2);
		pataPadreDer2 = glm::rotate(pataPadreDer2, glm::radians(giroPataDer2), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 pataPadreIzq1 = glm::translate(roverPadre, pivotePataIzq1);
		pataPadreIzq1 = glm::rotate(pataPadreIzq1, glm::radians(giroPataIzq1), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 pataPadreIzq2 = glm::translate(roverPadre, pivotePataIzq2);
		pataPadreIzq2 = glm::rotate(pataPadreIzq2, glm::radians(giroPataIzq2), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.65f, 0.65f, 0.65f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//pata derecha 1
		model = pataPadreDer1;
		model = glm::rotate(model, glm::radians(3.3458589f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_PataDer1_M.RenderModel();

		//pata derecha 2
		model = pataPadreDer2;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_PataDer2_M.RenderModel();

		//pata izquierda 1
		model = pataPadreIzq1;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_PataIzq1_M.RenderModel();

		//pata izquierda 2
		model = pataPadreIzq2;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_PataIzq2_M.RenderModel();

		//ruedas: color oscuro
		color = glm::vec3(0.12f, 0.12f, 0.12f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glm::vec3 posRuedaDer1 = glm::vec3(5.7111499f, 1.7056517f, 3.0323120f);
		glm::vec3 posRuedaDer2 = glm::vec3(-0.9459518f, 1.4127582f, 4.1212643f);
		glm::vec3 posRuedaDer3 = glm::vec3(-5.1317432f, 1.4127582f, 4.1212643f);

		glm::vec3 posRuedaIzq1 = glm::vec3(5.7111572f, 1.7056511f, -3.0323157f);
		glm::vec3 posRuedaIzq2 = glm::vec3(-0.9459507f, 1.4127559f, -4.1212643f);
		glm::vec3 posRuedaIzq3 = glm::vec3(-5.1121884f, 1.3864330f, -4.1212643f);

		//rueda delantera derecha: hija de PataDer1.
		model = pataPadreDer1;
		model = glm::translate(model, posRuedaDer1 - pivotePataDer1);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaDer1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_RuedaDer1_M.RenderModel();

		//ruedas media y trasera derechas: hijas de PataDer2.
		model = pataPadreDer2;
		model = glm::translate(model, posRuedaDer2 - pivotePataDer2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaDer2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_RuedaDer2_M.RenderModel();

		model = pataPadreDer2;
		model = glm::translate(model, posRuedaDer3 - pivotePataDer2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaDer3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_RuedaDer3_M.RenderModel();

		//rueda delantera izquierda: hija de PataIzq1.
		model = pataPadreIzq1;
		model = glm::translate(model, posRuedaIzq1 - pivotePataIzq1);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaIzq1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_RuedaIzq1_M.RenderModel();

		//ruedas media y trasera izquierdas: hijas de PataIzq2.
		model = pataPadreIzq2;
		model = glm::translate(model, posRuedaIzq2 - pivotePataIzq2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaIzq2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_RuedaIzq2_M.RenderModel();

		model = pataPadreIzq2;
		model = glm::translate(model, posRuedaIzq3 - pivotePataIzq2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaIzq3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		P9Separado_RuedaIzq3_M.RenderModel();


		}
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//fin agregado
        //agrega avion
        {
            GLint programaAvion = 0, unidadAnteriorAvion = 0, texturaAnteriorAvion = 0;
            GLfloat colorAnteriorAvion[3], modeloAnteriorAvion[16], offsetAnteriorAvion[2];
            GLfloat especularAnteriorAvion = 0.0f, brilloAnteriorAvion = 0.0f;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaAvion);
            glGetUniformfv(programaAvion, uniformColor, colorAnteriorAvion);
            glGetUniformfv(programaAvion, uniformModel, modeloAnteriorAvion);
            glGetUniformfv(programaAvion, uniformTextureOffset, offsetAnteriorAvion);
            glGetUniformfv(programaAvion, uniformSpecularIntensity, &especularAnteriorAvion);
            glGetUniformfv(programaAvion, uniformShininess, &brilloAnteriorAvion);
            glGetIntegerv(GL_ACTIVE_TEXTURE, &unidadAnteriorAvion);
            glActiveTexture(GL_TEXTURE0);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &texturaAnteriorAvion);

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(padreAvion));
            glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
            glUniform2f(uniformTextureOffset, 0.0f, 0.0f);
            Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
            P9AvionCuerpo.RenderModel();
            P9AvionCara.RenderModel();
            P9AvionAlas.RenderModel();

            glUniform3fv(uniformColor, 1, colorAnteriorAvion);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, modeloAnteriorAvion);
            glUniform2fv(uniformTextureOffset, 1, offsetAnteriorAvion);
            glUniform1f(uniformSpecularIntensity, especularAnteriorAvion);
            glUniform1f(uniformShininess, brilloAnteriorAvion);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texturaAnteriorAvion));
            glActiveTexture(static_cast<GLenum>(unidadAnteriorAvion));
        }
        //fin agregado

        // nave
        {
            GLint programaAnteriorP9 = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaAnteriorP9);
            shaderNaveP9.UseShader();
            GLint programaActualP9 = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaActualP9);
            GLuint uniformModel = shaderNaveP9.GetModelLocation();
            GLuint uniformColor = shaderNaveP9.getColorLocation();
            glUniformMatrix4fv(shaderNaveP9.GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(shaderNaveP9.GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            glUniform2f(shaderNaveP9.getOffsetLocation(), 0.0f, 0.0f);
            const glm::vec3 ojoNaveP9 = camera.getCameraPosition();
            glUniform3fv(shaderNaveP9.GetEyePositionLocation(), 1, glm::value_ptr(ojoNaveP9));
            shaderNaveP9.SetDirectionalLight(&mainLight);
            shaderNaveP9.SetPointLights(puntualesActivas, puntualesActivasCount);
            shaderNaveP9.SetSpotLights(spotsActivos, spotsActivosCount);
            Material_opaco.UseMaterial(shaderNaveP9.GetSpecularIntensityLocation(), shaderNaveP9.GetShininessLocation());
            glUniform1i(glGetUniformLocation(programaActualP9, "theTexture"), 0);
            glUniform1i(glGetUniformLocation(programaActualP9, "plasmaP9"), 0);
        {
            GLint programaNave=0, unidadAnteriorNave=0, textura0AnteriorNave=0, textura1AnteriorNave=0;
            GLfloat modeloAnteriorNave[16], colorAnteriorNave[3];
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaNave);
            glGetUniformfv(programaNave, uniformModel, modeloAnteriorNave);
            glGetUniformfv(programaNave, uniformColor, colorAnteriorNave);
            glGetIntegerv(GL_ACTIVE_TEXTURE, &unidadAnteriorNave);
            glActiveTexture(GL_TEXTURE0);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &textura0AnteriorNave);
            glActiveTexture(GL_TEXTURE1);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &textura1AnteriorNave);
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texturaEmisionHippocrates));
            GLint uniformEmision = glGetUniformLocation(programaNave, "emissionStrength");
            glUniform1i(glGetUniformLocation(programaNave, "emissionTexture"), 1);
            glUniform1f(uniformEmision, 1.5f);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizHippocrates));
            glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
            naveDifusaP9.UseTexture();
            Hippocrates_M.RenderModel();
            glUniform1f(uniformEmision, 0.0f);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, modeloAnteriorNave);
            glUniform3fv(uniformColor, 1, colorAnteriorNave);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(textura1AnteriorNave));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(textura0AnteriorNave));
            glActiveTexture(static_cast<GLenum>(unidadAnteriorNave));
        }
            glUseProgram(static_cast<GLuint>(programaAnteriorP9));
        }

		//Agave ¿qué sucede si lo renderizan antes del Rover?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMeshModel();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMeshModel();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		for (int i = 1; i<4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();

		 }

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();
		}
 

		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		toffsetnumerocambiau += 0.25; 
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		//if
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		glDisable(GL_BLEND);
        // NAVE P8: programa independiente, conservando el estado de la escena.
        {
            GLint programaAnteriorP9 = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaAnteriorP9);
            shaderNaveP9.UseShader();
            GLint programaActualP9 = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaActualP9);
            GLuint uniformModel = shaderNaveP9.GetModelLocation();
            GLuint uniformColor = shaderNaveP9.getColorLocation();
            glUniformMatrix4fv(shaderNaveP9.GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(shaderNaveP9.GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            glUniform2f(shaderNaveP9.getOffsetLocation(), 0.0f, 0.0f);
            const glm::vec3 ojoNaveP9 = camera.getCameraPosition();
            glUniform3fv(shaderNaveP9.GetEyePositionLocation(), 1, glm::value_ptr(ojoNaveP9));
            shaderNaveP9.SetDirectionalLight(&mainLight);
            shaderNaveP9.SetPointLights(puntualesActivas, puntualesActivasCount);
            shaderNaveP9.SetSpotLights(spotsActivos, spotsActivosCount);
            Material_opaco.UseMaterial(shaderNaveP9.GetSpecularIntensityLocation(), shaderNaveP9.GetShininessLocation());
            glUniform1i(glGetUniformLocation(programaActualP9, "theTexture"), 0);
            glUniform1i(glGetUniformLocation(programaActualP9, "plasmaP9"), 1);
        // Ambos motores permanecen encendidos en esta practica.
        if (mainWindow.getPlasma())
        {
            GLint programaPlasma=0, unidadPlasma=0, textura0Plasma=0, textura1Plasma=0;
            GLint srcRGB=0,dstRGB=0,srcAlpha=0,dstAlpha=0,ecuacionRGB=0,ecuacionAlpha=0;
            GLfloat modeloPrevioPlasma[16], colorPrevioPlasma[3], emisionPrevia=0;
            GLboolean profundidadPlasma=GL_TRUE;
            GLboolean blendPlasma=glIsEnabled(GL_BLEND), cullPlasma=glIsEnabled(GL_CULL_FACE);
            glGetIntegerv(GL_CURRENT_PROGRAM,&programaPlasma);
            glGetUniformfv(programaPlasma,uniformModel,modeloPrevioPlasma);
            glGetUniformfv(programaPlasma,uniformColor,colorPrevioPlasma);
            GLint uniformEmisionPlasma=glGetUniformLocation(programaPlasma,"emissionStrength");
            glGetUniformfv(programaPlasma,uniformEmisionPlasma,&emisionPrevia);
            glGetIntegerv(GL_ACTIVE_TEXTURE,&unidadPlasma);
            glActiveTexture(GL_TEXTURE0); glGetIntegerv(GL_TEXTURE_BINDING_2D,&textura0Plasma);
            glActiveTexture(GL_TEXTURE1); glGetIntegerv(GL_TEXTURE_BINDING_2D,&textura1Plasma);
            glGetBooleanv(GL_DEPTH_WRITEMASK,&profundidadPlasma);
            glGetIntegerv(GL_BLEND_SRC_RGB,&srcRGB); glGetIntegerv(GL_BLEND_DST_RGB,&dstRGB);
            glGetIntegerv(GL_BLEND_SRC_ALPHA,&srcAlpha); glGetIntegerv(GL_BLEND_DST_ALPHA,&dstAlpha);
            glGetIntegerv(GL_BLEND_EQUATION_RGB,&ecuacionRGB); glGetIntegerv(GL_BLEND_EQUATION_ALPHA,&ecuacionAlpha);
            glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(texturaPlasmaAzul));
            glUniform1i(glGetUniformLocation(programaPlasma,"emissionTexture"),1);
            plasmaAzulTexture.UseTexture();
            glUniform3f(uniformColor,0.0f,0.0f,0.0f); // El plasma aporta su propia emision.
            glUniform1f(uniformEmisionPlasma,1.15f);
            glEnable(GL_BLEND); glBlendEquation(GL_FUNC_ADD); glBlendFunc(GL_SRC_ALPHA,GL_ONE);
            glDepthMask(GL_FALSE); glDisable(GL_CULL_FACE);
            for (int motor=0; motor<2; ++motor)
                if (mainWindow.getPlasma(motor))
                for (int plano=0; plano<3; ++plano)
                {
                    glm::mat4 modeloPlasma=glm::rotate(matricesPlasma[motor],
                        glm::radians(60.0f*plano),glm::vec3(0,0,1));
                    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(modeloPlasma));
                    mallaPlasma.RenderMeshModel();
                }
            glDepthMask(profundidadPlasma);
            glBlendEquationSeparate(ecuacionRGB,ecuacionAlpha);
            glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);
            if (!blendPlasma) glDisable(GL_BLEND);
            if (cullPlasma) glEnable(GL_CULL_FACE);
            glUniform1f(uniformEmisionPlasma,emisionPrevia);
            glUniformMatrix4fv(uniformModel,1,GL_FALSE,modeloPrevioPlasma);
            glUniform3fv(uniformColor,1,colorPrevioPlasma);
            glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(textura1Plasma));
            glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(textura0Plasma));
            glActiveTexture(static_cast<GLenum>(unidadPlasma));
        }
            glUseProgram(static_cast<GLuint>(programaAnteriorP9));
        }

		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
