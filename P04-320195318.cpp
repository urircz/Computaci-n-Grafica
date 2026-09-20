/* Practica 4: Sonda espacial con modelado jerarquico.
E/R/T: girar la sonda. F/G: paneles solares. H/J: antena.
WASD y raton: camara. Esc: salir. */
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
Camera camera;
Window mainWindow(800, 600);
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
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
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
// Cilindro cerrado, con triangulos para las paredes y las tapas.
void CrearCilindro(int res, float R)
{
    vector<GLfloat> vertices;
    vector<unsigned int> indices;
    GLfloat dt = 2 * PI / res;

    for (int i = 0; i < res; i++)
    {
        GLfloat x = R * cos(i * dt);
        GLfloat z = R * sin(i * dt);
        vertices.push_back(x);
        vertices.push_back(-0.5f);
        vertices.push_back(z);
        vertices.push_back(x);
        vertices.push_back(0.5f);
        vertices.push_back(z);
    }

    // Centros de la tapa inferior y superior.
    vertices.push_back(0.0f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    for (int i = 0; i < res; i++)
    {
        unsigned int a = 2 * i;
        unsigned int b = 2 * ((i + 1) % res);
        // Dos triangulos de la pared.
        indices.push_back(a); indices.push_back(a + 1); indices.push_back(b);
        indices.push_back(b); indices.push_back(a + 1); indices.push_back(b + 1);
        // Tapa inferior.
        indices.push_back(2 * res); indices.push_back(a); indices.push_back(b);
        // Tapa superior.
        indices.push_back(2 * res + 1); indices.push_back(b + 1); indices.push_back(a + 1);
    }
    Mesh* cilindro = new Mesh();
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

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	if (mainWindow.Initialise() != 0) return 1;
	glfwSetWindowTitle(glfwGetCurrentContext(), "Practica 4: Sonda espacial - E/R/T cuerpo - F/G paneles - H/J antena");
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(18, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	

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

	camera = Camera(glm::vec3(10.0f, 8.0f, 18.0f), glm::vec3(0.0f, 1.0f, 0.0f), -119.0f, -20.0f, 6.0f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux2(1.0);// GUARDAR TEMPORALMENTE BIFURCACIONES

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	lastTime = glfwGetTime();
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


// parte del rover: cabina, base, brazo, pinza y seis llantas.
// camera = Camera(glm::vec3(-11.0f, 8.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -68.0f, -6.0f, 6.0f, 0.3f);
// ======================================================================
// // Las esferas azules representan las articulaciones del brazo.
// 		model = glm::mat4(1.0);
// 	
// 		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -4.0f)); //NOS POSICIONAMOS EN EL CENTRO DEL OBJETO X AMARILLA 
// 		modelaux = model; //guardamos la matriz de modelo para que la base se mueva con el objeto
// 		// Creando la cabina del rover ¿CUÁNTAS UNIDADES MEDIRÁ EN Z?
// 		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));//PARA LLEGAR AL CENTRO DE LA CABINA a PARTIR DEL ORIGEN
// 		modelaux = model;
// 		model = glm::scale(model, glm::vec3(8.0f, 4.0f, 6.0f));
// 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// 		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución se programe cambio entre proyección ortogonal y perspectiva
// 		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
// 		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
// 		color = glm::vec3(0.65f, 0.65f, 0.65f);
// 		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
// 		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
// 		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
// 		//sp.render(); //dibuja esfera
// 
// 
// 		// BASE
// 		//model = glm::mat4(1.0); //Si dejamos está linea, la base no estará unida al centro del objeto. se debe comentar para que la base se mueva con el objeto.
// 		/*En su lugar usamos la matriz auxiliar modelaux para que la base se mueva con el objeto.
// 		Lo que debemos de saber es: de las transformaciones geométricas que se aplican a la cabina y 
// 		cuales queremos que se apliquen a la base. En este caso, sólo queremos que se aplique la traslación del origen, no la rotación ni el escalado de la cabina. 
// 		Por lo tanto, debemos de guardar en modelaux sólo la traslación del origen y luego aplicarla a la base.
// 		*/
// 		model = modelaux;
// 		//NOS POSICIONAMOS EN EL CENTRO DE LA BASE
// 		model = glm::translate(model, glm::vec3(-1.0f, -2.75f, 0.0f));
// 		modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
// 		model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f));
// 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// 		color = glm::vec3(0.45f, 0.45f, 0.45f);
// 		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
// 		meshList[0]->RenderMesh();
// 		
// 		model = modelaux;
// 		modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
// 		// modelaux2 conserva la base para empezar cada rama.
// 
// 		//BASE PARA EL BRAZO
// 		model = modelaux2;
// 		model = glm::translate(model, glm::vec3(-3.5f, 1.25f, 2.5f));
// 		//modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
// 		//model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f)); NO se requirio escalar el cubo
// 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// 		color = glm::vec3(0.75f, 0.75f, 0.75f);
// 		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
// 		meshList[0]->RenderMesh();
// 
// 		//EJE DE ROTACIÓN
// 		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
// 		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
// 		modelaux = model;
// 		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
// 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// 		color = glm::vec3(0.0f, 0.0f, 1.0f);
// 		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
// 		sp.render();
// 
// 		//Brazo
// 		model = modelaux;
// 		model = glm::translate(model, glm::vec3(-1.06066f, 1.06066f, 0.0f));
// 		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
// 		modelaux = model;
// 		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f));
// 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// 		color = glm::vec3(0.75f, 0.75f, 0.75f);
// 		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
// 		meshList[2]->RenderMesh();
// 
// 		//eje br1-brz2
// 		model = modelaux;
// 		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
// 		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
// 		modelaux = model;
// 		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
// 		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// 		color = glm::vec3(0.0f, 0.0f, 1.0f);
// 		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
// 		sp.render();
// 		
//         // BRAZO 2. Partimos del codo, sin el escalado de la esfera.
//         model = modelaux;
//         model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//         model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
//         modelaux = model;
//         model = glm::scale(model, glm::vec3(0.35f, 4.0f, 0.35f));
//         glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//         color = glm::vec3(0.75f, 0.75f, 0.75f);
//         glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//         meshList[2]->RenderMesh();
// 
//         // MUNECA. H gira la pinza desde el extremo del segundo brazo.
//         model = modelaux;
//         model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
//         model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
//         modelaux = model;
//         model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
//         glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//         color = glm::vec3(0.0f, 0.0f, 1.0f);
//         glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//         sp.render();
// 
//         // Base de la pinza.
//         model = modelaux;
//         model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.0f));
//         modelaux = model;
//         model = glm::scale(model, glm::vec3(0.9f, 0.3f, 0.4f));
//         glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//         color = glm::vec3(0.4f, 0.4f, 0.4f);
//         glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//         meshList[0]->RenderMesh();
// 
//         // J abre y cierra la pinza entre 0 y 35 grados.
//         GLfloat apertura = fmod(mainWindow.getarticulacion4(), 140.0f);
//         if (apertura > 70.0f) apertura = 140.0f - apertura;
//         apertura = apertura / 2.0f;
//         glm::mat4 modelaux3(1.0f);
// 
//         // Un dedo a cada lado. Los dos parten de la base de la pinza.
//         for (int lado = -1; lado <= 1; lado += 2)
//         {
//             model = modelaux;
//             model = glm::translate(model, glm::vec3(lado * 0.35f, 0.15f, 0.0f));
//             model = glm::rotate(model, glm::radians(-lado * apertura), glm::vec3(0.0f, 0.0f, 1.0f));
//             modelaux3 = model;
//             model = glm::translate(model, glm::vec3(0.0f, 0.425f, 0.0f));
//             model = glm::scale(model, glm::vec3(0.18f, 0.85f, 0.25f));
//             glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//             color = glm::vec3(0.8f, 0.8f, 0.8f);
//             glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//             meshList[0]->RenderMesh();
// 
//             // Punta doblada hacia el centro.
//             model = modelaux3;
//             model = glm::translate(model, glm::vec3(-lado * 0.15f, 0.85f, 0.0f));
//             model = glm::scale(model, glm::vec3(0.4f, 0.18f, 0.25f));
//             glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//             meshList[0]->RenderMesh();
//         }
// 
//         // seis llantas: tres por lado, cada una con su propio angulo.
//         GLfloat posicionX[] = { -7.5f, -0.5f, 6.0f };
//         GLfloat inicioX[] = { -3.5f, 2.5f, 2.5f };
//         GLfloat soporteZ[] = { 3.8f, 4.6f, 4.6f };
//         GLfloat llantaZ[] = { 4.5f, 5.35f, 5.35f };
// 
//         for (int lado = -1; lado <= 1; lado += 2)
//         {
//             // dos anclajes por costado
//             for (int anclaje = 0; anclaje < 2; anclaje++)
//             {
//                 model = modelaux2;
//                 model = glm::translate(model, glm::vec3(inicioX[anclaje], -0.25f,
//                     lado * (3.75f + soporteZ[anclaje]) / 2.0f));
//                 model = glm::scale(model, glm::vec3(0.65f, 0.65f,
//                     soporteZ[anclaje] - 3.75f + 0.55f));
//                 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//                 color = glm::vec3(0.65f, 0.65f, 0.65f);
//                 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//                 meshList[0]->RenderMesh();
//             }
// 
//             for (int rueda = 0; rueda < 3; rueda++)
//             {
//                 // dos barras paralelas por pata
//                 GLfloat distanciaX = posicionX[rueda] - inicioX[rueda];
//                 GLfloat distanciaY = -2.0f - (-0.25f);
//                 GLfloat longitud = sqrt(distanciaX * distanciaX + distanciaY * distanciaY);
//                 GLfloat inclinacion = atan2(distanciaY, distanciaX);
// 
//                 for (int barra = -1; barra <= 1; barra += 2)
//                 {
//                     model = modelaux2;
//                     model = glm::translate(model, glm::vec3(
//                         (inicioX[rueda] + posicionX[rueda]) / 2.0f, -1.125f,
//                         lado * soporteZ[rueda] + barra * 0.14f));
//                     model = glm::rotate(model, inclinacion, glm::vec3(0.0f, 0.0f, 1.0f));
//                     model = glm::scale(model, glm::vec3(longitud, 0.28f, 0.14f));
//                     glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//                     color = glm::vec3(0.78f, 0.78f, 0.72f);
//                     glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//                     meshList[0]->RenderMesh();
//                 }
// 
//                 // eje entre las barras y el centro de la llanta.
//                 model = modelaux2;
//                 model = glm::translate(model, glm::vec3(posicionX[rueda], -2.0f,
//                     lado * (soporteZ[rueda] + llantaZ[rueda]) / 2.0f));
//                 model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//                 model = glm::scale(model, glm::vec3(0.22f,
//                     llantaZ[rueda] - soporteZ[rueda] + 0.45f, 0.22f));
//                 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//                 color = glm::vec3(0.45f, 0.45f, 0.45f);
//                 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//                 meshList[2]->RenderMesh();
// 
//                 GLfloat giro = 0.0f;
//                 if (rueda == 0) {
//                     if (lado == 1) giro = mainWindow.getarticulacion5();  // K
//                     else giro = mainWindow.getarticulacion6();          // L
//                 }
//                 if (rueda == 1) {
//                     if (lado == 1) giro = mainWindow.getarticulacion7();  // U
//                     else giro = mainWindow.getarticulacion8();          // I
//                 }
//                 if (rueda == 2) {
//                     if (lado == 1) giro = mainWindow.getarticulacion9();  // O
//                     else giro = mainWindow.getarticulacion10();         // P
//                 }
// 
//                 //ninguna rueda hereda el giro de otra.
//                 model = modelaux2;
//                 model = glm::translate(model, glm::vec3(posicionX[rueda], -2.0f,
//                     lado * llantaZ[rueda]));
//                 model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 0.0f, 1.0f));
//                 modelaux3 = model; // Guardar el giro antes de acostar/escalar el cilindro.
//                 model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//                 model = glm::scale(model, glm::vec3(1.25f, 0.7f, 1.25f));
//                 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//                 color = glm::vec3(0.04f, 0.04f, 0.04f);
//                 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//                 meshList[2]->RenderMesh();
// 
//                 // rayos en ambas caras para que se vea el giro individual.
//                 for (int cara = -1; cara <= 1; cara += 2)
//                 {
//                     for (int rayo = 0; rayo < 4; rayo++)
//                     {
//                         model = modelaux3;
//                         model = glm::translate(model, glm::vec3(0.0f, 0.0f, cara * 0.37f));
//                         model = glm::rotate(model, glm::radians(rayo * 45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//                         model = glm::scale(model, glm::vec3(2.1f, 0.12f, 0.08f));
//                         glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
//                         color = glm::vec3(0.7f, 0.7f, 0.7f);
//                         glUniform3fv(uniformColor, 1, glm::value_ptr(color));
//                         meshList[0]->RenderMesh();
//                     }
//                 }
//             }
//         }

        // matriz padre: todas las piezas heredan el giro de la sonda.
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
        modelaux = model; // Guardamos antes del escalado del cuerpo.

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        //cuerpo central: un cubo dorado.
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.8f, 0.6f, 0.15f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh();

        // paneles solares
        for (int lado = -1; lado <= 1; lado += 2)
        {
            model = modelaux;
            model = glm::translate(model, glm::vec3(lado * 1.5f, 0.0f, 0.0f));
            GLfloat giro = (lado == -1) ? mainWindow.getarticulacion1() : mainWindow.getarticulacion2();
            model = glm::rotate(model, glm::radians(lado * giro), glm::vec3(0, 0, 1));
            modelaux2 = model; // Bisagra: F mueve el panel izquierdo; G el derecho.

            //soporte entre el cuerpo y el panel
            model = glm::translate(model, glm::vec3(lado * 0.5f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 0.2f, 0.2f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            color = glm::vec3(0.7f, 0.7f, 0.7f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[0]->RenderMesh();

            // panel delgado, conectado al extremo del soporte.
            model = modelaux2;
            model = glm::translate(model, glm::vec3(lado * 3.0f, 0.0f, 0.0f));
            modelaux2 = model; // Las divisiones tambien se mueven con el panel.
            model = glm::scale(model, glm::vec3(4.0f, 0.16f, 3.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            color = glm::vec3(0.05f, 0.2f, 0.65f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[0]->RenderMesh();

            // tres divisiones para representar las celdas solares.
            for (int division = -1; division <= 1; division++)
            {
                model = modelaux2;
                model = glm::translate(model, glm::vec3(division * 1.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.05f, 0.18f, 3.0f));
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                color = glm::vec3(0.55f, 0.65f, 0.85f);
                glUniform3fv(uniformColor, 1, glm::value_ptr(color));
                meshList[0]->RenderMesh();
            }
        }

        // antena
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(25.0f + mainWindow.getarticulacion4()), glm::vec3(0, 0, 1));
        modelaux2 = model; // H orienta la antena; J cambia su inclinacion.

        //mastil cilidrico
        model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.12f, 1.2f, 0.12f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.7f, 0.7f, 0.7f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[2]->RenderMesh();

        // plato simplificado
        model = modelaux2;
        model = glm::translate(model, glm::vec3(0.0f, 1.4f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 0.2f, 1.2f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.9f, 0.9f, 0.9f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        sp.render();

        // receptor en el centro del plato.
        model = modelaux2;
        model = glm::translate(model, glm::vec3(0.0f, 1.9f, 0.0f));
        model = glm::scale(model, glm::vec3(0.07f, 0.6f, 0.07f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.9f, 0.65f, 0.15f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[2]->RenderMesh();

        // propulsor
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.35f, 0.35f, 0.35f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[2]->RenderMesh();

        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -2.35f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.55f, 0.55f, 0.55f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[3]->RenderMeshGeometry(); // El cono utiliza GL_TRIANGLE_FAN.


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		