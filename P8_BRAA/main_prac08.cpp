/*----------------------------------------------------------------------------------*/
/*-----------------------------------  Práctica 8 ----------------------------------*/
/*-----------------------------------    2019-2   ----------------------------------*/
/*------------- Computación Gráfica e Interacción Humano Computadora ---------------*/
/*----------------------------------------------------------------------------------*/
/*--------------------------- Balcázar Rodríguez Adan Alí --------------------------*/
/*---------------------------- Visual Studio Versión 2017 --------------------------*/
/*----------------------------------------------------------------------------------*/
/*---------------------------------- Instructivo: ----------------------------------*/
/*
Con las teclas del teclado se puede mover todo el modelo:
	D: Mueve el modelo a la izquierda
	A: Mueve el modelo a la derecha
	W: Acerca el modelo
	S: Aleja el modelo

Con el Mouse se puede mover la cámara desde la cual se observa el modelo
	Al mover el mouse a la derecha, la cámara gira a la derecha
	Al mover el mouse a la izquierda, la cámara gira a la izquierda
	Al mover el mouse hacia arriba, la cámara gira hacia arriba
	Al mover el mouse hacia abajo, la cámara gira hacia abajo
*/


#define STB_IMAGE_IMPLEMENTATION
#include "esfera.h"
#include "camera.h"
#include <stb_image.h>

Esfera my_sphere(1.0f);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void myData(void);
void display(void);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Texture
unsigned int texture1, texture2; //índices de cada textura para saber cuál mandar a llamar

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
}

void myData()
{	
	float vertices[] = {

		// ------------ CUBO  ------------
		// Vértices: 0-24
		// Posición          // UV's

		//Cara de enfrente
		 0.5f,  0.5f, 0.5f,   0.5f, 0.6666f,   // top right
		 0.5f, -0.5f, 0.5f,   0.5f, 0.3333f,   // bottom right
		-0.5f, -0.5f, 0.5f,   0.25f, 0.3333f,  // bottom left
		-0.5f,  0.5f, 0.5f,   0.25f, 0.6666f,  // top left 

		//Cara de atrás
		 0.5f,  0.5f, -0.5f,   1.0f, 0.6666f,   // top right
		 0.5f, -0.5f, -0.5f,   1.0f, 0.3333f,   // bottom right
		-0.5f, -0.5f, -0.5f,   0.75f, 0.3333f,  // bottom left
		-0.5f,  0.5f, -0.5f,   0.75f, 0.6666f,  // top left 

		//Cara de arriba
		 0.5f,  0.5f, -0.5f,  0.5f, 1.0f,      // top right
		 0.5f,  0.5f,  0.5f,  0.5f, 0.6666f,   // bottom right
		-0.5f,  0.5f,  0.5f,  0.25f, 0.6666f,  // bottom left
		-0.5f,  0.5f, -0.5f,  0.25f, 1.0f,     // top left 

		//Cara de la derecha
		 0.5f,  0.5f, -0.5f,  0.75f, 0.6666f,  // top right
		 0.5f, -0.5f, -0.5f,  0.75f, 0.3333f,  // bottom right
		 0.5f, -0.5f,  0.5f,  0.5f, 0.3333f,   // bottom left
		 0.5f,  0.5f,  0.5f,  0.5f, 0.6666f,   // top left 

		 //Cara de la izquierda
		 -0.5f,  0.5f, -0.5f,  0.25f, 0.6666f,  // top right
		 -0.5f, -0.5f, -0.5f,  0.25f, 0.3333f,  // bottom right
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.3333f,   // bottom left
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.6666f,   // top left 

		 //Cara de abajo
		  0.5f, -0.5f, -0.5f,  0.5f,  0.3333f,  // top right
		  0.5f, -0.5f,  0.5f,  0.5f,  0.0f,     // bottom right
		 -0.5f, -0.5f,  0.5f,  0.25f, 0.0f,     // bottom left
		 -0.5f, -0.5f, -0.5f,  0.25f, 0.3333f,  // top left 

		 // ------------ Plano ------------
		 // Vértices: 24-28
		 // Posición          // UV's

		 //Textura Completa repetida 4 veces (Cara Superior)
		  0.5f,  0.5f, -0.5f,  4.0f, 4.0f,    // top right
		  0.5f,  0.5f,  0.5f,  4.0f, 0.f,     // bottom right
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.f,     // bottom left
		 -0.5f,  0.5f, -0.5f,  0.0f, 4.0f,    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and create a texture 
	// -------------------------
	
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("Texturas/Piedra.jpg", &width, &height, &nrChannels, 0);     
	if (data)
	{
		// IMPORTANT note
		//remember to change to RGBA if image has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Si trabajo con texturas con transparencias debo habilitar el canal alfa, entonces en lugar de usar GL_RGB usaremos GL_RGBA
		// .jpg ---> RGB
		// .png ---> RGBA
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("Texturas/Ajedrez.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void animate(void)
{
}

void display(void)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);                      //Activamos la textura 1 cuyo índice se guarda en texture1
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	Shader projectionShader("shaders/shader_texture.vs", "shaders/shader_texture.fs");
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	//To Use Lighting
	projectionShader.use();                      //Se activa el Shader de proyección que se va a utilizar
	//projectionShader.setInt("texture1", 0); 
	//projectionShader.setInt("texture2", 1);

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	glm::mat4 modelTemp = glm::mat4(1.0f);  //Matriz auxiliar de dibujo

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	// --------- Zona de Dibujo ----------

	glBindVertexArray(VAO);
	
	//Mesa
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, -1, 0)); 
	modelTemp = model;
	model = glm::scale(model, glm::vec3(5, 1, 5));
	projectionShader.setInt("Texture", 0);              //Textura de Piedra
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Tablero
	model = modelTemp;
	model = glm::translate(model, glm::vec3(0, 0.51, 0));
	model = glm::scale(model, glm::vec3(2.5, 0.01, 2.5));
	projectionShader.setInt("Texture", 1);              //Textura de Ajedrez
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 24, 28);

	//Soporte
	model = modelTemp;
	model = glm::translate(model, glm::vec3(0, -2.5, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(1.5, 4, 1.5));
	projectionShader.setInt("Texture", 0);              //Textura de Piedra
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Banco 1
	model = modelTemp;
	model = glm::translate(model, glm::vec3(-4, -0.5, 0));
	model = glm::scale(model, glm::vec3(1.5, 3, 1.5));
	projectionShader.setInt("Texture", 0);              //Textura de Piedra
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Banco 2
	model = modelTemp;
	model = glm::translate(model, glm::vec3(4, -0.5, 0));
	model = glm::scale(model, glm::vec3(1.5, 3, 1.5));
	projectionShader.setInt("Texture", 0);              //Textura de Piedra
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	glBindVertexArray(0);

	//--------- Fin de la zona de Dibujo -----------
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 8", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();

	//Mis funciones
	//Datos a utilizar
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        my_input(window);
		animate();

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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}