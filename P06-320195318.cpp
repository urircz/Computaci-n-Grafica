/*
Práctica 6: Texturizado
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

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model AvionConCara_M; // Avion adicional con cara y logos en las alas.
Model Holocron_M;
Model HolocronStarWars_M; // Modelo texturizado adicional.

Skybox skybox;

// Recursos del holocron adicional construido por codigo.
MeshModel holocronStarWarsCodigo;
Texture holocronStarWarsCodigoTexture;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





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


	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	// Ejercicio 1: seis logos del atlas dado-starwars.png (804 x 568).
	// Orden: frente=radial, derecha=calavera, atras=simbolo izquierdo,
	// izquierda=casco, abajo=simbolo inferior, arriba=simbolo superior.
	// UV dentro del fondo negro, sin pestañas ni bordes del desplegado.
	// V = 1 - (y + 0.5) / 568 porque LoadTextureA voltea la imagen.
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.48818408f,  0.36003521f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.67350746f,	0.36003521f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.67350746f,	0.62235915f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.48818408f,	0.62235915f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.67972637f,  0.36003521f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.86504975f,	0.36003521f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.86504975f,	0.62235915f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.67972637f,	0.62235915f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.29042289f,  0.36003521f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.10509950f,	0.36003521f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.10509950f,	0.62235915f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.29042289f,	0.62235915f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.29664179f,  0.36003521f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.48196517f,	0.36003521f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.48196517f,	0.62235915f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.29664179f,	0.62235915f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.48818408f,  0.35123239f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.67350746f,	0.35123239f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.67350746f,	0.08890845f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.48818408f,	0.08890845f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.48818408f,  0.63116197f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.67350746f,	0.63116197f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.67350746f,	0.89348592f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.48818408f,	0.89348592f,		0.0f,	-1.0f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {

	12, 1, 0,
	 12, 4, 15,
	 14, 9, 2,
	 2, 12, 15,
	 8,  3,  4,
	 15,  4,  3,
	 3, 6, 14,
	 4, 5,  8,
	 6, 7, 13,
	 9, 10, 11,
	 11, 2, 9,
	 10, 7, 5,
	 13, 14, 6,
	 2, 3, 14,
	 2, 15, 3,
	 11, 12, 2,
	 13, 9, 14,
	 8, 6, 3,
	 12, 0, 4,
	 12, 11, 1,
	 4, 0, 5,
	 6, 8, 7,
	 9, 13, 10,
	 5, 0, 1,
	 1, 11, 10,
	 10, 13, 7,
	 7, 8, 5,
	 5, 1, 10

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat holocron_vertices[] = {
		// front
		//x					y			z			S		T		NX		NY		NZ
		 -2.517274,		-2.579095,		0.002115,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//0
		-1.185055,		-2.530499,		-1.198261,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//1
		0.049649,		2.440128,		-2.417217,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//2
		0.051152,		2.399781,		2.545043,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//3
		-2.646786,		-0.139047,		2.493757,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//4
		-1.328995,		-2.578386,		1.297838,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//5
		2.593003,		-0.085222,		2.505755,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//6
		1.385028,		-2.549476,		1.308677,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//7
		-0.140696,		-2.601934,		2.558561,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//8
		2.534944,		-0.061146,		-2.580280,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//9
		1.344182,		-2.542571,		-1.294387,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//10
		0.081432,		-2.513204,		-2.426005,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//11
		-2.453980,		 -0.063388,		-2.477451,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//12
		2.632201,		-2.553892,		0.007226,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//13
		2.521206,		2.448241,		0.063388,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//14
		-2.451372,		2.421759,		0.077538,	0.0f, 0.0f,		0.0f,	0.0f,	0.0f,	//15
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 128, 84);
	meshListModel.push_back(holocron);

}



// Holocron construido por codigo con la misma geometria y UV del importado.
// Cada vertice contiene XYZ, UV y normal XYZ, como en CrearDado/CrearHolocron.
// Se separan vertices cuando una posicion necesita distintas UV o normales.
void CrearHolocronStarWarsCodigo()
{
	GLfloat holocron_vertices[] = {
		// X, Y, Z, U, V, NX, NY, NZ
		-2.593003f, -0.085222f, -2.505755f, 0.881463f, 0.436897f, 0.557400f, 0.565000f, 0.608400f, // 0
		0.140696f, -2.601934f, -2.558561f, 0.778382f, 0.328399f, 0.495600f, 0.561000f, 0.663100f, // 1
		-1.385028f, -2.549476f, -1.308677f, 0.965973f, 0.330171f, 0.557100f, 0.565000f, 0.608600f, // 2
		2.453980f, -0.063388f, 2.477451f, 0.367060f, 0.684152f, -0.999900f, -0.001200f, 0.016500f, // 3
		2.646786f, -0.139047f, -2.493757f, 0.182588f, 0.485567f, -0.999900f, -0.001900f, 0.017000f, // 4
		2.451372f, 2.421759f, -0.077538f, 0.364115f, 0.482761f, -0.999900f, -0.001900f, 0.016900f, // 5
		-2.521206f, 2.448241f, -0.063388f, 0.822735f, 0.670407f, 0.566700f, -0.587600f, -0.577600f, // 6
		-2.534944f, -0.061146f, 2.580280f, 0.819639f, 0.478581f, 0.566600f, -0.587600f, -0.577600f, // 7
		-0.049649f, 2.440128f, 2.417217f, 0.969525f, 0.577272f, 0.566600f, -0.587700f, -0.577500f, // 8
		-0.049649f, 2.440128f, 2.417217f, 0.562626f, 0.342562f, -0.577600f, -0.587600f, -0.566600f, // 9
		2.453980f, -0.063388f, 2.477451f, 0.470226f, 0.451159f, -0.577600f, -0.587600f, -0.566700f, // 10
		2.451372f, 2.421759f, -0.077538f, 0.377072f, 0.342435f, -0.577600f, -0.587700f, -0.566600f, // 11
		0.140696f, -2.601934f, -2.558561f, 0.344404f, 0.713562f, 0.000600f, -0.017000f, 0.999900f, // 12
		-0.051152f, 2.399781f, -2.545043f, 0.173441f, 0.897507f, 0.000800f, -0.017100f, 0.999900f, // 13
		2.646786f, -0.139047f, -2.493757f, 0.171340f, 0.711943f, 0.000900f, -0.017100f, 0.999900f, // 14
		2.451372f, 2.421759f, -0.077538f, 0.581301f, 0.571418f, -0.566700f, -0.587400f, 0.577700f, // 15
		2.646786f, -0.139047f, -2.493757f, 0.773163f, 0.570602f, -0.566400f, -0.587700f, 0.577800f, // 16
		-0.051152f, 2.399781f, -2.545043f, 0.675802f, 0.678626f, -0.566700f, -0.587400f, 0.577700f, // 17
		-0.051152f, 2.399781f, -2.545043f, 0.572273f, 0.562165f, 0.578800f, -0.584900f, 0.568200f, // 18
		-2.593003f, -0.085222f, -2.505755f, 0.470722f, 0.670558f, 0.578900f, -0.585000f, 0.568100f, // 19
		-2.521206f, 2.448241f, -0.063388f, 0.378400f, 0.561903f, 0.578800f, -0.584900f, 0.568200f, // 20
		2.646786f, -0.139047f, -2.493757f, 0.472429f, 0.666320f, -0.617600f, 0.587700f, 0.522700f, // 21
		1.328995f, -2.578386f, -1.297838f, 0.471688f, 0.560937f, -0.617600f, 0.587600f, 0.522800f, // 22
		0.140696f, -2.601934f, -2.558561f, 0.570889f, 0.563046f, -0.615700f, 0.588000f, 0.524600f, // 23
		-0.081432f, -2.513204f, 2.426005f, 0.781920f, 0.686045f, -0.001100f, -0.017400f, -0.999800f, // 24
		-0.049649f, 2.440128f, 2.417217f, 0.601339f, 0.882912f, -0.001100f, -0.017400f, -0.999800f, // 25
		-2.534944f, -0.061146f, 2.580280f, 0.601549f, 0.686178f, -0.001100f, -0.017500f, -0.999800f, // 26
		-1.344182f, -2.542571f, 1.294387f, 0.273780f, 0.207902f, -0.004400f, 0.999800f, 0.020000f, // 27
		-1.385028f, -2.549476f, -1.308677f, 0.199544f, 0.116505f, 0.003300f, 0.999700f, 0.022400f, // 28
		1.328995f, -2.578386f, -1.297838f, 0.276444f, 0.043587f, 0.006300f, 0.999900f, 0.013600f, // 29
		-2.632201f, -2.553892f, -0.007226f, 0.558879f, 0.701156f, 0.999200f, -0.035800f, 0.017100f, // 30
		-2.521206f, 2.448241f, -0.063388f, 0.389907f, 0.882836f, 0.999200f, -0.035800f, 0.017100f, // 31
		-2.593003f, -0.085222f, -2.505755f, 0.392408f, 0.699203f, 0.999200f, -0.035900f, 0.017100f, // 32
		-0.049649f, 2.440128f, 2.417217f, 0.974227f, 0.903792f, -0.001200f, -0.999900f, 0.017000f, // 33
		-0.051152f, 2.399781f, -2.545043f, 0.805545f, 0.716573f, -0.001000f, -0.999900f, 0.017000f, // 34
		-2.521206f, 2.448241f, -0.063388f, 0.967339f, 0.716760f, -0.001500f, -0.999900f, 0.016800f, // 35
		2.451372f, 2.421759f, -0.077538f, 0.812554f, 0.905026f, -0.001500f, -0.999900f, 0.017000f, // 36
		2.453980f, -0.063388f, 2.477451f, 0.781664f, 0.883153f, -0.001100f, -0.017400f, -0.999800f, // 37
		-2.534944f, -0.061146f, 2.580280f, 0.557075f, 0.883906f, 0.999200f, -0.036400f, 0.016700f, // 38
		-2.593003f, -0.085222f, -2.505755f, 0.342412f, 0.898790f, 0.000800f, -0.017000f, 0.999900f, // 39
		2.517274f, -2.579095f, -0.002115f, 0.180598f, 0.686298f, -0.999900f, -0.001800f, 0.016900f, // 40
		-1.344182f, -2.542571f, 1.294387f, 0.583286f, 0.333554f, 0.557700f, 0.587400f, -0.586500f, // 41
		-0.081432f, -2.513204f, 2.426005f, 0.774374f, 0.332800f, 0.515900f, 0.587300f, -0.623700f, // 42
		-2.534944f, -0.061146f, 2.580280f, 0.675266f, 0.439161f, 0.558200f, 0.587500f, -0.585900f, // 43
		2.517274f, -2.579095f, -0.002115f, 0.318409f, 0.088124f, 0.017200f, 0.999800f, 0.012800f, // 44
		1.185055f, -2.530499f, 1.198261f, 0.345279f, 0.127685f, -0.002200f, 0.999900f, 0.011900f, // 45
		-0.081432f, -2.513204f, 2.426005f, 0.307557f, 0.174166f, -0.008200f, 0.999900f, 0.011400f, // 46
		-2.632201f, -2.553892f, -0.007226f, 0.240201f, 0.171561f, -0.009200f, 0.999600f, 0.028300f, // 47
		0.140696f, -2.601934f, -2.558561f, 0.237690f, 0.079439f, 0.007800f, 1.000000f, 0.001900f, // 48
		2.453980f, -0.063388f, 2.477451f, 0.279280f, 0.425681f, -0.505400f, 0.601400f, -0.618700f, // 49
		-0.081432f, -2.513204f, 2.426005f, 0.167014f, 0.297576f, -0.535800f, 0.602500f, -0.591600f, // 50
		1.185055f, -2.530499f, 1.198261f, 0.270974f, 0.296491f, -0.505600f, 0.601400f, -0.618600f, // 51
		-1.385028f, -2.549476f, -1.308677f, 0.779095f, 0.324726f, 0.557100f, 0.565000f, 0.608600f, // 52
		-2.632201f, -2.553892f, -0.007226f, 0.973548f, 0.327073f, 0.616800f, 0.564600f, 0.548500f, // 53
		-2.593003f, -0.085222f, -2.505755f, 0.881194f, 0.440353f, 0.557400f, 0.565000f, 0.608400f, // 54
		-2.534944f, -0.061146f, 2.580280f, 0.673123f, 0.440392f, 0.558200f, 0.587500f, -0.585900f, // 55
		-2.632201f, -2.553892f, -0.007226f, 0.585225f, 0.334271f, 0.596900f, 0.586600f, -0.547400f, // 56
		-1.344182f, -2.542571f, 1.294387f, 0.768849f, 0.336714f, 0.557700f, 0.587400f, -0.586500f, // 57
		1.185055f, -2.530499f, 1.198261f, 0.269654f, 0.295390f, -0.505600f, 0.601400f, -0.618600f, // 58
		2.517274f, -2.579095f, -0.002115f, 0.367476f, 0.300643f, -0.476500f, 0.598900f, -0.643600f, // 59
		2.453980f, -0.063388f, 2.477451f, 0.285971f, 0.426336f, -0.505400f, 0.601400f, -0.618700f, // 60
		2.517274f, -2.579095f, -0.002115f, 0.379411f, 0.561682f, -0.619400f, 0.587600f, 0.520600f, // 61
	};
	unsigned int holocron_indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		33, 36, 34,
		24, 37, 25,
		30, 38, 31,
		12, 39, 13,
		3, 40, 4,
		41, 42, 43,
		29, 44, 45,
		45, 46, 27,
		27, 47, 28,
		28, 48, 29,
		29, 45, 27,
		49, 50, 51,
		52, 53, 54,
		55, 56, 57,
		58, 59, 60,
		21, 61, 22,
	};
	// CreateMeshModel recibe cantidad de componentes GLfloat, no de vertices.
	holocronStarWarsCodigo.CreateMeshModel(holocron_vertices, holocron_indices,
		static_cast<unsigned int>(sizeof(holocron_vertices) / sizeof(holocron_vertices[0])),
		static_cast<unsigned int>(sizeof(holocron_indices) / sizeof(holocron_indices[0])));
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CrearHolocronStarWarsCodigo();
	CreateShaders();

	// Vista inicial dirigida al punto medio entre los dos dados.
	camera = Camera(glm::vec3(0.0f, 6.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -112.62f, -13.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado-starwars.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/holocronunwrap.png");
	holocronTexture.LoadTextureA();
	holocronStarWarsCodigoTexture = Texture("Textures/HolocronStarWars.jpg");
	holocronStarWarsCodigoTexture.LoadTextureA();
	
	// Ejercicio 2: cargar el cubo exportado con sus UV y material.
	Dado_M.LoadModel("Models/dadomodelo.obj");
	
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocron_simple.obj");
	HolocronStarWars_M.LoadModel("Models/HolocronStarWars.obj");

	// Importar las tres texturas del avion
	GLint alineacionAntesDelAvion = 4;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alineacionAntesDelAvion);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	AvionConCara_M.LoadModel("Models/avion con cara.obj");
	glPixelStorei(GL_UNPACK_ALIGNMENT, alineacionAntesDelAvion);
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();


		

		//Dado de Opengl
		//Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes
		
		// Ejercicio 2: dado importado, al lado del dado del ejercicio 1.
		// El OBJ mide 2 unidades; escala 0.5 para igualar el cubo de 1 unidad.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.5f, 4.5f, -2.0f));
		// Alinear las seis caras con el dado construido por codigo.
		model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Dado_M.RenderModel();
	



		/*Reporte de práctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		
		*/


		//Holocrones
		color = glm::vec3(0.0f, 1.0f, 0.0f);//color que multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();
		

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

		// Holocron Star Wars adicional: conserva todos los objetos anteriores.
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, -4.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HolocronStarWars_M.RenderModel();

		// Comparacion: misma geometria, UV, textura, orientacion y escala del importado.
		// Solo cambia la posicion: esta copia se muestra debajo del modelo importado.
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.9f, -4.7f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronStarWarsCodigoTexture.UseTexture();
		holocronStarWarsCodigo.RenderMeshModel();

		// Avion con cara y logos: objeto adicional, sin mover la escena anterior.
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 2.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AvionConCara_M.RenderModel();

		


	
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/