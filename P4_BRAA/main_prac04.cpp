/*----------------------------------------------------------------------------------*/
/*-----------------------------------  Práctica 4 ----------------------------------*/
/*-----------------------------------    2019-2   ----------------------------------*/
/*------------- Computación Gráfica e Interacción Humano Computadora ---------------*/

/*------------- Balcázar Rodríguez Adan Alí ---------------*/
/*-------------- Visual Studio Versión 2017 ---------------*/

/*--------------------- Instructivo: ----------------------*/
/*
Con las teclas del teclado se puede mover a la figura
	W: Aleja
	A: Mueve a la izquierda
	S: Acerca
	D: Mueve a la derecha
	RePág: Mueve hacia arriba
	AvPág: Mueve hacia abajo
	Flecha Arriba: Rota hacia arriba
	Flecha Abajo: Rota hacia abajo 
	Flecha Izquierda: Rota a la izquierda
	Flecha Derecha: Rota a la derecha
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
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f;

float angX = 0.0f,
      angY = 0.0f;

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
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

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
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));

	//Rotación
	view = glm::rotate(view, glm::radians(angX), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(angY), glm::vec3(1, 0, 0));
	
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);
	glBindVertexArray(VAO);

	// Colores:
	//	Lengua 0.827f, 0.470f, 0.470f
	//	Cuerpo 0.67f, 0.18f, 0.16f
	//	Estómago/Labios 0.35f, 0.12f, 0.1f
	//	Nariz/Orejas 1.0f, 0.48f, 0.45f
	//	Ojos/Colmillos 0.996f, 0.866f, 0.482f
	//	Pelo/Pezuñas 0.215f, 0.003f, 0.003f


	// ------------ Se comienza con el dibujo de Pumbaa -----------

	// -------> Cuerpo

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::scale(model, glm::vec3(10, 11, 16));     //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Estómago

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.35f, 0.12f, 0.1f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -4.5, 0));  //Traslación
	model = glm::scale(model, glm::vec3(10.1, 2.1, 6));                      //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pata Delantera Derecha

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3, -7, 6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 2));                         //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pezuña Delantera Derecha

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3, -9, 6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 2));                         //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pata Delantera Izquierda

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, -7, 6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 2));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pezuña Delantera Izquierda

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, -9, 6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 2));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pata Trasera Derecha

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3, -7, -6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 2));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pezuña Trasera Derecha

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3, -9, -6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 2));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pata Trasera Izquierda

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, -7, -6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 2));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pezuña Trasera Izquierda

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, -9, -6));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 2));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Cola (Inicio)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 6, -8.5));  //Traslación
	model = glm::scale(model, glm::vec3(1, 6, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Cola (Mitad)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 8.5, -10));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 2));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Cola (Punta)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 7.5, -10.5));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                              //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Cola (Pelo)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 6, -10.5));  //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 1));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Cabeza

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 3, 8)); //Traslación
	model = glm::scale(model, glm::vec3(10, 13, 5));                     //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Oreja Derecha (Atrás)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2.5, 11, 7.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 1));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Oreja Derecha (Adelante)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2.5, 11, 8.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 1));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Oreja Izquierda (Atrás)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2.5, 11, 7.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 1));                              //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Oreja Izquierda (Adelante)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2.5, 11, 8.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 3, 1));                              //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pelo de la Cabeza

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 10.5, 6.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 2, 6));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pelo de la Nuca

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 7.5, 4.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 4, 2));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pelo del Lomo

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.215f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 6.5, 1.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 2, 4));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pómulos

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 4.5, 11.5));  //Traslación
	model = glm::scale(model, glm::vec3(10, 2, 2));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Ojo Derecho (Lat Izq)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(1.5, 6.5, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 1));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Ojo Derecho (Sup)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2.5, 7, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Ojo Derecho (Lat Der)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3.5, 6.5, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 1));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pupila Derecha

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2.5, 6, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Ojo Izquierdo (Lat Izq)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-1.5, 6.5, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 1));                              //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Ojo Izquierdo (Sup)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2.5, 7, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Ojo Izquierdo (Lat Der)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3.5, 6.5, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 1));                              //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Pupila Izquierda

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2.5, 6, 11));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Tabique (Inferior)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 4.5, 14.5));  //Traslación
	model = glm::scale(model, glm::vec3(6, 2, 4));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Tabique (Superior)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 6, 13.5));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 6));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Centro)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 4.5, 17));  //Traslación
	model = glm::scale(model, glm::vec3(2, 4, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Derecha - Sup)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2, 5, 17));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 1));                         //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Derecha - Lat)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2.5, 4, 17));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Derecha - Inf)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(2, 3, 17));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 1));                         //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Izquierda - Sup)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2, 5, 17));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 1));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Izquierda - Lat)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2.5, 4, 17));  //Traslación
	model = glm::scale(model, glm::vec3(1, 1, 1));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Nariz (Izquierda - Inf)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.48f, 0.45f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2, 3, 17));  //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 1));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Mentón

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.67f, 0.18f, 0.16f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -2.5, 13));  //Traslación
	model = glm::scale(model, glm::vec3(10, 2, 5));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Labio Inferior

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.35f, 0.12f, 0.1f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -1, 13.5));  //Traslación
	model = glm::scale(model, glm::vec3(10, 1, 6));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Contorno Labio Derecho

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.35f, 0.12f, 0.1f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(4.5, 1, 11));    //Traslación
	model = glm::scale(model, glm::vec3(1, 3, 1));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Contorno Labio Izquierdo

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.35f, 0.12f, 0.1f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-4.5, 1, 11));   //Traslación
	model = glm::scale(model, glm::vec3(1, 3, 1));                             //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Labio Superior (Atrás)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.35f, 0.12f, 0.1f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 3, 11.5));   //Traslación
	model = glm::scale(model, glm::vec3(10, 1, 2));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Labio Superior (Delante)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.35f, 0.12f, 0.1f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 3, 14.5));   //Traslación
	model = glm::scale(model, glm::vec3(6, 1, 4));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Interior de la Boca

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.12f, 0.003f, 0.003f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 1.5, 11));  //Traslación
	model = glm::scale(model, glm::vec3(8, 2, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Lengua

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.827f, 0.470f, 0.470f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 0, 13));  //Traslación
	model = glm::scale(model, glm::vec3(8, 1, 5));                         //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);
	

	// -------> Diente

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-2, 0, 16));   //Traslación
	model = glm::scale(model, glm::vec3(2, 1, 1));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Colmillo Derecho (Base)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(3, 2, 15));   //Traslación
	model = glm::scale(model, glm::vec3(3, 1, 2));                          //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Colmillo Derecho (Lateral)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(4, 3.5, 15));   //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 2));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Colmillo Izquierdo (Base)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-3, 2, 15));   //Traslación
	model = glm::scale(model, glm::vec3(3, 1, 2));                           //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
	glDrawArrays(GL_QUADS, 0, 24);


	// -------> Colmillo Izquierdo (Lateral)

	//Se cambia el color
	projectionShader.setVec3("aColor", glm::vec3(0.99f, 0.87f, 0.55f));

	//Se realiza traslación y escala
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-4, 3.5, 15));  //Traslación
	model = glm::scale(model, glm::vec3(1, 2, 2));                            //Escala
	projectionShader.setMat4("model", model);

	//Se dibuja
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4", NULL, NULL);
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

		//Mi función de dibujo
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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		angX -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		angX += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		angY -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		angY += 0.8f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}