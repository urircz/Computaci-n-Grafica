//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"



//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* vShaderMagenta = "shaders/shadermagenta.vert";
static const char* vShaderAmarillo =
"shaders/shaderamarillo.vert";

static const char* vShaderNegro =
"shaders/shadernegro.vert";


float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

const int PIRAMIDE = 0;
const int CUBO = 1;

const int ROJO = 2;
const int VERDE = 3;
const int AZUL = 4;
const int CAFE = 5;
const int MAGENTA = 6;
const int AMARILLO = 7;
const int NEGRO = 8;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	
	//Triangulo azul
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		0.0f,	0.0f,	1.0f,
		 1.0f,		-1.0f,		0.5f,		0.0f,	0.0f,	1.0f,
		 0.0f,		 1.0f,		0.5f,		0.0f,	0.0f,	1.0f
	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);


	//Triangulo verde
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		0.0f,	0.5f,	0.0f,
		 1.0f,		-1.0f,		0.5f,		0.0f,	0.5f,	0.0f,
		 0.0f,		 1.0f,		0.5f,		0.0f,	0.5f,	0.0f
	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);


	//Cuadrado rojo
	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,		-0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		 0.5f,		-0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		 0.5f,		 0.5f,		0.5f,		1.0f,	0.0f,	0.0f,

		-0.5f,		-0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		 0.5f,		 0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		-0.5f,		 0.5f,		0.5f,		1.0f,	0.0f,	0.0f
	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);


	//Cuadrado verde
	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,		-0.5f,		0.5f,		0.0f,	1.0f,	0.0f,
		 0.5f,		-0.5f,		0.5f,		0.0f,	1.0f,	0.0f,
		 0.5f,		 0.5f,		0.5f,		0.0f,	1.0f,	0.0f,

		-0.5f,		-0.5f,		0.5f,		0.0f,	1.0f,	0.0f,
		 0.5f,		 0.5f,		0.5f,		0.0f,	1.0f,	0.0f,
		-0.5f,		 0.5f,		0.5f,		0.0f,	1.0f,	0.0f
	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


	//Cuadrado cafe
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R			G			B
		-0.5f,		-0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		 0.5f,		-0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		 0.5f,		 0.5f,		0.5f,		0.478f,		0.255f,		0.067f,

		-0.5f,		-0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		 0.5f,		 0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		-0.5f,		 0.5f,		0.5f,		0.478f,		0.255f,		0.067f
	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);
	

	//Triangulo rojo
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		1.0f,	0.0f,	0.0f,
		 1.0f,		-1.0f,		0.5f,		1.0f,	0.0f,	0.0f,
		 0.0f,		 1.0f,		0.5f,		1.0f,	0.0f,	0.0f
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);


	//Triangulo amarillo
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		1.0f,	1.0f,	0.0f,
		 1.0f,		-1.0f,		0.5f,		1.0f,	1.0f,	0.0f,
		 0.0f,		 1.0f,		0.5f,		1.0f,	1.0f,	0.0f
	};

	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);


	//Triangulo morado
	GLfloat vertices_triangulomorado[] = {
		//X			Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		0.5f,	0.0f,	1.0f,
		 1.0f,		-1.0f,		0.5f,		0.5f,	0.0f,	1.0f,
		 0.0f,		 1.0f,		0.5f,		0.5f,	0.0f,	1.0f
	};

	MeshColor* triangulomorado = new MeshColor();
	triangulomorado->CreateMeshColor(vertices_triangulomorado, 18);
	meshColorList.push_back(triangulomorado);


	//Cuadrado azul
	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,		-0.5f,		0.5f,		0.0f,	0.0f,	1.0f,
		 0.5f,		-0.5f,		0.5f,		0.0f,	0.0f,	1.0f,
		 0.5f,		 0.5f,		0.5f,		0.0f,	0.0f,	1.0f,

		-0.5f,		-0.5f,		0.5f,		0.0f,	0.0f,	1.0f,
		 0.5f,		 0.5f,		0.5f,		0.0f,	0.0f,	1.0f,
		-0.5f,		 0.5f,		0.5f,		0.0f,	0.0f,	1.0f
	};

	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);


	//Cuadrado negro
	GLfloat vertices_cuadradonegro[] = {
		//X			Y			Z			R		G		B
		-0.5f,		-0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		 0.5f,		-0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		 0.5f,		 0.5f,		0.5f,		0.0f,	0.0f,	0.0f,

		-0.5f,		-0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		 0.5f,		 0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		-0.5f,		 0.5f,		0.5f,		0.0f,	0.0f,	0.0f
	};

	MeshColor* cuadradonegro = new MeshColor();
	cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
	meshColorList.push_back(cuadradonegro);
}


void CreateShaders()
{
	//Shader original para Mesh
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	//Shader para MeshColor
	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);


	//Shader rojo
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShaderColor);
	shaderList.push_back(*shaderRojo);


	//Shader verde
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShaderVerde, fShaderColor);
	shaderList.push_back(*shaderVerde);


	//Shader azul
	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShaderAzul, fShaderColor);
	shaderList.push_back(*shaderAzul);


	//Shader cafe
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShaderCafe, fShaderColor);
	shaderList.push_back(*shaderCafe);


	//Shader magenta
	Shader* shaderMagenta = new Shader();
	shaderMagenta->CreateFromFiles(vShaderMagenta, fShaderColor);
	shaderList.push_back(*shaderMagenta);

	//Shader amarillo
	Shader* shaderAmarillo = new Shader();
	shaderAmarillo->CreateFromFiles(
		vShaderAmarillo,
		fShaderColor
	);
	shaderList.push_back(*shaderAmarillo);


	//Shader negro
	Shader* shaderNegro = new Shader();
	shaderNegro->CreateFromFiles(
		vShaderNegro,
		fShaderColor
	);
	shaderList.push_back(*shaderNegro);
}


void DibujarObjeto(
	int objeto,
	int shaderIndice,
	float x, float y, float z,
	float escalaX, float escalaY, float escalaZ,
	float rotX, float rotY, float rotZ,
	glm::mat4 projection)
{
	//Usar shader correspondiente
	shaderList[shaderIndice].useShader();

	GLuint uniformModel =
		shaderList[shaderIndice].getModelLocation();

	GLuint uniformProjection =
		shaderList[shaderIndice].getProjectLocation();


	//Matriz modelo
	glm::mat4 model = glm::mat4(1.0f);

	//Posicion
	model = glm::translate(
		model,
		glm::vec3(x, y, z)
	);

	//Rotacion en X
	model = glm::rotate(
		model,
		glm::radians(rotX),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);

	//Rotacion en Y
	model = glm::rotate(
		model,
		glm::radians(rotY),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	//Rotacion en Z
	model = glm::rotate(
		model,
		glm::radians(rotZ),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	//Escala
	model = glm::scale(
		model,
		glm::vec3(
			escalaX,
			escalaY,
			escalaZ
		)
	);


	glUniformMatrix4fv(
		uniformModel,
		1,
		GL_FALSE,
		glm::value_ptr(model)
	);

	glUniformMatrix4fv(
		uniformProjection,
		1,
		GL_FALSE,
		glm::value_ptr(projection)
	);

	meshList[objeto]->RenderMesh();
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	glEnable(GL_DEPTH_TEST);
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(
		-1.35f, 1.35f,
		-1.0f, 1.0f,
		0.1f, 100.0f
	);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Shader para las figuras con color
		shaderList[1].useShader();

		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(
			uniformProjection,
			1,
			GL_FALSE,
			glm::value_ptr(projection)
		);

		//Base negra
		DibujarObjeto(
			CUBO, NEGRO,
			0.0f, -0.78f, -4.30f,

			2.65f, 0.08f, 0.18f,

			-10.0f, 10.0f, 0.0f,

			projection
		);


		//Figura izquierda

		//Barra cafe izquierda
		DibujarObjeto(
			CUBO, CAFE,
			-1.10f, -0.19f, -4.05f,

			0.08f, 1.05f, 0.22f,

			-10.0f, 18.0f, 0.0f,

			projection
		);


		//Barra cafe derecha
		DibujarObjeto(
			CUBO, CAFE,
			-0.70f, -0.19f, -4.05f,

			0.08f, 1.05f, 0.22f,

			-10.0f, 18.0f, 0.0f,

			projection
		);


		//Piramide amarilla
		DibujarObjeto(
			PIRAMIDE, AMARILLO,
			-0.90f, 0.015f, -3.80f,

			0.35f, 0.29f, 0.45f,

			-12.0f, 16.0f, 180.0f,

			projection
		);


		//Piramide roja
		DibujarObjeto(
			PIRAMIDE, ROJO,
			-0.90f, -0.279f, -3.80f,

			0.35f, 0.29f, 0.45f,

			-12.0f, 16.0f, 180.0f,

			projection
		);


		//Piramide verde
		DibujarObjeto(
			PIRAMIDE, VERDE,
			-0.90f, -0.573f, -3.80f,

			0.35f, 0.29f, 0.45f,

			-12.0f, 16.0f, 180.0f,

			projection
		);



		//Figura central

		//Piramide amarilla superior izquierda
		DibujarObjeto(
			PIRAMIDE, AMARILLO,
			-0.285f, -0.055f, -4.0f,

			0.54f, 0.27f, 0.42f,

			-10.0f, 14.0f, 45.0f,

			projection
		);


		//Piramide roja superior derecha
		DibujarObjeto(
			PIRAMIDE, ROJO,
			0.285f, -0.055f, -4.0f,

			0.54f, 0.27f, 0.42f,

			-10.0f, 14.0f, -45.0f,

			projection
		);


		//Piramide magenta inferior izquierda
		DibujarObjeto(
			PIRAMIDE, MAGENTA,
			-0.285f, -0.625f, -4.0f,

			0.54f, 0.27f, 0.42f,

			-10.0f, 14.0f, 135.0f,

			projection
		);


		//Piramide verde inferior derecha
		DibujarObjeto(
			PIRAMIDE, VERDE,
			0.285f, -0.625f, -4.0f,

			0.54f, 0.27f, 0.42f,

			-10.0f, 14.0f, 225.0f,

			projection
		);


		//Cubo azul central
		DibujarObjeto(
			CUBO, AZUL,
			0.0f, -0.34f, -3.58f,

			0.50f, 0.50f, 0.38f,

			-16.0f, 22.0f, 45.0f,

			projection
		);


		//Cubo cafe central
		DibujarObjeto(
			CUBO, CAFE,
			0.0f, -0.34f, -3.18f,

			0.27f, 0.17f, 0.28f,

			-16.0f, 22.0f, 45.0f,

			projection
		);



		//Figura derecha

		//Piramide magenta superior
		DibujarObjeto(
			PIRAMIDE, MAGENTA,
			0.95f, -0.15f, -4.0f,

			0.40f, 0.38f, 0.45f,

			-12.0f, 16.0f, 0.0f,

			projection
		);


		//Piramide verde inferior izquierda
		DibujarObjeto(
			PIRAMIDE, VERDE,
			0.75f, -0.53f, -4.0f,

			0.40f, 0.38f, 0.45f,

			-12.0f, 16.0f, 0.0f,

			projection
		);


		//Piramide roja inferior derecha
		DibujarObjeto(
			PIRAMIDE, ROJO,
			1.15f, -0.53f, -4.0f,

			0.40f, 0.38f, 0.45f,

			-12.0f, 16.0f, 0.0f,

			projection
		);


		//Piramide amarilla central invertida
		DibujarObjeto(
			PIRAMIDE, AMARILLO,
			0.95f, -0.53f, -3.55f,

			0.40f, 0.38f, 0.45f,

			-12.0f, 16.0f, 180.0f,

			projection
		);


		
		/*
		//Letra U - Roja

		//Lado izquierdo
		DibujarFigura(
			2,
			-1.05f, 0.0f, -4.0f,
			0.10f, 1.0f,
			0.0f,
			uniformModel
		);

		//Lado derecho
		DibujarFigura(
			2,
			-0.65f, 0.0f, -4.0f,
			0.10f, 1.0f,
			0.0f,
			uniformModel
		);

		//Parte inferior
		DibujarFigura(
			2,
			-0.85f, -0.45f, -4.0f,
			0.50f, 0.10f,
			0.0f,
			uniformModel
		);



		//Letra R - Verde

		//Lado izquierdo
		DibujarFigura(
			3,
			-0.25f, 0.0f, -4.0f,
			0.10f, 1.0f,
			0.0f,
			uniformModel
		);

		//Parte superior
		DibujarFigura(
			3,
			-0.05f, 0.45f, -4.0f,
			0.50f, 0.10f,
			0.0f,
			uniformModel
		);

		//Parte central
		DibujarFigura(
			3,
			-0.05f, 0.05f, -4.0f,
			0.50f, 0.10f,
			0.0f,
			uniformModel
		);

		//Lado derecho superior
		DibujarFigura(
			3,
			0.20f, 0.25f, -4.0f,
			0.10f, 0.50f,
			0.0f,
			uniformModel
		);

		//Parte diagonal
		DibujarFigura(
			3,
			0.08f, -0.23f, -4.0f,
			0.10f, 0.62f,
			25.0f,
			uniformModel
		);



		//Letra C - Azul

		//Parte superior
		DibujarFigura(
			8,
			0.85f, 0.45f, -4.0f,
			0.55f, 0.10f,
			0.0f,
			uniformModel
		);

		//Lado izquierdo
		DibujarFigura(
			8,
			0.60f, 0.0f, -4.0f,
			0.10f, 1.0f,
			0.0f,
			uniformModel
		);

		//Parte inferior
		DibujarFigura(
			8,
			0.85f, -0.45f, -4.0f,
			0.55f, 0.10f,
			0.0f,
			uniformModel
		);
		*/

		//meshColorList[0]->RenderMeshColor();
		/*
		//Base negra
		DibujarFigura(
			9,
			0.0f, -0.76f, -4.5f,
			2.65f, 0.08f,
			0.0f,
			uniformModel
		);


		//Parte izquierda

		//Barra cafe izquierda
		DibujarFigura(
			4,
			-1.10f, -0.195f, -4.0f,
			0.05f, 1.05f,
			0.0f,
			uniformModel
		);

		//Barra cafe derecha
		DibujarFigura(
			4,
			-0.70f, -0.195f, -4.0f,
			0.05f, 1.05f,
			0.0f,
			uniformModel
		);


		//Triangulo amarillo
		DibujarFigura(
			6,
			-0.90f, 0.015f, -3.8f,
			0.175f, 0.147f,
			180.0f,
			uniformModel
		);

		//Triangulo rojo
		DibujarFigura(
			5,
			-0.90f, -0.279f, -3.8f,
			0.175f, 0.147f,
			180.0f,
			uniformModel
		);

		//Triangulo verde
		DibujarFigura(
			1,
			-0.90f, -0.573f, -3.8f,
			0.175f, 0.147f,
			180.0f,
			uniformModel
		);



		//Parte central

		//Triangulo amarillo superior izquierdo
		DibujarFigura(
			6,
			-0.285f, -0.055f, -4.0f,
			0.269f, 0.134f,
			45.0f,
			uniformModel
		);

		//Triangulo rojo superior derecho
		DibujarFigura(
			5,
			0.285f, -0.055f, -4.0f,
			0.269f, 0.134f,
			-45.0f,
			uniformModel
		);

		//Triangulo morado inferior izquierdo
		DibujarFigura(
			7,
			-0.285f, -0.625f, -4.0f,
			0.269f, 0.134f,
			135.0f,
			uniformModel
		);

		//Triangulo verde inferior derecho
		DibujarFigura(
			1,
			0.285f, -0.625f, -4.0f,
			0.269f, 0.134f,
			225.0f,
			uniformModel
		);


		//Rombo azul
		DibujarFigura(
			8,
			0.0f, -0.34f, -3.7f,
			0.54f, 0.54f,
			45.0f,
			uniformModel
		);

		//Rombo cafe
		model = glm::mat4(1.0f);

		model = glm::translate(
			model,
			glm::vec3(0.0f, -0.34f, -3.5f)
		);

		model = glm::scale(
			model,
			glm::vec3(0.30f, 0.18f, 1.0f)
		);

		model = glm::rotate(
			model,
			glm::radians(45.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		glUniformMatrix4fv(
			uniformModel,
			1,
			GL_FALSE,
			glm::value_ptr(model)
		);

		meshColorList[4]->RenderMeshColor();



		//Parte derecha

		//Triangulo morado superior
		DibujarFigura(
			7,
			0.95f, -0.15f, -4.0f,
			0.20f, 0.19f,
			0.0f,
			uniformModel
		);

		//Triangulo verde inferior izquierdo
		DibujarFigura(
			1,
			0.75f, -0.53f, -4.0f,
			0.20f, 0.19f,
			0.0f,
			uniformModel
		);

		//Triangulo rojo inferior derecho
		DibujarFigura(
			5,
			1.15f, -0.53f, -4.0f,
			0.20f, 0.19f,
			0.0f,
			uniformModel
		);

		//Triangulo amarillo central invertido
		DibujarFigura(
			6,
			0.95f, -0.53f, -3.8f,
			0.20f, 0.19f,
			180.0f,
			uniformModel
		);
		*/

		/*
		//Shader para las figuras con color
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Triangulo azul
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, 0.45f, -4.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.30f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshColorList[0]->RenderMeshColor();

		//Triangulo verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.55f, 0.45f, -4.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.30f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[1]->RenderMeshColor();

		//Cuadrado rojo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.60f, -0.40f, -4.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[2]->RenderMeshColor();

		//Cuadrado verde
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.40f, -4.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[3]->RenderMeshColor();

		//Cuadrado cafe
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.60f, -0.40f, -4.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshColorList[4]->RenderMeshColor();
		*/

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		/*shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		*/
		
		
		glUseProgram(0);
		mainWindow.swapBuffers();


	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/