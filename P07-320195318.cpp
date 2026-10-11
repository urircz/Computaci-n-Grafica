/*
Práctica 7: Iluminación 1 
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

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;
Texture holocronTexture;
//añadir Texturas del dado y del holocron


Model Blackhawk_M;
//agregado avion
Model avionCuerpo_M, avionCara_M, avionAlas_M;
Model Rover_M; //sustituir por sus propios Rovers Con jerarquía
//Añadir modelo del avión.



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


//función de calculo de normales por promedio de vértices 
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
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, 1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, 1.0f, 0.0f
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
	
	//enviar normales calculadas a la GPU.
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel *obj4 = new MeshModel();
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
	//normales exteriores
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
	//Ejercicio 1, reemplazar con sus dados de 6 caras texturizados, agregar normales
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.48818408f,  0.36003521f,		0.0f,	0.0f,	1.000000f,	//0
		0.5f, -0.5f,  0.5f,		0.67350746f,	0.36003521f,		0.0f,	0.0f,	1.000000f,	//1
		0.5f,  0.5f,  0.5f,		0.67350746f,	0.62235915f,		0.0f,	0.0f,	1.000000f,	//2
		-0.5f,  0.5f,  0.5f,	0.48818408f,	0.62235915f,		0.0f,	0.0f,	1.000000f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.67972637f,  0.36003521f,		1.000000f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.86504975f,	0.36003521f,		1.000000f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.86504975f,	0.62235915f,		1.000000f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.67972637f,	0.62235915f,		1.000000f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.29042289f,  0.36003521f,		0.0f,	0.0f,	-1.000000f,
		0.5f, -0.5f, -0.5f,		0.10509950f,	0.36003521f,		0.0f,	0.0f,	-1.000000f,
		0.5f,  0.5f, -0.5f,		0.10509950f,	0.62235915f,		0.0f,	0.0f,	-1.000000f,
		-0.5f,  0.5f, -0.5f,	0.29042289f,	0.62235915f,		0.0f,	0.0f,	-1.000000f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.29664179f,  0.36003521f,		-1.000000f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.48196517f,	0.36003521f,		-1.000000f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.48196517f,	0.62235915f,		-1.000000f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.29664179f,	0.62235915f,		-1.000000f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.48818408f,  0.35123239f,		0.0f,	-1.000000f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.67350746f,	0.35123239f,		0.0f,	-1.000000f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.67350746f,	0.08890845f,		0.0f,	-1.000000f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.48818408f,	0.08890845f,		0.0f,	-1.000000f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.48818408f,  0.63116197f,		0.0f,	1.000000f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.67350746f,	0.63116197f,		0.0f,	1.000000f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.67350746f,	0.89348592f,		0.0f,	1.000000f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.48818408f,	0.89348592f,		0.0f,	1.000000f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	//normales exteriores unitarias de las caras inclinadas.
	GLfloat holocron_vertices[] = {
		// X, Y, Z, U, V, NX, NY, NZ
		-2.593003f, -0.085222f, -2.505755f, 0.881463f, 0.436897f, -0.557400f, -0.565000f, -0.608400f, // 0
		0.140696f, -2.601934f, -2.558561f, 0.778382f, 0.328399f, -0.495600f, -0.561000f, -0.663100f, // 1
		-1.385028f, -2.549476f, -1.308677f, 0.965973f, 0.330171f, -0.557100f, -0.565000f, -0.608600f, // 2
		2.453980f, -0.063388f, 2.477451f, 0.367060f, 0.684152f, 0.999900f, 0.001200f, -0.016500f, // 3
		2.646786f, -0.139047f, -2.493757f, 0.182588f, 0.485567f, 0.999900f, 0.001900f, -0.017000f, // 4
		2.451372f, 2.421759f, -0.077538f, 0.364115f, 0.482761f, 0.999900f, 0.001900f, -0.016900f, // 5
		-2.521206f, 2.448241f, -0.063388f, 0.822735f, 0.670407f, -0.566700f, 0.587600f, 0.577600f, // 6
		-2.534944f, -0.061146f, 2.580280f, 0.819639f, 0.478581f, -0.566600f, 0.587600f, 0.577600f, // 7
		-0.049649f, 2.440128f, 2.417217f, 0.969525f, 0.577272f, -0.566600f, 0.587700f, 0.577500f, // 8
		-0.049649f, 2.440128f, 2.417217f, 0.562626f, 0.342562f, 0.577600f, 0.587600f, 0.566600f, // 9
		2.453980f, -0.063388f, 2.477451f, 0.470226f, 0.451159f, 0.577600f, 0.587600f, 0.566700f, // 10
		2.451372f, 2.421759f, -0.077538f, 0.377072f, 0.342435f, 0.577600f, 0.587700f, 0.566600f, // 11
		0.140696f, -2.601934f, -2.558561f, 0.344404f, 0.713562f, -0.000600f, 0.017000f, -0.999900f, // 12
		-0.051152f, 2.399781f, -2.545043f, 0.173441f, 0.897507f, -0.000800f, 0.017100f, -0.999900f, // 13
		2.646786f, -0.139047f, -2.493757f, 0.171340f, 0.711943f, -0.000900f, 0.017100f, -0.999900f, // 14
		2.451372f, 2.421759f, -0.077538f, 0.581301f, 0.571418f, 0.566700f, 0.587400f, -0.577700f, // 15
		2.646786f, -0.139047f, -2.493757f, 0.773163f, 0.570602f, 0.566400f, 0.587700f, -0.577800f, // 16
		-0.051152f, 2.399781f, -2.545043f, 0.675802f, 0.678626f, 0.566700f, 0.587400f, -0.577700f, // 17
		-0.051152f, 2.399781f, -2.545043f, 0.572273f, 0.562165f, -0.578800f, 0.584900f, -0.568200f, // 18
		-2.593003f, -0.085222f, -2.505755f, 0.470722f, 0.670558f, -0.578900f, 0.585000f, -0.568100f, // 19
		-2.521206f, 2.448241f, -0.063388f, 0.378400f, 0.561903f, -0.578800f, 0.584900f, -0.568200f, // 20
		2.646786f, -0.139047f, -2.493757f, 0.472429f, 0.666320f, 0.617600f, -0.587700f, -0.522700f, // 21
		1.328995f, -2.578386f, -1.297838f, 0.471688f, 0.560937f, 0.617600f, -0.587600f, -0.522800f, // 22
		0.140696f, -2.601934f, -2.558561f, 0.570889f, 0.563046f, 0.615700f, -0.588000f, -0.524600f, // 23
		-0.081432f, -2.513204f, 2.426005f, 0.781920f, 0.686045f, 0.001100f, 0.017400f, 0.999800f, // 24
		-0.049649f, 2.440128f, 2.417217f, 0.601339f, 0.882912f, 0.001100f, 0.017400f, 0.999800f, // 25
		-2.534944f, -0.061146f, 2.580280f, 0.601549f, 0.686178f, 0.001100f, 0.017500f, 0.999800f, // 26
		-1.344182f, -2.542571f, 1.294387f, 0.273780f, 0.207902f, 0.004400f, -0.999800f, -0.020000f, // 27
		-1.385028f, -2.549476f, -1.308677f, 0.199544f, 0.116505f, -0.003300f, -0.999700f, -0.022400f, // 28
		1.328995f, -2.578386f, -1.297838f, 0.276444f, 0.043587f, -0.006300f, -0.999900f, -0.013600f, // 29
		-2.632201f, -2.553892f, -0.007226f, 0.558879f, 0.701156f, -0.999200f, 0.035800f, -0.017100f, // 30
		-2.521206f, 2.448241f, -0.063388f, 0.389907f, 0.882836f, -0.999200f, 0.035800f, -0.017100f, // 31
		-2.593003f, -0.085222f, -2.505755f, 0.392408f, 0.699203f, -0.999200f, 0.035900f, -0.017100f, // 32
		-0.049649f, 2.440128f, 2.417217f, 0.974227f, 0.903792f, 0.001200f, 0.999900f, -0.017000f, // 33
		-0.051152f, 2.399781f, -2.545043f, 0.805545f, 0.716573f, 0.001000f, 0.999900f, -0.017000f, // 34
		-2.521206f, 2.448241f, -0.063388f, 0.967339f, 0.716760f, 0.001500f, 0.999900f, -0.016800f, // 35
		2.451372f, 2.421759f, -0.077538f, 0.812554f, 0.905026f, 0.001500f, 0.999900f, -0.017000f, // 36
		2.453980f, -0.063388f, 2.477451f, 0.781664f, 0.883153f, 0.001100f, 0.017400f, 0.999800f, // 37
		-2.534944f, -0.061146f, 2.580280f, 0.557075f, 0.883906f, -0.999200f, 0.036400f, -0.016700f, // 38
		-2.593003f, -0.085222f, -2.505755f, 0.342412f, 0.898790f, -0.000800f, 0.017000f, -0.999900f, // 39
		2.517274f, -2.579095f, -0.002115f, 0.180598f, 0.686298f, 0.999900f, 0.001800f, -0.016900f, // 40
		-1.344182f, -2.542571f, 1.294387f, 0.583286f, 0.333554f, -0.557700f, -0.587400f, 0.586500f, // 41
		-0.081432f, -2.513204f, 2.426005f, 0.774374f, 0.332800f, -0.515900f, -0.587300f, 0.623700f, // 42
		-2.534944f, -0.061146f, 2.580280f, 0.675266f, 0.439161f, -0.558200f, -0.587500f, 0.585900f, // 43
		2.517274f, -2.579095f, -0.002115f, 0.318409f, 0.088124f, -0.017200f, -0.999800f, -0.012800f, // 44
		1.185055f, -2.530499f, 1.198261f, 0.345279f, 0.127685f, 0.002200f, -0.999900f, -0.011900f, // 45
		-0.081432f, -2.513204f, 2.426005f, 0.307557f, 0.174166f, 0.008200f, -0.999900f, -0.011400f, // 46
		-2.632201f, -2.553892f, -0.007226f, 0.240201f, 0.171561f, 0.009200f, -0.999600f, -0.028300f, // 47
		0.140696f, -2.601934f, -2.558561f, 0.237690f, 0.079439f, -0.007800f, -1.000000f, -0.001900f, // 48
		2.453980f, -0.063388f, 2.477451f, 0.279280f, 0.425681f, 0.505400f, -0.601400f, 0.618700f, // 49
		-0.081432f, -2.513204f, 2.426005f, 0.167014f, 0.297576f, 0.535800f, -0.602500f, 0.591600f, // 50
		1.185055f, -2.530499f, 1.198261f, 0.270974f, 0.296491f, 0.505600f, -0.601400f, 0.618600f, // 51
		-1.385028f, -2.549476f, -1.308677f, 0.779095f, 0.324726f, -0.557100f, -0.565000f, -0.608600f, // 52
		-2.632201f, -2.553892f, -0.007226f, 0.973548f, 0.327073f, -0.616800f, -0.564600f, -0.548500f, // 53
		-2.593003f, -0.085222f, -2.505755f, 0.881194f, 0.440353f, -0.557400f, -0.565000f, -0.608400f, // 54
		-2.534944f, -0.061146f, 2.580280f, 0.673123f, 0.440392f, -0.558200f, -0.587500f, 0.585900f, // 55
		-2.632201f, -2.553892f, -0.007226f, 0.585225f, 0.334271f, -0.596900f, -0.586600f, 0.547400f, // 56
		-1.344182f, -2.542571f, 1.294387f, 0.768849f, 0.336714f, -0.557700f, -0.587400f, 0.586500f, // 57
		1.185055f, -2.530499f, 1.198261f, 0.269654f, 0.295390f, 0.505600f, -0.601400f, 0.618600f, // 58
		2.517274f, -2.579095f, -0.002115f, 0.367476f, 0.300643f, 0.476500f, -0.598900f, 0.643600f, // 59
		2.453980f, -0.063388f, 2.477451f, 0.285971f, 0.426336f, 0.505400f, -0.601400f, 0.618700f, // 60
		2.517274f, -2.579095f, -0.002115f, 0.379411f, 0.561682f, 0.619400f, -0.587600f, -0.520600f, // 61
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
	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices,
		static_cast<unsigned int>(sizeof(holocron_vertices) / sizeof(holocron_vertices[0])),
		static_cast<unsigned int>(sizeof(holocron_indices) / sizeof(holocron_indices[0])));
	meshListModel.push_back(holocron);
}




// agregado del rover
void CrearMallasRoverJerarquico(MeshModel* mallas)
{
    GLfloat cubo[] = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    };
    unsigned int caras[] = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
    mallas[0].CreateMeshModel(cubo, caras, sizeof(cubo)/sizeof(cubo[0]), sizeof(caras)/sizeof(caras[0]));

    //cilindro de radio 1 y altura 1
    const unsigned int sectores = 18;
    const float piRover = 3.14159265f;
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i <= sectores; ++i)
    {
        float u = static_cast<float>(i) / sectores;
        float x = cos(2.0f * piRover * u), z = sin(2.0f * piRover * u);
        vertices.insert(vertices.end(), {x,-0.5f,z, u,0.0f, x,0.0f,z});
        vertices.insert(vertices.end(), {x, 0.5f,z, u,1.0f, x,0.0f,z});
        if (i < sectores)
        {
            unsigned int a = 2*i, b = a+2;
            indices.insert(indices.end(), {a,a+1,b, b,a+1,b+1});
        }
    }
    for (int signo = -1; signo <= 1; signo += 2)
    {
        unsigned int centro = static_cast<unsigned int>(vertices.size()/8);
        float y = 0.5f * signo;
        vertices.insert(vertices.end(), {0.0f,y,0.0f, 0.5f,0.5f, 0.0f,static_cast<float>(signo),0.0f});
        for (unsigned int i=0; i<=sectores; ++i)
        {
            float angulo=2.0f*piRover*i/sectores;
            float x=cos(angulo), z=sin(angulo);
            vertices.insert(vertices.end(), {x,y,z, 0.5f+0.5f*x,0.5f+0.5f*z, 0.0f,static_cast<float>(signo),0.0f});
            if (i<sectores)
            {
                unsigned int a=centro+1+i, b=a+1;
                if (signo<0) indices.insert(indices.end(), {centro,a,b});
                else indices.insert(indices.end(), {centro,b,a});
            }
        }
    }
    mallas[1].CreateMeshModel(vertices.data(), indices.data(),
        static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(indices.size()));

    // Esfera unitaria para las articulaciones; formato XYZ, UV, normal XYZ.
    vertices.clear(); indices.clear();
    const unsigned int filas=20, columnas=20;
    for (unsigned int fila=0; fila<=filas; ++fila)
    {
        float v=static_cast<float>(fila)/filas;
        float phi=piRover*v;
        for (unsigned int col=0; col<=columnas; ++col)
        {
            float u=static_cast<float>(col)/columnas;
            float theta=2.0f*piRover*u;
            float x=sin(phi)*cos(theta), y=cos(phi), z=sin(phi)*sin(theta);
            vertices.insert(vertices.end(), {x,y,z, u,v, x,y,z});
        }
    }
    for (unsigned int fila=0; fila<filas; ++fila)
        for (unsigned int col=0; col<columnas; ++col)
        {
            unsigned int a=fila*(columnas+1)+col, b=a+columnas+1;
            if (fila>0) indices.insert(indices.end(), {a,a+1,b});
            if (fila<filas-1) indices.insert(indices.end(), {a+1,b+1,b});
        }
    mallas[2].CreateMeshModel(vertices.data(), indices.data(),
        static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(indices.size()));
}
//fin del rover

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

    //carga de los tres grupos originales
    GLint alineacionAvion = 4;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alineacionAvion);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    avionCuerpo_M.LoadModel("Models/avion_p7_cuerpo.obj");
    avionCara_M.LoadModel("Models/avion_p7_cara.obj");
    avionAlas_M.LoadModel("Models/avion_p7_alas.obj");
    glPixelStorei(GL_UNPACK_ALIGNMENT, alineacionAvion);
    //inicializacion rover
    MeshModel mallasRoverJerarquico[3];
    CrearMallasRoverJerarquico(mallasRoverJerarquico);
    GLfloat angulosRover[10] = {};
    glm::vec3 posicionRover(8.0f, -1.0f, -6.0f);
    const glm::vec3 posicionFaroLocal(-5.2f, 4.2f, -4.0f); // Frente, lado del brazo.
    const glm::vec3 direccionFaroLocal = glm::normalize(glm::vec3(-1.0f, -0.8f, 0.0f)); // Hacia delante y al piso cercano.
    double tiempoRoverAnterior = glfwGetTime();
    glm::vec3 posicionAvion(-12.0f, 4.0f, -8.0f);
    GLfloat giroAvion = 0.0f;
    //fin de la inicializacion.


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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
	dadoTexture = Texture("Textures/dado-starwars.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/HolocronStarWars.jpg");
	holocronTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
	Rover_M = Model();
	Rover_M.LoadModel("Models/rovermodificado.obj");
	
	//Añadir modelo del avión
	//Avion_M = Model();
	//Avion_M.LoadModel("Models/");

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
	//declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.2f, 1.2f, // Linterna de camara: ambiental y difusa mas suaves.
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		35.0f); // Se conserva la amplitud de la linterna.
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
    //faro azul frontal del rover jerarquico
    const unsigned int indiceFaroRover = spotLightCount;
    spotLights[indiceFaroRover] = SpotLight(0.0f, 0.0f, 1.0f,
        0.05f, 2.0f,              //componente ambiental y difusa
        0.0f, 0.0f, 0.0f,        //se actualiza desde la matriz del rover
        -1.0f, -0.8f, 0.0f,       //frente local -X, inclinado hacia el piso cercano
        1.0f, 0.05f, 0.01f,      //atenuacion constante, lineal y cuadratica
        30.0f);                  //semiangulo del cono en grados
    spotLightCount++;

    //faro amarillo inferior del avion
    const unsigned int indiceFaroAvion = spotLightCount;
    spotLights[indiceFaroAvion] = SpotLight(1.0f, 1.0f, 0.0f,
        0.02f, 1.4f, //menor intensidad para conservar detalle en el piso
        0.0f, 0.0f, 0.0f,        //posicion actualizada con la matriz padre
        0.0f, -1.0f, 0.0f,       //hacia abajo en coordenadas del avion
        1.0f, 0.09f, 0.032f,     //mayor atenuacion, pierde intensidad al subir
        30.0f);                  //semiangulo del cono
    spotLightCount++;

    //segundo faro del avion: blanco, frontal. Conserva el faro inferior amarillo.
    const unsigned int indiceFaroFrontalAvion = spotLightCount;
    spotLights[indiceFaroFrontalAvion] = SpotLight(1.0f, 1.0f, 1.0f,
        0.05f, 2.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,       
        1.0f, 0.02f, 0.002f,
        35.0f);
    spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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

        //acutsaliza las jerarquias
        double tiempoRover = glfwGetTime();
        GLfloat dtRover = static_cast<GLfloat>(tiempoRover-tiempoRoverAnterior);
        tiempoRoverAnterior = tiempoRover;
        if (dtRover>0.05f) dtRover=0.05f;
        const int teclasRover[10] = {GLFW_KEY_F,GLFW_KEY_G,GLFW_KEY_H,GLFW_KEY_J,
            GLFW_KEY_K,GLFW_KEY_L,GLFW_KEY_U,GLFW_KEY_I,GLFW_KEY_O,GLFW_KEY_P};
        bool* teclas = mainWindow.getsKeys();
        GLfloat sentido = (teclas[GLFW_KEY_LEFT_SHIFT] || teclas[GLFW_KEY_RIGHT_SHIFT]) ? -1.0f : 1.0f;
        for (int i=0;i<10;++i)
            if (teclas[teclasRover[i]])
            {
                GLfloat periodo = (i==3) ? 140.0f : 360.0f;
                angulosRover[i] = fmod(angulosRover[i] + sentido*45.0f*dtRover + periodo, periodo);
            }

        //flecha arriba: avanzar; flecha abajo: retroceder 
        GLfloat avanceRover = (teclas[GLFW_KEY_UP] ? 1.0f : 0.0f)
                           - (teclas[GLFW_KEY_DOWN] ? 1.0f : 0.0f);
        GLfloat distanciaRover = avanceRover * 3.0f * dtRover;
        posicionRover.x -= distanciaRover; // Avanzar hacia el frente -X.
        for (int i=4; i<10; ++i)
            angulosRover[i] = fmod(angulosRover[i]
                + glm::degrees(distanciaRover / (1.25f * 0.45f)) + 360.0f, 360.0f);

        //una misma matriz padre para el cuerpo, las ruedas, el brazo y el faro
        glm::mat4 matrizRover = glm::translate(glm::mat4(1.0f), posicionRover);
        matrizRover = glm::scale(matrizRover, glm::vec3(0.45f));
        glm::vec3 posicionFaroMundo = glm::vec3(matrizRover * glm::vec4(posicionFaroLocal, 1.0f));
        glm::vec3 direccionFaroMundo = glm::normalize(glm::mat3(matrizRover) * direccionFaroLocal);
        spotLights[indiceFaroRover].SetFlash(posicionFaroMundo, direccionFaroMundo);

        //numeros: 4/6 mover en X, 8/2 en Z, 9/3 altura, 7/1 girar
        posicionAvion.x += ((teclas[GLFW_KEY_6] ? 1.0f : 0.0f)
                         - (teclas[GLFW_KEY_4] ? 1.0f : 0.0f)) * 3.0f * dtRover;
        posicionAvion.z += ((teclas[GLFW_KEY_2] ? 1.0f : 0.0f)
                         - (teclas[GLFW_KEY_8] ? 1.0f : 0.0f)) * 3.0f * dtRover;
        posicionAvion.y += ((teclas[GLFW_KEY_9] ? 1.0f : 0.0f)
                         - (teclas[GLFW_KEY_3] ? 1.0f : 0.0f)) * 3.0f * dtRover;
        giroAvion += ((teclas[GLFW_KEY_7] ? 1.0f : 0.0f)
                   - (teclas[GLFW_KEY_1] ? 1.0f : 0.0f)) * 45.0f * dtRover;

        // T: avanzar; Y: retroceder siguiendo la orientacion actual del avion
        glm::mat4 orientacionAvion = glm::rotate(glm::mat4(1.0f),
            glm::radians(giroAvion), glm::vec3(0.0f,1.0f,0.0f));
        glm::vec3 frenteAvion = glm::normalize(glm::mat3(orientacionAvion)
            * glm::vec3(0.0f,0.0f,1.0f));
        GLfloat avanceAvion = (teclas[GLFW_KEY_T] ? 1.0f : 0.0f)
                           - (teclas[GLFW_KEY_Y] ? 1.0f : 0.0f);
        posicionAvion += frenteAvion * (avanceAvion * 3.0f * dtRover);

        //la misma matriz padre dibuja el avion y situa su faro en este fotograma.
        glm::mat4 padreAvion = glm::translate(glm::mat4(1.0f), posicionAvion);
        padreAvion = glm::rotate(padreAvion, glm::radians(giroAvion), glm::vec3(0.0f,1.0f,0.0f));
        padreAvion = glm::scale(padreAvion, glm::vec3(0.45f));
        // Debajo del fuselaje: la cota minima del OBJ es Y=-0.744459.
        glm::vec3 posicionFaroAvion = glm::vec3(padreAvion * glm::vec4(0.0f,-0.8f,0.0f,1.0f));
        glm::vec3 direccionFaroAvion = glm::normalize(glm::mat3(padreAvion) * glm::vec3(0.0f,-1.0f,0.0f));
        spotLights[indiceFaroAvion].SetFlash(posicionFaroAvion, direccionFaroAvion);

        
        glm::vec3 posicionFaroFrontalAvion = glm::vec3(padreAvion
            * glm::vec4(0.0f,-0.1f,2.0f,1.0f));
        glm::vec3 direccionFaroFrontalAvion = glm::normalize(glm::mat3(padreAvion)
            * glm::vec3(0.0f,0.0f,1.0f));
        spotLights[indiceFaroFrontalAvion].SetFlash(
            posicionFaroFrontalAvion, direccionFaroFrontalAvion);

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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
	
		//BLACKHAWK
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Rover: Sustituir por su propio Rover con jerarquía
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();


		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 7.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();


        //inicio del rover agregado: dibujo y controles propios.
        {
            GLint programaRover=0, texturaAnteriorRover=0, unidadAnteriorRover=0;
            GLfloat colorAnteriorRover[3], modeloAnteriorRover[16];
            glGetIntegerv(GL_CURRENT_PROGRAM,&programaRover);
            glGetUniformfv(programaRover,uniformColor,colorAnteriorRover);
            glGetUniformfv(programaRover,uniformModel,modeloAnteriorRover);
            glGetIntegerv(GL_ACTIVE_TEXTURE,&unidadAnteriorRover);
            glActiveTexture(GL_TEXTURE0);
            glGetIntegerv(GL_TEXTURE_BINDING_2D,&texturaAnteriorRover);
            plainTexture.UseTexture();

            //rover adicional a la derecha 
            //su base de ruedas queda sobre el piso 
            glm::mat4 model(1.0f), modelaux(1.0f), modelaux2(1.0f);
            glm::vec3 color(1.0f);
		// Las esferas azules representan las articulaciones del brazo.
		model = matrizRover;
	
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -4.0f)); //NOS POSICIONAMOS EN EL CENTRO DEL OBJETO X AMARILLA 
		modelaux = model; //guardamos la matriz de modelo para que la base se mueva con el objeto
		// Creando la cabina del rover ¿CUÁNTAS UNIDADES MEDIRÁ EN Z?
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));//PARA LLEGAR AL CENTRO DE LA CABINA a PARTIR DEL ORIGEN
		modelaux = model;
		model = glm::scale(model, glm::vec3(8.0f, 4.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.65f, 0.65f, 0.65f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		mallasRoverJerarquico[0].RenderMeshModel(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//mallasRoverJerarquico[2].RenderMeshModel(); //dibuja esfera


		// BASE
		//model = glm::mat4(1.0); //Si dejamos está linea, la base no estará unida al centro del objeto. se debe comentar para que la base se mueva con el objeto.
		/*En su lugar usamos la matriz auxiliar modelaux para que la base se mueva con el objeto.
		Lo que debemos de saber es: de las transformaciones geométricas que se aplican a la cabina y 
		cuales queremos que se apliquen a la base. En este caso, sólo queremos que se aplique la traslación del origen, no la rotación ni el escalado de la cabina. 
		Por lo tanto, debemos de guardar en modelaux sólo la traslación del origen y luego aplicarla a la base.
		*/
		model = modelaux;
		//NOS POSICIONAMOS EN EL CENTRO DE LA BASE
		model = glm::translate(model, glm::vec3(-1.0f, -2.75f, 0.0f));
		modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.45f, 0.45f, 0.45f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		mallasRoverJerarquico[0].RenderMeshModel();
		
		model = modelaux;
		modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		// modelaux2 conserva la base para empezar cada rama.

		//BASE PARA EL BRAZO
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.5f, 1.25f, 2.5f));
		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); NO se requirio escalar el cubo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		mallasRoverJerarquico[0].RenderMeshModel();

		//EJE DE ROTACIÓN
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(angulosRover[0]), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		mallasRoverJerarquico[2].RenderMeshModel();

		//Brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.06066f, 1.06066f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.75f, 0.75f, 0.75f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		mallasRoverJerarquico[1].RenderMeshModel();

		//eje br1-brz2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(angulosRover[1]), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		mallasRoverJerarquico[2].RenderMeshModel();
		
        // BRAZO 2. Partimos del codo, sin el escalado de la esfera.
        model = modelaux;
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(0.35f, 4.0f, 0.35f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.75f, 0.75f, 0.75f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        mallasRoverJerarquico[1].RenderMeshModel();

        // MUNECA. H gira la pinza desde el extremo del segundo brazo.
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angulosRover[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        mallasRoverJerarquico[2].RenderMeshModel();

        // Base de la pinza.
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(0.9f, 0.3f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.4f, 0.4f, 0.4f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        mallasRoverJerarquico[0].RenderMeshModel();

        // J abre y cierra la pinza entre 0 y 35 grados.
        GLfloat apertura = fmod(angulosRover[3], 140.0f);
        if (apertura > 70.0f) apertura = 140.0f - apertura;
        apertura = apertura / 2.0f;
        glm::mat4 modelaux3(1.0f);

        // Un dedo a cada lado. Los dos parten de la base de la pinza.
        for (int lado = -1; lado <= 1; lado += 2)
        {
            model = modelaux;
            model = glm::translate(model, glm::vec3(lado * 0.35f, 0.15f, 0.0f));
            model = glm::rotate(model, glm::radians(-lado * apertura), glm::vec3(0.0f, 0.0f, 1.0f));
            modelaux3 = model;
            model = glm::translate(model, glm::vec3(0.0f, 0.425f, 0.0f));
            model = glm::scale(model, glm::vec3(0.18f, 0.85f, 0.25f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            color = glm::vec3(0.8f, 0.8f, 0.8f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            mallasRoverJerarquico[0].RenderMeshModel();

            // Punta doblada hacia el centro.
            model = modelaux3;
            model = glm::translate(model, glm::vec3(-lado * 0.15f, 0.85f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.18f, 0.25f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            mallasRoverJerarquico[0].RenderMeshModel();
        }

        // seis llantas: tres por lado, cada una con su propio angulo.
        GLfloat posicionX[] = { -7.5f, -0.5f, 6.0f };
        GLfloat inicioX[] = { -3.5f, 2.5f, 2.5f };
        GLfloat soporteZ[] = { 3.8f, 4.6f, 4.6f };
        GLfloat llantaZ[] = { 4.5f, 5.35f, 5.35f };

        for (int lado = -1; lado <= 1; lado += 2)
        {
            // dos anclajes por costado
            for (int anclaje = 0; anclaje < 2; anclaje++)
            {
                model = modelaux2;
                model = glm::translate(model, glm::vec3(inicioX[anclaje], -0.25f,
                    lado * (3.75f + soporteZ[anclaje]) / 2.0f));
                model = glm::scale(model, glm::vec3(0.65f, 0.65f,
                    soporteZ[anclaje] - 3.75f + 0.55f));
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                color = glm::vec3(0.65f, 0.65f, 0.65f);
                glUniform3fv(uniformColor, 1, glm::value_ptr(color));
                mallasRoverJerarquico[0].RenderMeshModel();
            }

            for (int rueda = 0; rueda < 3; rueda++)
            {
                // dos barras paralelas por pata
                GLfloat distanciaX = posicionX[rueda] - inicioX[rueda];
                GLfloat distanciaY = -2.0f - (-0.25f);
                GLfloat longitud = sqrt(distanciaX * distanciaX + distanciaY * distanciaY);
                GLfloat inclinacion = atan2(distanciaY, distanciaX);

                for (int barra = -1; barra <= 1; barra += 2)
                {
                    model = modelaux2;
                    model = glm::translate(model, glm::vec3(
                        (inicioX[rueda] + posicionX[rueda]) / 2.0f, -1.125f,
                        lado * soporteZ[rueda] + barra * 0.14f));
                    model = glm::rotate(model, inclinacion, glm::vec3(0.0f, 0.0f, 1.0f));
                    model = glm::scale(model, glm::vec3(longitud, 0.28f, 0.14f));
                    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                    color = glm::vec3(0.78f, 0.78f, 0.72f);
                    glUniform3fv(uniformColor, 1, glm::value_ptr(color));
                    mallasRoverJerarquico[0].RenderMeshModel();
                }

                // eje entre las barras y el centro de la llanta.
                model = modelaux2;
                model = glm::translate(model, glm::vec3(posicionX[rueda], -2.0f,
                    lado * (soporteZ[rueda] + llantaZ[rueda]) / 2.0f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.22f,
                    llantaZ[rueda] - soporteZ[rueda] + 0.45f, 0.22f));
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                color = glm::vec3(0.45f, 0.45f, 0.45f);
                glUniform3fv(uniformColor, 1, glm::value_ptr(color));
                mallasRoverJerarquico[1].RenderMeshModel();

                GLfloat giro = 0.0f;
                if (rueda == 0) {
                    if (lado == 1) giro = angulosRover[4];  // K
                    else giro = angulosRover[5];          // L
                }
                if (rueda == 1) {
                    if (lado == 1) giro = angulosRover[6];  // U
                    else giro = angulosRover[7];          // I
                }
                if (rueda == 2) {
                    if (lado == 1) giro = angulosRover[8];  // O
                    else giro = angulosRover[9];         // P
                }

                //ninguna rueda hereda el giro de otra.
                model = modelaux2;
                model = glm::translate(model, glm::vec3(posicionX[rueda], -2.0f,
                    lado * llantaZ[rueda]));
                model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 0.0f, 1.0f));
                modelaux3 = model; // Guardar el giro antes de acostar/escalar el cilindro.
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(1.25f, 0.7f, 1.25f));
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                color = glm::vec3(0.04f, 0.04f, 0.04f);
                glUniform3fv(uniformColor, 1, glm::value_ptr(color));
                mallasRoverJerarquico[1].RenderMeshModel();

                // rayos en ambas caras para que se vea el giro individual.
                for (int cara = -1; cara <= 1; cara += 2)
                {
                    for (int rayo = 0; rayo < 4; rayo++)
                    {
                        model = modelaux3;
                        model = glm::translate(model, glm::vec3(0.0f, 0.0f, cara * 0.37f));
                        model = glm::rotate(model, glm::radians(rayo * 45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        model = glm::scale(model, glm::vec3(2.1f, 0.12f, 0.08f));
                        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                        color = glm::vec3(0.7f, 0.7f, 0.7f);
                        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
                        mallasRoverJerarquico[0].RenderMeshModel();
                    }
                }
            }
        }

            glUniform3fv(uniformColor,1,colorAnteriorRover);
            glUniformMatrix4fv(uniformModel,1,GL_FALSE,modeloAnteriorRover);
            glBindTexture(GL_TEXTURE_2D,static_cast<GLuint>(texturaAnteriorRover));
            glActiveTexture(static_cast<GLenum>(unidadAnteriorRover));
        }
        //fin del agregado rover

        //agregado avion
        {
            GLint programaAvion=0, texturaAvionAnterior=0, unidadAvionAnterior=0;
            GLfloat colorAvionAnterior[3], modeloAvionAnterior[16];
            glGetIntegerv(GL_CURRENT_PROGRAM, &programaAvion);
            glGetUniformfv(programaAvion, uniformColor, colorAvionAnterior);
            glGetUniformfv(programaAvion, uniformModel, modeloAvionAnterior);
            glGetIntegerv(GL_ACTIVE_TEXTURE, &unidadAvionAnterior);
            glActiveTexture(GL_TEXTURE0);
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &texturaAvionAnterior);

            glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
            glm::mat4 cuerpoAvion = padreAvion * glm::mat4(1.0f);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(cuerpoAvion));
            avionCuerpo_M.RenderModel();
            glm::mat4 caraAvion = padreAvion * glm::mat4(1.0f);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(caraAvion));
            avionCara_M.RenderModel();
            glm::mat4 alasAvion = padreAvion * glm::mat4(1.0f);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(alasAvion));
            avionAlas_M.RenderModel();

            glUniform3fv(uniformColor, 1, colorAvionAnterior);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, modeloAvionAnterior);
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texturaAvionAnterior));
            glActiveTexture(static_cast<GLenum>(unidadAvionAnterior));
        }
        //fin agregado avion

		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
