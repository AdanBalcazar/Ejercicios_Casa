/*----------------------------------------------------------------------------------*/
/*-----------------------------------  Pr�ctica 9 ----------------------------------*/
/*-----------------------------------    2019-2   ----------------------------------*/
/*------------- Computaci�n Gr�fica e Interacci�n Humano Computadora ---------------*/
/*----------------------------------------------------------------------------------*/
/*--------------------------- Balc�zar Rodr�guez Adan Al� --------------------------*/
/*---------------------------- Visual Studio Versi�n 2017 --------------------------*/
/*----------------------------------------------------------------------------------*/
/*---------------------------------- Instructivo: ----------------------------------*/
/*
Con las siguientes teclas se puede mover todo el modelo:
	D: Mueve el modelo a la izquierda
	A: Mueve el modelo a la derecha
	W: Acerca el modelo
	S: Aleja el modelo

Con las siguientes teclas se cambia el tipo de iluminaci�n:
	1: Luz Direccional (Directional)
	2: Luz Posicional (Positional)
	3: Luz de Reflector (Spotlight)

Con las siguientes teclas se modifica el color de la luz:
	R: La luz se vuelve m�s roja
	G: La luz se vuelve m�s verde
	B: La luz se vuelve m�s azul

	H: La luz se vuelve completamente blanca
	T: La luz se apaga

Con las siguientes teclas se modifica la intensidad de la luz
	9: Disminuye la instensidad
	0: Aumenta la instensidad

---> NOTA IMPORTANTE: 
	Para modificar la intensidad de la luz y el color se debe presionar 
	varias veces la tecla para ir aumentando, si la tecla se deja presionada,
	�sta se tomar� como un solo incremento.

Con el Mouse se puede mover la c�mara desde la cual se observa el modelo
	Al mover el mouse a la derecha, la c�mara gira a la derecha
	Al mover el mouse a la izquierda, la c�mara gira a la izquierda
	Al mover el mouse hacia arriba, la c�mara gira hacia arriba
	Al mover el mouse hacia abajo, la c�mara gira hacia abajo
*/

#define STB_IMAGE_IMPLEMENTATION
#include "esfera.h"
#include "camera.h"
#include "stb_image.h"
#include <string.h>
using namespace std;

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

//Lighting                                    //Luz tipo Reflector
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);    //Posici�n de la Luz
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);  //Direcci�n de la Luz

void myData(void);
void display(void);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

// Para controlar la intensidad de la luz
int CeroPresionado = 0;
int NuevePresionado = 0;

// Para controlar el color de la luz
int RPresionado = 0;
int GPresionado = 0;
int BPresionado = 0;

//Para controlar la fuente de iluminaci�n --> SpotLight por defecto

char Light_VertxShader[100] = "shaders/shader_texture_light_spot.vs";
char Light_FragShader[100] = "shaders/shader_texture_light_spot.fs";

glm::vec3 Light_Position = camera.Position;
glm::vec3 Light_Direction = camera.Front;
float Light_CutOff = glm::cos(glm::radians(12.5f));


//-------- Texturas --------

//Difusas
unsigned int t_muro_enfrente;
unsigned int t_muro_derecha;
unsigned int t_muro_izquierda;
unsigned int t_muro_atras;

unsigned int t_punta_enfrente;
unsigned int t_punta_derecha;
unsigned int t_punta_izquierda;
unsigned int t_punta_atras;

unsigned int t_piedra;
unsigned int t_terracota;
unsigned int t_negro;

//Especulares
unsigned int t_muro_enfrente_brillo;
unsigned int t_muro_derecha_brillo;
unsigned int t_muro_izquierda_brillo;
unsigned int t_muro_atras_brillo;

unsigned int t_punta_enfrente_brillo;
unsigned int t_punta_derecha_brillo;
unsigned int t_punta_izquierda_brillo;
unsigned int t_punta_atras_brillo;

unsigned int t_piedra_brillo;
unsigned int t_terracota_brillo;


// ------- Propiedades de la Luz --------
float opacidad = 32.0f;

float R = 1.0f;
float G = 1.0f;
float B = 1.0f;

int Luz_Reflector = 1;  //Por defecto, la luz es de tipo Reflector

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

//Valores a colocar si la extensi�n de la textura es:
// .jpg ----> 0
// .png ----> 1
void LoadTextures()
{
	t_muro_enfrente = generateTextures("Texturas/MuroEnfrente.png", 1);
	t_muro_derecha = generateTextures("Texturas/MuroDerecha.png", 1);
	t_muro_izquierda = generateTextures("Texturas/MuroIzquierda.png", 1);
	t_muro_atras = generateTextures("Texturas/MuroAtras.png", 1);

	t_muro_enfrente_brillo = generateTextures("Texturas/MuroEnfrente_Especular.png", 1);
	t_muro_derecha_brillo = generateTextures("Texturas/MuroDerecha_Especular.png", 1);
	t_muro_izquierda_brillo = generateTextures("Texturas/MuroIzquierda_Especular.png", 1);
	t_muro_atras_brillo = generateTextures("Texturas/MuroAtras_Especular.png", 1);

	t_punta_enfrente = generateTextures("Texturas/PuntaEnfrente.png", 1);
	t_punta_derecha = generateTextures("Texturas/PuntaDerecha.png", 1);
	t_punta_izquierda = generateTextures("Texturas/PuntaIzquierda.png", 1);
	t_punta_atras = generateTextures("Texturas/PuntaAtras.png", 1);

	t_punta_enfrente_brillo = generateTextures("Texturas/PuntaEnfrente_Especular.png", 1);
	t_punta_derecha_brillo = generateTextures("Texturas/PuntaDerecha_Especular.png", 1);
	t_punta_izquierda_brillo = generateTextures("Texturas/PuntaIzquierda_Especular.png", 1);
	t_punta_atras_brillo = generateTextures("Texturas/PuntaAtras_Especular.png", 1);

	t_piedra = generateTextures("Texturas/Piedra.png", 1);
	t_terracota = generateTextures("Texturas/Terracota.png", 1);
	t_negro = generateTextures("Texturas/Negro.png", 1);

	t_piedra_brillo = generateTextures("Texturas/Piedra_Especular.png", 1);
	t_terracota_brillo = generateTextures("Texturas/Terracota_Especular.png", 1);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_muro_enfrente);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_muro_derecha);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_muro_izquierda);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, t_muro_atras);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, t_muro_enfrente_brillo);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, t_muro_derecha_brillo);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, t_muro_izquierda_brillo);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, t_muro_atras_brillo);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, t_punta_enfrente);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, t_punta_derecha);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, t_punta_izquierda);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, t_punta_atras);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, t_punta_enfrente_brillo);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, t_punta_derecha_brillo);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, t_punta_izquierda_brillo);
	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, t_punta_atras_brillo);

	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_2D, t_piedra);
	glActiveTexture(GL_TEXTURE18);
	glBindTexture(GL_TEXTURE_2D, t_terracota);
	glActiveTexture(GL_TEXTURE19);
	glBindTexture(GL_TEXTURE_2D, t_negro);

	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, t_piedra_brillo);
	glActiveTexture(GL_TEXTURE21);
	glBindTexture(GL_TEXTURE_2D, t_terracota_brillo);
}

void myData()
{	
	float vertices[] = {

		// ------------ CUBO ------------
		// V�rtices: 0-24

		// Posici�n           //Normales            // UV's

		//Cara de enfrente
		 0.5f,  0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,    // top right
		 0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,    // bottom right
		-0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    // bottom left
		-0.5f,  0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,    // top left 

		//Cara de atr�s
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,    // top right
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,    // bottom right
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,    // bottom left
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,    // top left 

		//Cara de arriba
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f, 0.0f,   1.0f, 1.0f,    // top right
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f, 0.0f,   1.0f, 0.0f,    // bottom right
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f, 0.0f,   0.0f, 0.0f,    // bottom left
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f, 0.0f,   0.0f, 1.0f,    // top left 

		//Cara de abajo
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,    // top right
	     0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,    // bottom right
	    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,    // bottom left
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,    // top left 

		//Cara de la derecha
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,    // top right
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    // bottom right
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,    // bottom left
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,    // top left 

		 //Cara de la izquierda
		-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,    // top right
		-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    // bottom right
		-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,    // bottom left
		-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,    // top left 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{
}

void display(void)
{
	//Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");

	// ------ Aqu� se selecciona el tipo de luz a utilizar, descomentar la l�nea que se va a ocupar -----

	//Shader lightingShader("shaders/shader_texture_light_dir.vs", "shaders/shader_texture_light_dir.fs");    //Directional
	//Shader lightingShader("shaders/shader_texture_light_pos.vs", "shaders/shader_texture_light_pos.fs");    //Positional
	//Shader lightingShader("shaders/shader_texture_light_spot.vs", "shaders/shader_texture_light_spot.fs");  //Spotlight

	Shader lightingShader(Light_VertxShader, Light_FragShader);
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	//To Use Lighting
	lightingShader.use();

	// ------ Aqu� se selecciona el tipo de luz a utilizar, descomentar la l�nea que se va a ocupar -----

	//	Directional - Luz direccional, se manda el sentido en el que apunta la fuente de luz
	//lightingShader.setVec3("light.direction", lightDirection);

	//	Positional - Luz posicional, se manda la ubicaci�n de la fuente de luz
	//lightingShader.setVec3("light.position", lightPosition);

	//	Spotlight - Luz de reflector, la luz se encuentra en la c�mara
	//lightingShader.setVec3("light.position", camera.Position);
	//lightingShader.setVec3("light.direction", camera.Front);
	//lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));

	lightingShader.setVec3("light.position", Light_Position);
	lightingShader.setVec3("light.direction", Light_Direction);
	lightingShader.setFloat("light.cutOff", Light_CutOff);

	lightingShader.setVec3("viewPos", camera.Position);

	/*
	// Propiedades de la luz --- Luz Est�tica (Blanca)
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	*/

	// Propiedades de la luz --- Luz Variable
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", R, G, B);
	lightingShader.setVec3("light.specular", R, G, B);

	//PAra la Luz Posicional y de Reflector
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// Brillo del material
	lightingShader.setFloat("material_shininess", opacidad);

	//Matriz de modelo, vista t proyecci�n
	glm::mat4 model_temp = glm::mat4(1.0f); //Maztriz auxiliar de modelo
	glm::mat4 model = glm::mat4(1.0f);		//Initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	// Mientras la Luz sea de tipo reflector, �sta se estar� actualizando constantemente en base a la posici�n de la c�mara
	if (Luz_Reflector == 1)
	{
		Light_Position = camera.Position;
		Light_Direction = camera.Front;
		Light_CutOff = glm::cos(glm::radians(12.5f));
	}

	// ------------------------- Zona de Dibujo -------------------------

	glBindVertexArray(VAO);

	// ------------ Muros  ------------

	// Cara de enfrente
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.2f, 2.0f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_muro_enfrente);
	lightingShader.setInt("material_specular", t_muro_enfrente_brillo);
	glDrawArrays(GL_QUADS, 0, 4);

	// Cara de atr�s
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.2f, 2.0f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_muro_atras);
	lightingShader.setInt("material_specular", t_muro_atras_brillo);
	glDrawArrays(GL_QUADS, 4, 4);

	// Cara de arriba
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0.0f, 0.375f, 0.0f));
	model = glm::scale(model, glm::vec3(3.2f, 1.25f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_terracota);
	lightingShader.setInt("material_specular", t_terracota_brillo);
	glDrawArrays(GL_QUADS, 8, 4);

	// Cara de abajo
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0.0f, -0.375f, 0.0f));
	model = glm::scale(model, glm::vec3(3.2f, 1.25f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_piedra);
	lightingShader.setInt("material_specular", t_piedra_brillo);
	glDrawArrays(GL_QUADS, 12, 4);

	// Cara de la derecha
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0.975f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.25f, 2.0f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_muro_derecha);
	lightingShader.setInt("material_specular", t_muro_derecha_brillo);
	glDrawArrays(GL_QUADS, 16, 4);

	// Cara de la izquierda
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-0.975f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.25f, 2.0f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_muro_izquierda);
	lightingShader.setInt("material_specular", t_muro_izquierda_brillo);
	glDrawArrays(GL_QUADS, 20, 4);


	// ------------ Pin�culo  ------------

	// Cara de enfrente
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0.0f, 1.17f, 0.0f));
	model_temp = model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.02f, 0.34f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_punta_enfrente);
	lightingShader.setInt("material_specular", t_punta_enfrente_brillo);
	glDrawArrays(GL_QUADS, 0, 4);

	// Cara de atr�s
	model = model_temp;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.66f));
	model = glm::scale(model, glm::vec3(1.02f, 0.34f, 1.0f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_punta_atras);
	lightingShader.setInt("material_specular", t_punta_atras_brillo);
	glDrawArrays(GL_QUADS, 4, 4);

	// Cara de arriba
	model = model_temp;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.33f));
	model = glm::scale(model, glm::vec3(1.02f, 0.34f, 0.34f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_terracota);
	lightingShader.setInt("material_specular", t_terracota_brillo);
	glDrawArrays(GL_QUADS, 8, 4);

	// Cara de la derecha
	model = model_temp;
	model = glm::translate(model, glm::vec3(0.34f, 0.0f, 0.33));
	model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_punta_derecha);
	lightingShader.setInt("material_specular", t_punta_derecha_brillo);
	glDrawArrays(GL_QUADS, 16, 4);

	// Cara de la izquierda
	model = model_temp;
	model = glm::translate(model, glm::vec3(-0.34f, 0.0f, 0.33));
	model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_punta_izquierda);
	lightingShader.setInt("material_specular", t_punta_izquierda_brillo);
	glDrawArrays(GL_QUADS, 20, 4);


	// ------------- Luz ---------------
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, glm::vec3(0.25f));
	lampShader.setMat4("model", model);

	//glBindVertexArray(lightVAO);
	glDrawArrays(GL_QUADS, 0, 24);	//Light

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 9", NULL, NULL);
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
	LoadTextures();
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

		//Mi funci�n de dibujo
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


	// --------------- Tipo de Iluminaci�n

	// Luz Direccional
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		strcpy_s(Light_VertxShader, "shaders/shader_texture_light_dir.vs");
		strcpy_s(Light_FragShader, "shaders/shader_texture_light_dir.fs");
		Light_Direction = lightDirection;

		Luz_Reflector = 0;   //La luz ya no es de tipo reflector
	}

	// Luz Posicional
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		strcpy_s(Light_VertxShader, "shaders/shader_texture_light_pos.vs");
		strcpy_s(Light_FragShader, "shaders/shader_texture_light_pos.fs");

		Light_Position = lightPosition;

		Luz_Reflector = 0;  //La luz ya no es de tipo reflector
	}

	// Luz de Reflector
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		strcpy_s(Light_VertxShader, "shaders/shader_texture_light_spot.vs");
		strcpy_s(Light_FragShader, "shaders/shader_texture_light_spot.fs");

		Luz_Reflector = 1;  //La luz es de tipo reflector
	}


	// --------------- Intensidad de la luz
	
	// Aumenta
	if ((glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) && (CeroPresionado == 0))
	{
		opacidad -= 2;
		CeroPresionado = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE)
		CeroPresionado = 0;

	// Disminuye
	if ((glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) && (NuevePresionado == 0))
	{
		opacidad += 2;
		NuevePresionado = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_RELEASE)
		NuevePresionado = 0;


	// ------- Color de la luz

	// Aumenta el Rojo
	if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (RPresionado == 0))
	{
		R += 0.1f;
		RPresionado = 1;
		
		if (R > 1.1f)
			R = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
		RPresionado = 0;

	// Aumenta el Verde
	if ((glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) && (GPresionado == 0))
	{
		G += 0.1f;
		GPresionado = 1;

		if (G > 1.1f)
			G = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
		GPresionado = 0;

	// Aumenta el Azul
	if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) && (BPresionado == 0))
	{
		B += 0.1f;
		BPresionado = 1;

		if (B > 1.1f)
			B = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		BPresionado = 0;

	// Luz Blanca
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		R = 1.0f;
		G = 1.0f;
		B = 1.0f;
	}

	// Luz Apagada
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		R = 0.0f;
		G = 0.0f;
		B = 0.0f;
	}
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

	camera.ProcessMouseMovement(xoffset*3, yoffset*3);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}