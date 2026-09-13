//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	// Cara triangular exterior y una punta hacia dentro del cubo.
	// Esta primera instancia corresponde a la esquina superior izquierda.
	GLfloat vertices_piramide_triangular[] = {
		-1.0f,  1.0f,  0.0f,  // 0: esquina superior izquierda
		-1.0f,  0.0f,  0.0f,  // 1: mitad del borde izquierdo
		 0.0f,  1.0f,  0.0f,  // 2: mitad del borde superior
		-0.5f,  0.5f, -0.45f  // 3: punta hacia el interior
	};

	unsigned int indices_piramide_triangular[] = {
		0, 1, 2,  // cara exterior
		0, 3, 1,
		1, 3, 2,
		2, 3, 0
	};

	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(
		vertices_piramide_triangular,
		indices_piramide_triangular,
		12,
		12
	);

	meshList.push_back(piramidet);
}


//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}



/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	// paredes, dos triangulos por cada segmento.
	for (i = 0; i < res; i++)
	{
		unsigned int abajo = 2 * i;
		unsigned int arriba = abajo + 1;

		indices.push_back(abajo);
		indices.push_back(arriba);
		indices.push_back(abajo + 2);

		indices.push_back(arriba);
		indices.push_back(arriba + 2);
		indices.push_back(abajo + 2);
	}

	unsigned int base = 2 * (res + 1);
	unsigned int tapa = 3 * (res + 1);

	for (i = 1; i < res - 1; i++)
	{
		indices.push_back(base);
		indices.push_back(base + i);
		indices.push_back(base + i + 1);

		indices.push_back(tapa);
		indices.push_back(tapa + i + 1);
		indices.push_back(tapa + i);
	}

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

void DibujarInstancia(
	int indice,
	glm::vec3 posicion,
	glm::vec3 escala,
	glm::vec3 rotacion,
	glm::vec3 color,
	GLuint uniformModel,
	GLuint uniformColor,
	glm::mat4 matrizPadre = glm::mat4(1.0f))

{
	glm::mat4 model = matrizPadre;

	//Posicion
	model = glm::translate(
		model,
		posicion
	);

	//Rotacion X
	model = glm::rotate(
		model,
		glm::radians(rotacion.x),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);

	//Rotacion Y
	model = glm::rotate(
		model,
		glm::radians(rotacion.y),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	//Rotacion Z
	model = glm::rotate(
		model,
		glm::radians(rotacion.z),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	//Escala
	model = glm::scale(
		model,
		escala
	);

	//Enviar modelo al shader
	glUniformMatrix4fv(
		uniformModel,
		1,
		GL_FALSE,
		glm::value_ptr(model)
	);

	//Enviar color
	glUniform3fv(
		uniformColor,
		1,
		glm::value_ptr(color)
	);

	//Dibujar objeto
	if (indice == 5)
	{
		sp.render();
	}
	else if (indice == 3)
	{
		// el cono original usa un abanico de triangulos
		meshList[indice]->RenderMeshGeometry();
	}
	else
	{
		meshList[indice]->RenderMesh();
	};
}

void DibujarPatronCara(
	glm::mat4 matrizCara,
	GLuint uniformModel,
	GLuint uniformColor)
{
	glm::vec3 colores[] = {
		glm::vec3(1.0f, 1.0f, 0.0f), // amarillo: arriba izquierda
		glm::vec3(1.0f, 0.0f, 0.0f), // rojo: arriba derecha
		glm::vec3(0.0f, 1.0f, 0.0f), // verde: abajo derecha
		glm::vec3(0.5f, 0.0f, 1.0f)  // morado: abajo izquierda
	};

	// La misma piramide se repite en las cuatro esquinas.
	for (int i = 0; i < 4; i++)
	{
		DibujarInstancia(
			1,
			glm::vec3(0.0f, 0.0f, 0.003f),
			glm::vec3(1.0f),
			glm::vec3(0.0f, 0.0f, -90.0f * i),
			colores[i],
			uniformModel,
			uniformColor,
			matrizCara
		);
	}

	// Cubo delgado girado para formar el rombo central.
	DibujarInstancia(
		0,
		glm::vec3(0.0f, 0.0f, 0.012f),
		glm::vec3(0.8f, 0.8f, 0.04f),
		glm::vec3(0.0f, 0.0f, 45.0f),
		glm::vec3(0.478f, 0.255f, 0.067f),
		uniformModel,
		uniformColor,
		matrizCara
	);
}

vector<Mesh*> carasPiramide;

void CrearPiramidePorCaras()
{
	// cuatro esquinas de la base y punta.
	GLfloat vertices[] = {
		-0.65f, 0.0f,  0.65f, // 0
		 0.65f, 0.0f,  0.65f, // 1
		 0.65f, 0.0f, -0.65f, // 2
		-0.65f, 0.0f, -0.65f, // 3
		 0.0f, 1.4f,  0.0f   // 4: punta
	};

	unsigned int frente[] = { 0, 1, 4 };
	unsigned int derecha[] = { 1, 2, 4 };
	unsigned int atras[] = { 2, 3, 4 };
	unsigned int izquierda[] = { 3, 0, 4 };
	unsigned int base[] = { 0, 3, 2, 0, 2, 1 };

	Mesh* caraRoja = new Mesh();
	caraRoja->CreateMesh(vertices, frente, 15, 3);
	carasPiramide.push_back(caraRoja);

	Mesh* caraVerde = new Mesh();
	caraVerde->CreateMesh(vertices, derecha, 15, 3);
	carasPiramide.push_back(caraVerde);

	Mesh* caraAmarilla = new Mesh();
	caraAmarilla->CreateMesh(vertices, atras, 15, 3);
	carasPiramide.push_back(caraAmarilla);

	Mesh* caraMagenta = new Mesh();
	caraMagenta->CreateMesh(vertices, izquierda, 15, 3);
	carasPiramide.push_back(caraMagenta);

	Mesh* caraAzul = new Mesh();
	caraAzul->CreateMesh(vertices, base, 15, 6);
	carasPiramide.push_back(caraAzul);
}

void DibujarPiramideColores(
	glm::mat4 model,
	GLuint uniformModel,
	GLuint uniformColor)
{
	glm::vec3 colores[] = {
		glm::vec3(1.0f, 0.0f, 0.0f), // rojo
		glm::vec3(0.0f, 1.0f, 0.0f), // verde
		glm::vec3(1.0f, 1.0f, 0.0f), // amarillo
		glm::vec3(1.0f, 0.0f, 1.0f), // magenta
		glm::vec3(0.0f, 0.0f, 1.0f)  // azul
	};

	glUniformMatrix4fv(
		uniformModel,
		1,
		GL_FALSE,
		glm::value_ptr(model)
	);

	for (int cara = 0; cara < 5; cara++)
	{
		glUniform3fv(
			uniformColor,
			1,
			glm::value_ptr(colores[cara])
		);

		carasPiramide[cara]->RenderMesh();
	}
}

int main()
{
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(30, 1.0f);//índice 2 en MeshList
	CrearCono(30, 1.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	CrearPiramidePorCaras();

	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(
		glm::vec3(0.0f, 0.0f, 5.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,                     
		0.0f,                       
		3.0f,
		0.15f
	);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		glEnable(GL_DEPTH_TEST);
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo, pirámide triangular y pirámide base cuadrangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono
		//sp.render(); //dibuja esfera
		// ejercicio 2 practica 3 figura 
		// transformacion de toda la figura.
		glm::mat4 conjunto = glm::mat4(1.0f);

		// permite girar la union 
		conjunto = glm::rotate(conjunto,glm::radians(mainWindow.getrotax()),glm::vec3(1.0f, 0.0f, 0.0f));

		conjunto = glm::rotate(conjunto,glm::radians(mainWindow.getrotay()),glm::vec3(0.0f, 1.0f, 0.0f));

		conjunto = glm::rotate(conjunto,glm::radians(mainWindow.getrotaz()),glm::vec3(0.0f, 0.0f, 1.0f));

		
		// cuatro bases de las piramidescompatidas compartidas: arriba, derecha, abajo e izquierda.
		// cada posicion aparece dos veces porque cada una tiene 2 piramides.
		glm::vec3 posiciones[] = {
			glm::vec3(0.0f,  0.65f, 0.0f), // arriba: hacia afuera
			glm::vec3(0.0f,  0.65f, 0.0f), // arriba: hacia el centro

			glm::vec3(0.65f, 0.0f,  0.0f), // derecha: hacia afuera
			glm::vec3(0.65f, 0.0f,  0.0f), // derecha: hacia el centro

			glm::vec3(0.0f, -0.65f, 0.0f), // abajo: hacia afuera
			glm::vec3(0.0f, -0.65f, 0.0f), // abajo: hacia el centro

			glm::vec3(-0.65f, 0.0f,  0.0f), // izquierda: hacia afuera
			glm::vec3(-0.65f, 0.0f,  0.0f)  // izquierda: hacia el centro
		};

		// orienta las puntas en el plano XY.
		float girosZ[] = {
			  0.0f, 180.0f,  // arriba
			-90.0f,  90.0f,  // derecha
			180.0f,   0.0f,  // abajo
			 90.0f, -90.0f   // izquierda
		};

		// gira cada piramide sobre su eje para mostrar
		float girosY[] = {
			  0.0f,  90.0f,  
			180.0f, -90.0f, 
			 90.0f,   0.0f,  
			-90.0f, 180.0f   
		};

		for (int i = 0; i < 8; i++)
		{
			glm::mat4 model = glm::translate(
				conjunto,
				posiciones[i]
			);

			model = glm::rotate(
				model,
				glm::radians(girosZ[i]),
				glm::vec3(0.0f, 0.0f, 1.0f)
			);

			model = glm::rotate(
				model,
				glm::radians(girosY[i]),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

			model = glm::scale(
				model,
				glm::vec3(1.0f, 0.65f / 1.4f, 1.0f)
			);

			DibujarPiramideColores(
				model,
				uniformModel,
				uniformColor
			);
		}
		
		
		/*
		//ejercicio practica 3 cohete
		*/
		
		/*
		// plataforma del cohete
		DibujarInstancia(
			0,
			glm::vec3(0.0f, -0.10f, 0.0f),
			glm::vec3(6.0f, 0.20f, 6.0f),
			glm::vec3(0.0f),
			glm::vec3(0.10f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);

		// cuerpo del cohete, un cilindro gris
		DibujarInstancia(
			2,
			glm::vec3(0.0f, 2.40f, 0.0f),
			glm::vec3(0.65f, 3.0f, 0.65f),
			glm::vec3(0.0f),
			glm::vec3(0.80f, 0.80f, 0.85f),
			uniformModel, uniformColor
		);

		// punta roja del cohete
		DibujarInstancia(
			3,
			glm::vec3(0.0f, 4.50f, 0.0f),
			glm::vec3(0.65f, 1.20f, 0.65f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);

		// ventana, esfera aplanada azul
		DibujarInstancia(
			5,
			glm::vec3(0.0f, 2.90f, 0.65f),
			glm::vec3(0.24f, 0.24f, 0.10f),
			glm::vec3(0.0f),
			glm::vec3(0.05f, 0.40f, 1.0f),
			uniformModel, uniformColor
		);

		// propulsor izquierdo, cilindro
		DibujarInstancia(
			2,
			glm::vec3(-1.0f, 1.65f, 0.0f),
			glm::vec3(0.25f, 2.0f, 0.25f),
			glm::vec3(0.0f),
			glm::vec3(0.55f, 0.55f, 0.60f),
			uniformModel, uniformColor
		);

		// propulsor derecho, cilindro.
		DibujarInstancia(
			2,
			glm::vec3(1.0f, 1.65f, 0.0f),
			glm::vec3(0.25f, 2.0f, 0.25f),
			glm::vec3(0.0f),
			glm::vec3(0.55f, 0.55f, 0.60f),
			uniformModel, uniformColor
		);

		// punta izquierda del propulsor
		DibujarInstancia(
			3,
			glm::vec3(-1.0f, 2.90f, 0.0f),
			glm::vec3(0.25f, 0.50f, 0.25f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);

		// punta derecha del propulsor
		DibujarInstancia(
			3,
			glm::vec3(1.0f, 2.90f, 0.0f),
			glm::vec3(0.25f, 0.50f, 0.25f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);

		// aleta izquierda
		DibujarInstancia(
			4,
			glm::vec3(-0.55f, 1.50f, 0.0f),
			glm::vec3(0.75f, 1.10f, 0.20f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);


		// aleta derecha
		DibujarInstancia(
			4,
			glm::vec3(0.55f, 1.50f, 0.0f),
			glm::vec3(0.75f, 1.10f, 0.20f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);


		// aleta delantera
		DibujarInstancia(
			4,
			glm::vec3(0.0f, 1.50f, 0.55f),
			glm::vec3(0.20f, 1.10f, 0.75f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);


		// aleta trasera
		DibujarInstancia(
			4,
			glm::vec3(0.0f, 1.50f, -0.55f),
			glm::vec3(0.20f, 1.10f, 0.75f),
			glm::vec3(0.0f),
			glm::vec3(0.90f, 0.10f, 0.10f),
			uniformModel, uniformColor
		);

		// fuego central, cono invertido
		DibujarInstancia(
			3,
			glm::vec3(0.0f, 0.50f, 0.0f),
			glm::vec3(0.40f, 0.80f, 0.40f),
			glm::vec3(180.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.45f, 0.0f),
			uniformModel, uniformColor
		);

		// fuego del propulsor izquierdo
		DibujarInstancia(
			3,
			glm::vec3(-1.0f, 0.40f, 0.0f),
			glm::vec3(0.18f, 0.50f, 0.18f),
			glm::vec3(180.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.70f, 0.0f),
			uniformModel, uniformColor
		);

		// fuego del propulsor derecho
		DibujarInstancia(
			3,
			glm::vec3(1.0f, 0.40f, 0.0f),
			glm::vec3(0.18f, 0.50f, 0.18f),
			glm::vec3(180.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.70f, 0.0f),
			uniformModel, uniformColor
		);
		*/
		
		glm::vec3 colores[] = {
		glm::vec3(1.0f, 0.0f, 0.0f), // triangular roja
		glm::vec3(0.0f, 1.0f, 0.0f), // triangular verde
		glm::vec3(1.0f, 1.0f, 0.0f), // triangular amarilla
		glm::vec3(1.0f, 0.0f, 1.0f), // triangular magenta
		glm::vec3(0.0f, 0.0f, 1.0f)  // base cuadrada azul
		};

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	for (int i = 0; i < carasPiramide.size(); i++)
	{
		delete carasPiramide[i];
	}

	carasPiramide.clear();

	return 0;
}

	
		