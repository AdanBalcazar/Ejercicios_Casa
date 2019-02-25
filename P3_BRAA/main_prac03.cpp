/*----------------------------------------------------------------------------------*/
/*-----------------------------------  Pr�ctica 3 ----------------------------------*/
/*-----------------------------------    2019-2   ----------------------------------*/
/*------------- Computaci�n Gr�fica e Interacci�n Humano Computadora ---------------*/

/*------------- Balc�zar Rodr�guez Adan Al� ---------------*/
/*-------------- Visual Studio Versi�n 2017 ---------------*/

/*--------------------- Instructivo: ----------------------*/
/*
Con las teclas del teclado se puede mover a la figura
	A: Mueve a la izquierda
	D: Mueve a la derecha
	S: Aleja
	W: Acerca
	Q: Mueve hacia arriba
	E: Mueve hacia abajo

Con la funci�n colorShader.setVec3("color", #f, #f, #f); se establece
el color con el que se desea pintar el siguiente cubo, donde cada #f
es un n�mero entre 0.0 y 1.0, y representa la cantidad de rojo, verde
y azul (RGB) del cubo, respectivamente.

La carpeta Shaders debe estar dentro de la carpeta del proyecto.
*/

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
//Variables de control
float movX = 0.0f;
float movY = 0.0f;
float movZ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	//Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	Shader colorShader("shaders/shader_color.vs", "shaders/shader_color.fs");

	colorShader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection


	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f); //Anchura, Altura, Profundidad

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));  //Cuanto se mueve en x, en y, y en z
	// pass them to the shaders
	colorShader.setMat4("model", model);
	colorShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	colorShader.setMat4("projection", projection);

	//Se manda el color con el que se quiere dibujar el 1er cubo
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo

	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, 24); //A lonely cube :(

	//Nota: cualquier transformaci�n realizada sobre la matriz "model" es acumulativo a las siguientes transformaciones.

	//Se dibujan todos los dem�s cubos

	//1er l�nea
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//2da l�nea
	model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//3ra l�nea
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//4ta l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	//5ta l�nea
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//6ta l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//7ma l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//8va l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//9na l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 0.0f, 0.0f);  //Rojo
	glDrawArrays(GL_QUADS, 0, 24);
	
	//10ma l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-9.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	colorShader.setVec3("color", 1.0f, 1.0f, 1.0f);  //Blanco
	glDrawArrays(GL_QUADS, 0, 24);

	//11va l�nea
	model = glm::translate(model, glm::vec3(3.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//12va l�nea
	model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//13va l�nea
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//14va l�nea
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	colorShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	glBindVertexArray(0);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		my_input(window);

		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi funci�n de dibujo
		display();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  //GLFW_RELEASE
		movX += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  //GLFW_RELEASE
		movX -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  //GLFW_RELEASE
		movZ += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  //GLFW_RELEASE
		movZ -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)  //GLFW_RELEASE
		movY += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)  //GLFW_RELEASE
		movY -= 0.1f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}