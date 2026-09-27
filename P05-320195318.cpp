/*
Práctica 5
*/
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
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//lista de Modelos a importar
//cada parte del Rover se carga por separado
Model Rover_M;                 // cuerpo principal
Model BaseBrazo_M;             // base pegada al cuerpo
Model BrazoSup1_M;
Model BrazoSup2_M;
Model BrazoSup3_M;

Model PataDer1_M;
Model PataDer2_M;
Model PataIzq1_M;
Model PataIzq2_M;

Model RuedaDer1_M;
Model RuedaDer2_M;
Model RuedaDer3_M;
Model RuedaIzq1_M;
Model RuedaIzq2_M;
Model RuedaIzq3_M;

//holocron, cuerpo padre y las 8 esquinas importadas por separado
Model CuerpoHolocron_M;
Model TrianguloSup1_M;
Model TrianguloSup2_M;
Model TrianguloSup3_M;
Model TrianguloSup4_M;
Model TrianguloInf1_M;
Model TrianguloInf2_M;
Model TrianguloInf3_M;
Model TrianguloInf4_M;

//satelite, archivos separados
Model CuerpoSatelite_M;
Model AlaDer1_M;
Model AlaDer2_M;
Model AlaIzq1_M;
Model AlaIzq2_M;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


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

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); 
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	//vista inicial 
	camera = Camera(glm::vec3(32.0f, 26.0f, 62.0f), glm::vec3(0.0f, 1.0f, 0.0f), -117.0f, -16.0f, 18.0f, 0.3f);
	//carga los modelos por separado
	Rover_M = Model();
	Rover_M.LoadModel("Models/rovermodificado.fbx");

	BaseBrazo_M = Model();
	BaseBrazo_M.LoadModel("Models/basepegadacuerpo.fbx");

	BrazoSup1_M = Model();
	BrazoSup1_M.LoadModel("Models/brazosup1.fbx");

	BrazoSup2_M = Model();
	BrazoSup2_M.LoadModel("Models/brazosup2.fbx");

	BrazoSup3_M = Model();
	BrazoSup3_M.LoadModel("Models/brazosup3.fbx");

	PataDer1_M = Model();
	PataDer1_M.LoadModel("Models/patader1.fbx");

	PataDer2_M = Model();
	PataDer2_M.LoadModel("Models/patader2.fbx");

	PataIzq1_M = Model();
	PataIzq1_M.LoadModel("Models/pataizq1.fbx");

	PataIzq2_M = Model();
	PataIzq2_M.LoadModel("Models/pataizq2.fbx");

	RuedaDer1_M = Model();
	RuedaDer1_M.LoadModel("Models/ruedader1.fbx");

	RuedaDer2_M = Model();
	RuedaDer2_M.LoadModel("Models/ruedader2.fbx");

	RuedaDer3_M = Model();
	RuedaDer3_M.LoadModel("Models/ruedader3.fbx");

	RuedaIzq1_M = Model();
	RuedaIzq1_M.LoadModel("Models/ruedaizq1.fbx");

	RuedaIzq2_M = Model();
	RuedaIzq2_M.LoadModel("Models/ruedaizq2.fbx");

	RuedaIzq3_M = Model();
	RuedaIzq3_M.LoadModel("Models/ruedaizq3.fbx");

	//carga las nueve piezas del Holocron
	CuerpoHolocron_M.LoadModel("Models/Holocron/cuerpoholocron.fbx");
	TrianguloSup1_M.LoadModel("Models/Holocron/triangulosup1.fbx");
	TrianguloSup2_M.LoadModel("Models/Holocron/triangulosup2.fbx");
	TrianguloSup3_M.LoadModel("Models/Holocron/triangulosup3.fbx");
	TrianguloSup4_M.LoadModel("Models/Holocron/triangulosup4.fbx");
	TrianguloInf1_M.LoadModel("Models/Holocron/trianguloinf1.fbx");
	TrianguloInf2_M.LoadModel("Models/Holocron/trianguloinf2.fbx");
	TrianguloInf3_M.LoadModel("Models/Holocron/trianguloinf3.fbx");
	TrianguloInf4_M.LoadModel("Models/Holocron/trianguloinf4.fbx");

	//carga el cuerpo y las cuatro alas del satelite
	CuerpoSatelite_M.LoadModel("Models/Satelite/cuerposatelite.fbx");
	AlaDer1_M.LoadModel("Models/Satelite/alader1.fbx");
	AlaDer2_M.LoadModel("Models/Satelite/alader2.fbx");
	AlaIzq1_M.LoadModel("Models/Satelite/alaizq1.fbx");
	AlaIzq2_M.LoadModel("Models/Satelite/alaizq2.fbx");

	//crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	//angulos independientes de las cuatro patas
	//cada pata se limita entre -45 y 45 grados
	GLfloat giroPataDer1 = 0.0f;
	GLfloat giroPataDer2 = 0.0f;
	GLfloat giroPataIzq1 = 0.0f;
	GLfloat giroPataIzq2 = 0.0f;

	//cada rueda tiene su propio angulo de rotacion alrededor de su eje
	GLfloat giroRuedaDer1 = 0.0f;
	GLfloat giroRuedaDer2 = 0.0f;
	GLfloat giroRuedaDer3 = 0.0f;
	GLfloat giroRuedaIzq1 = 0.0f;
	GLfloat giroRuedaIzq2 = 0.0f;
	GLfloat giroRuedaIzq3 = 0.0f;

	//angulos propios de la base y las tres articulaciones del brazo.
	GLfloat giroBaseBrazo = 0.0f;
	GLfloat giroBrazo1 = 0.0f;
	GLfloat giroBrazo2 = 0.0f;
	GLfloat giroBrazo3 = 0.0f;

	//inicia el reloj despues de cargar los recursos
	//Un angulo independiente por esquina; 360 grados completan el ciclo.
	GLfloat giroHolocronSup1 = 0.0f;
	GLfloat giroHolocronSup2 = 0.0f;
	GLfloat giroHolocronSup3 = 0.0f;
	GLfloat giroHolocronSup4 = 0.0f;
	GLfloat giroHolocronInf1 = 0.0f;
	GLfloat giroHolocronInf2 = 0.0f;
	GLfloat giroHolocronInf3 = 0.0f;
	GLfloat giroHolocronInf4 = 0.0f;

	//traslacion independiente del satelite y angulos locales de sus cuatro alas
	glm::vec3 posicionSatelite = glm::vec3(18.0f, 7.0f, -1.5f);
	GLfloat giroAlaDer1 = 0.0f, giroAlaDer2 = 0.0f;
	GLfloat giroAlaIzq1 = 0.0f, giroAlaIzq2 = 0.0f;

	lastTime = glfwGetTime();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibe eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Rotacion independiente de las patas.
		GLfloat velocidadPata = 30.0f * deltaTime;

		//pata derecha 1: K abre hacia fuera / L cierra hacia dentro
		if (mainWindow.getsKeys()[GLFW_KEY_K]) giroPataDer1 += velocidadPata;
		if (mainWindow.getsKeys()[GLFW_KEY_L]) giroPataDer1 -= velocidadPata;

		//pata derecha 2: U / I
		if (mainWindow.getsKeys()[GLFW_KEY_U]) giroPataDer2 += velocidadPata;
		if (mainWindow.getsKeys()[GLFW_KEY_I]) giroPataDer2 -= velocidadPata;

		//pata izquierda 1: O abre hacia fuera / P cierra hacia dentro
		if (mainWindow.getsKeys()[GLFW_KEY_O]) giroPataIzq1 += velocidadPata;
		if (mainWindow.getsKeys()[GLFW_KEY_P]) giroPataIzq1 -= velocidadPata;

		//pata izquierda 2: B / N
		if (mainWindow.getsKeys()[GLFW_KEY_B]) giroPataIzq2 += velocidadPata;
		if (mainWindow.getsKeys()[GLFW_KEY_N]) giroPataIzq2 -= velocidadPata;

		//limite de +/-45 grados para las cuatro patas.
		giroPataDer1 = glm::clamp(giroPataDer1, -45.0f, 45.0f);
		giroPataDer2 = glm::clamp(giroPataDer2, -45.0f, 45.0f);
		giroPataIzq1 = glm::clamp(giroPataIzq1, -45.0f, 45.0f);
		giroPataIzq2 = glm::clamp(giroPataIzq2, -45.0f, 45.0f);

		//rotacion independiente de las ruedas: teclas 1,2,3 derechas; 4,5,6 izquierdas.
		//mantener Shift con el numero invierte el sentido de esa rueda.
		GLfloat velocidadRueda = 90.0f * deltaTime;
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT_SHIFT] || mainWindow.getsKeys()[GLFW_KEY_RIGHT_SHIFT])
			velocidadRueda = -velocidadRueda;

		if (mainWindow.getsKeys()[GLFW_KEY_1]) giroRuedaDer1 += velocidadRueda;
		if (mainWindow.getsKeys()[GLFW_KEY_2]) giroRuedaDer2 += velocidadRueda;
		if (mainWindow.getsKeys()[GLFW_KEY_3]) giroRuedaDer3 += velocidadRueda;
		if (mainWindow.getsKeys()[GLFW_KEY_4]) giroRuedaIzq1 += velocidadRueda;
		if (mainWindow.getsKeys()[GLFW_KEY_5]) giroRuedaIzq2 += velocidadRueda;
		if (mainWindow.getsKeys()[GLFW_KEY_6]) giroRuedaIzq3 += velocidadRueda;

		//las ruedas pueden dar vueltas completas; solo las patas se limitan a 45 grados.
		giroRuedaDer1 = std::fmod(giroRuedaDer1, 360.0f);
		giroRuedaDer2 = std::fmod(giroRuedaDer2, 360.0f);
		giroRuedaDer3 = std::fmod(giroRuedaDer3, 360.0f);
		giroRuedaIzq1 = std::fmod(giroRuedaIzq1, 360.0f);
		giroRuedaIzq2 = std::fmod(giroRuedaIzq2, 360.0f);
		giroRuedaIzq3 = std::fmod(giroRuedaIzq3, 360.0f);

		//brazo: F base, G primer segmento, H segundo, J tercero.
		//Shift junto con la tecla invierte el sentido de la articulacion.
		GLfloat velocidadBrazo = 30.0f * deltaTime;
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT_SHIFT] || mainWindow.getsKeys()[GLFW_KEY_RIGHT_SHIFT])
			velocidadBrazo = -velocidadBrazo;

		if (mainWindow.getsKeys()[GLFW_KEY_F]) giroBaseBrazo += velocidadBrazo;
		if (mainWindow.getsKeys()[GLFW_KEY_G]) giroBrazo1 += velocidadBrazo;
		if (mainWindow.getsKeys()[GLFW_KEY_H]) giroBrazo2 += velocidadBrazo;
		if (mainWindow.getsKeys()[GLFW_KEY_J]) giroBrazo3 += velocidadBrazo;

		//la base gira sobre Y; los segmentos flexionan +/-90 grados desde el reposo.
		giroBaseBrazo = std::fmod(giroBaseBrazo, 360.0f);
		giroBrazo1 = glm::clamp(giroBrazo1, -90.0f, 90.0f);
		giroBrazo2 = glm::clamp(giroBrazo2, -90.0f, 90.0f);
		giroBrazo3 = glm::clamp(giroBrazo3, -90.0f, 90.0f);

		//holocron: F1-F4 superiores, F5-F8 inferiores; shift invierte el sentido
		//F9 mueve todas las esquinas; F10 devuelve el Holocron a su forma cerrada
		GLfloat velocidadHolocron = 60.0f * deltaTime;
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT_SHIFT] || mainWindow.getsKeys()[GLFW_KEY_RIGHT_SHIFT])
			velocidadHolocron = -velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F1] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronSup1 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F2] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronSup2 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F3] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronSup3 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F4] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronSup4 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F5] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronInf1 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F6] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronInf2 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F7] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronInf3 += velocidadHolocron;
		if (mainWindow.getsKeys()[GLFW_KEY_F8] || mainWindow.getsKeys()[GLFW_KEY_F9]) giroHolocronInf4 += velocidadHolocron;
		giroHolocronSup1 = std::fmod(giroHolocronSup1, 360.0f);
		giroHolocronSup2 = std::fmod(giroHolocronSup2, 360.0f);
		giroHolocronSup3 = std::fmod(giroHolocronSup3, 360.0f);
		giroHolocronSup4 = std::fmod(giroHolocronSup4, 360.0f);
		giroHolocronInf1 = std::fmod(giroHolocronInf1, 360.0f);
		giroHolocronInf2 = std::fmod(giroHolocronInf2, 360.0f);
		giroHolocronInf3 = std::fmod(giroHolocronInf3, 360.0f);
		giroHolocronInf4 = std::fmod(giroHolocronInf4, 360.0f);
		if (mainWindow.getsKeys()[GLFW_KEY_F10])
		{
			giroHolocronSup1 = 0.0f;
			giroHolocronSup2 = 0.0f;
			giroHolocronSup3 = 0.0f;
			giroHolocronSup4 = 0.0f;
			giroHolocronInf1 = 0.0f;
			giroHolocronInf2 = 0.0f;
			giroHolocronInf3 = 0.0f;
			giroHolocronInf4 = 0.0f;
		}

		//satelite: izquierda/derecha = X; RePag/AvPag = Y; arriba/abajo = Z
		GLfloat velocidadSatelite = 5.0f * deltaTime;
		if (mainWindow.getsKeys()[GLFW_KEY_RIGHT]) posicionSatelite.x += velocidadSatelite;
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT]) posicionSatelite.x -= velocidadSatelite;
		if (mainWindow.getsKeys()[GLFW_KEY_PAGE_UP]) posicionSatelite.y += velocidadSatelite;
		if (mainWindow.getsKeys()[GLFW_KEY_PAGE_DOWN]) posicionSatelite.y -= velocidadSatelite;
		if (mainWindow.getsKeys()[GLFW_KEY_UP]) posicionSatelite.z -= velocidadSatelite;
		if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) posicionSatelite.z += velocidadSatelite;

		//alas: 7 derecha1, 8 derecha2, 9 izquierda1, 0 izquierda2.
		//shift invierte el giro, las alas flexionan +/-90 grados 
		GLfloat velocidadAla = 30.0f * deltaTime;
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT_SHIFT] || mainWindow.getsKeys()[GLFW_KEY_RIGHT_SHIFT])
			velocidadAla = -velocidadAla;
		if (mainWindow.getsKeys()[GLFW_KEY_7]) giroAlaDer1 += velocidadAla;
		if (mainWindow.getsKeys()[GLFW_KEY_8]) giroAlaDer2 += velocidadAla;
		if (mainWindow.getsKeys()[GLFW_KEY_9]) giroAlaIzq1 += velocidadAla;
		if (mainWindow.getsKeys()[GLFW_KEY_0]) giroAlaIzq2 += velocidadAla;
		giroAlaDer1 = glm::clamp(giroAlaDer1, -90.0f, 90.0f);
		giroAlaDer2 = glm::clamp(giroAlaDer2, -90.0f, 90.0f);
		giroAlaIzq1 = glm::clamp(giroAlaIzq1, -90.0f, 90.0f);
		giroAlaIzq2 = glm::clamp(giroAlaIzq2, -90.0f, 90.0f);
		//Inicio restablece solo el satelite.
		if (mainWindow.getsKeys()[GLFW_KEY_HOME])
		{
			posicionSatelite = glm::vec3(18.0f, 7.0f, -1.5f);
			giroAlaDer1 = giroAlaDer2 = giroAlaIzq1 = giroAlaIzq2 = 0.0f;
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//inicia el dibujo de los demas objetos

		//transformacion general del Rover dentro de la escena.
		glm::mat4 roverPadre = glm::mat4(1.0f);
		roverPadre = glm::translate(roverPadre, glm::vec3(0.0f, -2.0f, -1.5f));

		//cuerpo
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		model = roverPadre;
		//rotacion que trae el cuerpo desde Blender al exportarlo en FBX.
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();

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
		BaseBrazo_M.RenderModel();

		//brazo 1: hereda la base y agrega su giro en el pivote compartido.
		modelaux = glm::rotate(modelaux, glm::radians(giroBrazo1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(modelaux, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoSup1_M.RenderModel();

		//brazo 2: avanzar desde el pivote del brazo 1 hasta el suyo.
		modelaux = glm::translate(modelaux, pivoteBrazo2 - pivoteBrazo1);
		modelaux = glm::rotate(modelaux, glm::radians(giroBrazo2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(modelaux, glm::radians(-29.6691909f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoSup2_M.RenderModel();

		//brazo 3: hereda el brazo 2 y agrega su desplazamiento relativo.
		modelaux = glm::translate(modelaux, pivoteBrazo3 - pivoteBrazo2);
		modelaux = glm::rotate(modelaux, glm::radians(giroBrazo3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(modelaux, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoSup3_M.RenderModel();

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
		PataDer1_M.RenderModel();

		//pata derecha 2
		model = pataPadreDer2;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDer2_M.RenderModel();

		//pata izquierda 1
		model = pataPadreIzq1;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataIzq1_M.RenderModel();

		//pata izquierda 2
		model = pataPadreIzq2;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataIzq2_M.RenderModel();

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
		RuedaDer1_M.RenderModel();

		//ruedas media y trasera derechas: hijas de PataDer2.
		model = pataPadreDer2;
		model = glm::translate(model, posRuedaDer2 - pivotePataDer2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaDer2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaDer2_M.RenderModel();

		model = pataPadreDer2;
		model = glm::translate(model, posRuedaDer3 - pivotePataDer2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaDer3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaDer3_M.RenderModel();

		//rueda delantera izquierda: hija de PataIzq1.
		model = pataPadreIzq1;
		model = glm::translate(model, posRuedaIzq1 - pivotePataIzq1);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaIzq1), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaIzq1_M.RenderModel();

		//ruedas media y trasera izquierdas: hijas de PataIzq2.
		model = pataPadreIzq2;
		model = glm::translate(model, posRuedaIzq2 - pivotePataIzq2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaIzq2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaIzq2_M.RenderModel();

		model = pataPadreIzq2;
		model = glm::translate(model, posRuedaIzq3 - pivotePataIzq2);
		//girar en su propio centro, heredando antes el movimiento de la pata.
		model = glm::rotate(model, glm::radians(giroRuedaIzq3), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RuedaIzq3_M.RenderModel();

		//holocron, padre comun situado al lado del Rover
		//las esquinas son hijas del centro, no de una bisagra en su propio borde
		glm::mat4 holocronPadre = glm::mat4(1.0f);
		holocronPadre = glm::translate(holocronPadre, glm::vec3(-18.0f, 6.0f, -1.5f));
		holocronPadre = glm::scale(holocronPadre, glm::vec3(0.8f));

		color = glm::vec3(0.05f, 0.30f, 0.50f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(holocronPadre, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoHolocron_M.RenderModel();

		//origen comun de los ocho fbx respecto al cuerpo
		glm::vec3 origenEsquinas = glm::vec3(0.00407529f, 0.04012322f, 0.01084948f);
		color = glm::vec3(0.85f, 0.60f, 0.15f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//el giro propio conserva la trayectoria del centro y completa una vuelta por ciclo
		GLfloat aperturaHolocron = 0.0f;

		//triangulosup1: F1; solo esta esquina utiliza giroHolocronSup1
		aperturaHolocron = std::sin(glm::radians(giroHolocronSup1) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(giroHolocronSup1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroSup1 = glm::vec3(3.81922685f, 3.73523595f, 3.86784493f);
		model = glm::translate(model, centroSup1);
		model = glm::rotate(model, glm::radians(giroHolocronSup1), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
		model = glm::translate(model, -centroSup1);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloSup1_M.RenderModel();

		//triangulosup2: F2; solo esta esquina utiliza giroHolocronSup2
		aperturaHolocron = std::sin(glm::radians(giroHolocronSup2) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(giroHolocronSup2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroSup2 = glm::vec3(3.83961686f, 3.87063676f, -3.74258596f);
		model = glm::translate(model, centroSup2);
		model = glm::rotate(model, glm::radians(giroHolocronSup2), glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));
		model = glm::translate(model, -centroSup2);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloSup2_M.RenderModel();

		//triangulosup3: F3; solo esta esquina utiliza giroHolocronSup3
		aperturaHolocron = std::sin(glm::radians(giroHolocronSup3) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(giroHolocronSup3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroSup3 = glm::vec3(-3.82454684f, 3.86211286f, -3.74578433f);
		model = glm::translate(model, centroSup3);
		model = glm::rotate(model, glm::radians(giroHolocronSup3), glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)));
		model = glm::translate(model, -centroSup3);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloSup3_M.RenderModel();

		//triangulosup4: F4; solo esta esquina utiliza giroHolocronSup4
		aperturaHolocron = std::sin(glm::radians(giroHolocronSup4) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(giroHolocronSup4), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroSup4 = glm::vec3(-3.80661469f, 3.72984831f, 3.88569730f);
		model = glm::translate(model, centroSup4);
		model = glm::rotate(model, glm::radians(giroHolocronSup4), glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)));
		model = glm::translate(model, -centroSup4);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloSup4_M.RenderModel();

		//trianguloinf1: F5; solo esta esquina utiliza giroHolocronInf1
		aperturaHolocron = std::sin(glm::radians(giroHolocronInf1) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(-giroHolocronInf1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroInf1 = glm::vec3(3.83311521f, -3.92492276f, 3.72036998f);
		model = glm::translate(model, centroInf1);
		model = glm::rotate(model, glm::radians(-giroHolocronInf1), glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)));
		model = glm::translate(model, -centroInf1);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloInf1_M.RenderModel();

		//trianguloinf2: F6; solo esta esquina utiliza giroHolocronInf2
		aperturaHolocron = std::sin(glm::radians(giroHolocronInf2) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(-giroHolocronInf2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroInf2 = glm::vec3(3.83854344f, -3.81510897f, -3.86601067f);
		model = glm::translate(model, centroInf2);
		model = glm::rotate(model, glm::radians(-giroHolocronInf2), glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)));
		model = glm::translate(model, -centroInf2);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloInf2_M.RenderModel();

		//trianguloinf3: F7; solo esta esquina utiliza giroHolocronInf3.
		aperturaHolocron = std::sin(glm::radians(giroHolocronInf3) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(-giroHolocronInf3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina 
		glm::vec3 centroInf3 = glm::vec3(-3.81797262f, -3.83467732f, -3.86930024f);
		model = glm::translate(model, centroInf3);
		model = glm::rotate(model, glm::radians(-giroHolocronInf3), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
		model = glm::translate(model, -centroInf3);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloInf3_M.RenderModel();

		//trianguloinf4: F8; solo esta esquina utiliza giroHolocronInf4
		aperturaHolocron = std::sin(glm::radians(giroHolocronInf4) * 0.5f);
		aperturaHolocron = 3.0f * aperturaHolocron * aperturaHolocron;
		model = glm::rotate(holocronPadre, glm::radians(-giroHolocronInf4), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)) * aperturaHolocron);
		model = glm::translate(model, origenEsquinas);
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//giro propio alrededor del centro geometrico de esta esquina
		glm::vec3 centroInf4 = glm::vec3(-3.81317352f, -3.92629607f, 3.76023213f);
		model = glm::translate(model, centroInf4);
		model = glm::rotate(model, glm::radians(-giroHolocronInf4), glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)));
		model = glm::translate(model, -centroInf4);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TrianguloInf4_M.RenderModel();

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

		//jerarquia, cuerpo -> derecha1 -> derecha2; cuerpo -> izquierda1 -> izquierda2
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

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
