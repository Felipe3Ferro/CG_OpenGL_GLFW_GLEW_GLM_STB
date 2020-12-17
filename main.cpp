#include <iostream>
#include <cassert>
#include<array>
#include <fstream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include <glm/ext.hpp> 
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

int Width = 800;
int Height = 600;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 UV;
};

std::string ReadFile(const char* FilePath) {
	std::string FileContents;
	if (std::ifstream FileStream{ FilePath, std::ios::in }) {

		// Vai ler dentro do FIleContents o conteudo do arquivo apontado por FilePath
		FileContents.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}
	return FileContents;
}

void CheckShader(GLuint ShaderId) {
	//ShaderId tem que ser um identificador de um shader já compilado

	GLint Result = GL_TRUE;
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE) {

		//Houve um erro ao compilar o shader, vamos imprimir o log para saber qual foi o erro

		//Obter o tamanho do log
		GLint InfoLogLength = 0;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0) {

			std::string ShaderInfoLog(InfoLogLength, '\0');
			glGetShaderInfoLog(ShaderId, InfoLogLength, nullptr, &ShaderInfoLog[0]);

			std::cout << "Erro no shader" << std::endl;
			std::cout << ShaderInfoLog << std::endl;

			assert(false);

		}
	}
}

GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile) {
	std::string VertexShaderSource = ReadFile(VertexShaderFile);
	std::string FragmentShaderSource = ReadFile(FragmentShaderFile);

	assert(!VertexShaderSource.empty());
	assert(!FragmentShaderSource.empty());

	//Cria os identificadores do Vertex e Shader
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	std::cout << "Compilando " << VertexShaderFile << std::endl;
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertexShaderId);
	CheckShader(VertexShaderId);

	// Verificar se a compilação do VertexShader deu certo

	std::cout << "Compilando " << FragmentShaderFile << std::endl;
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	glShaderSource(FragmentShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	glCompileShader(FragmentShaderId);
	CheckShader(FragmentShaderId);

	// Verificar se a compilação do FragmentShader deu certo

	std::cout << "Linkando o programa" << std::endl;
	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);

	//Verifica se o programa foi linkado corretamente

	GLint Result = GL_TRUE;

	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE) {

		//Houve um erro ao compilar o shader, vamos imprimir o log para saber qual o erro

		//Obtem o tamanho do log
		GLint InfoLogLength = 0;
		glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0) {
			std::string ProgramInfoLog(InfoLogLength, '\0');
			glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramInfoLog[0]);

			std::cout << "Erro ao linkar o programa" << std::endl;
			std::cout << ProgramInfoLog << std::endl;
			assert(false);
		}
	}

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	return ProgramId;

}

GLuint LoadGeometry() {

	//Definindo um triangulo em coordenadas normalizadas
	std::array<Vertex, 6> Quad = {
		Vertex{ glm::vec3{-1.0f,-1.0f,0.0f},
				glm::vec3{1.0f,0.0f,0.0f},
				glm::vec2{0.0f,0.0f} },

		Vertex{ glm::vec3{1.0f,-1.0f,0.0f},
				glm::vec3{0.0f,1.0f,0.0f},
				glm::vec2{1.0f,0.0f} },

		Vertex{ glm::vec3{1.0f,1.0f,0.0f},
				glm::vec3{1.0f,0.0f,0.0f},
				glm::vec2{1.0f,1.0f} },

		Vertex{ glm::vec3{-1.0f,1.0f,0.0f},
				glm::vec3{0.0f,0.0f,1.0f},
				glm::vec2{0.0f,1.0f} }

	};

	std::array<glm::ivec3, 2> Indices = {
		glm::ivec3{0,1,3},
		glm::ivec3{3,1,2}
	};


	//Agora, vamos copiar os vértices do triangulo para a memória da GPU
	GLuint VertexBuffer;

	//Pedir para o OpenGL gerar o identificador do VBO
	glGenBuffers(1, &VertexBuffer);

	//Pedir para o OpenGL gerar o identificador do EBO
	GLuint ElementBuffer = 0;
	glGenBuffers(1, &ElementBuffer);

	//Ativar o VertexBuffer como sendo o buffer para onde vamos copiar os dados do triangulo
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//Copiar os dados para a memória de vídeo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW);

	//copiar os dados do Element Buffer para GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

	//Gerar o Vertex Array Object (VAO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	//Habilitar o VAO
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Diz para o OpenGL que o VertexBuffer vai ser o buffer ativo no momento
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);


	//Informa ao OpenGL onde, dentro do vertexBuffer, onde os vertices estão
	//NO caso o array Triangles é contiguoo em memória, logo basta dizer quantos vertices vamos usar para desenhar o triangulo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Color)));

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

	//Desabilita o VAO
	glBindVertexArray(0);

	return VAO;
}

GLuint LoadTexture(const char* TextureFile) {
	std::cout << "Carregando Textura" << TextureFile << std::endl;

	stbi_set_flip_vertically_on_load(true);

	int TextureWidth,TextureHeight,NumberOfComponents = 0;


	unsigned char* TextureData= stbi_load(TextureFile, &TextureWidth, &TextureHeight, &NumberOfComponents,3);

	assert(TextureData);

	//Gerar o identificador da textura
	GLuint TextureId;
	glGenTextures(1, &TextureId);

	//Habilita a textura para ser modificada
	glBindTexture(GL_TEXTURE_2D, TextureId);

	//Copia a textura para a memória de vídeo (GPU)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,TextureWidth,TextureHeight,0,GL_RGB,GL_UNSIGNED_BYTE,TextureData);

	//Filtros de magnificação e minificação
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Configurar o Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Gerar o mipmap a partir da textura

	glGenerateMipmap(GL_TEXTURE_2D);

	//Desligar a textura pois ela ja foi copiada para GPU
	glBindTexture(GL_TEXTURE_2D,0);

	stbi_image_free(TextureData);

	return TextureId;
}

class FlyCamera {
public:
	

	void MoveFoward(float Amount) {
		Location += glm::normalize(Direction) * Amount * Speed;
		//std::cout << glm::to_string(Location);
	}

	void MoveRight(float Amount) {
		glm::vec3 Rigth = glm::normalize(glm::cross(Direction, Up));

		Location += Rigth * Amount * Speed;
	}

	void Look(float Yaw, float Pitch) {
		Yaw *= Sensitivity;
		Pitch *= Sensitivity;

		const glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));

		const glm::mat4 I = glm::identity<glm::mat4>();
		glm::mat4 YawRotation = glm::rotate(I,glm::radians(Yaw),-Up);
		glm::mat4 PitchRotation = glm::rotate(I, glm::radians(Pitch), -Right);


		Up = PitchRotation * glm::vec4{ Up,0.0f };
		Direction = YawRotation * PitchRotation *glm::vec4{ Direction,0.0f };

	}

	glm::mat4 GetViewProjection() const {
		glm::mat4 View = glm::lookAt(Location, Location + Direction, Up);
		glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
		return Projection * View;
	}
	//Parametros de Interatividade
	float Speed = 5.0f;
	float Sensitivity = 0.1f;

	//Definição da Matriz de View
	glm::vec3 Location{ 0.0f,0.0f,10.0f };
	glm::vec3 Direction{ 0.0f,0.0f,-1.0f };
	glm::vec3 Up{ 0.0f,1.0f,0.0f };

	//Definição da Matriz Projection
	float FieldOfView = glm::radians(45.0f);
	float AspectRatio = Width / Height;
	float Near = 0.01f;
	float Far = 1000.0f;
};

FlyCamera Camera;
bool bEnableMouseMovement = false;
glm::vec2 PreviousCursor{ 0.0,0.0 };

void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Modifiers) {
	if (Button == GLFW_MOUSE_BUTTON_LEFT) {
		if (Action == GLFW_PRESS) {
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			bEnableMouseMovement = true;

			double X, Y;
			glfwGetCursorPos(Window, &X, &Y);

			PreviousCursor = glm::vec2{ X,Y };
		}
		if (Action == GLFW_RELEASE) {
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			bEnableMouseMovement = false;
		}
	}
}

void MouseMotionCallback(GLFWwindow* Window, double X, double Y) {
	if (bEnableMouseMovement) {
		glm::vec2 CurrentCursor{ X,Y };
		glm::vec2 DeltaCursor = CurrentCursor - PreviousCursor;

		//std::cout << glm::to_string(DeltaCursor) << std::endl;
		Camera.Look(DeltaCursor.x,DeltaCursor.y);
		PreviousCursor = CurrentCursor;

	}
}

void Resize(GLFWwindow* Window, int NewWidth, int NewHeight) {
	Width = NewWidth;
	Height = NewHeight;

	Camera.AspectRatio = static_cast<float>(Width) / Height;
	glViewport(0, 0, Width, Height);
}

int main() {

	//Inicializa a biblioteca GLFW
	assert(glfwInit() == GLFW_TRUE);

	//Criar uma janela 
	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	assert(Window);

	//Cadastrar as callbacks no GLFW
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);
	glfwSetCursorPosCallback(Window, MouseMotionCallback);
	glfwSetFramebufferSizeCallback(Window, Resize);

	//Ativa o contexto criado na janela do Windows
	glfwMakeContextCurrent(Window);

	//Habilita ou Desabilita o V-sync
	glfwSwapInterval(0);

	//Inicializa a biblioteca GLEW
	assert(glewInit() == GLEW_OK);

	/*//Veifica a versão do OpenGL que esta sendo usada
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "OpenGL Version: " << GLMajorVersion << "," << GLMinorVersion << std::endl;

	//Usar o glGetString() para obter informações do driver que estamos usando
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;*/

	Resize(Window, Width, Height);

	GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	GLuint TextureId = LoadTexture("textures/earth_2k.jpg");

	GLuint QuadVao = LoadGeometry();

	//Model Matrix
	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	//Definir a cor de fundo
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	//Guarda o tempo do frame anterior
	double PreviousTime = glfwGetTime();

	//Habilitar o BackFace culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Entra no loop de eventos da aplicação
	while (!glfwWindowShouldClose(Window)) {

		double CurrentTime = glfwGetTime();
		double DeltaTime = CurrentTime - PreviousTime;

		if (DeltaTime > 0) {
			PreviousTime = CurrentTime;
		}

		// glClear vai limpar o framebuffer. GLCOLOR_BUFFER_BIT diz para limpar o buffer de cor. Depois de limpar, ele vai preencher com a cor que foi configurada no glClearColor.
		//Quando formos desenhar geometris 3D, vamos voltar ao glClear porque teremos que limpar o buffer de profundidade(depth buffer)
		glClear(GL_COLOR_BUFFER_BIT);

		//Ativar o programa de shader
		glUseProgram(ProgramId);

		glm::mat4 ViewProjectionMatrix = Camera.GetViewProjection();

		glm::mat4 ModelViewProjection = ViewProjectionMatrix * ModelMatrix;


		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramId, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);

		glBindVertexArray(QuadVao);

		//Finalmente diz para o OpenGL desenhar o triangulo com os dados que estão armazenados no VertexBuffer
		// glDrawArrays(GL_TRIANGLES, 0, Quad.size());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		//Desabilitar o programa ativo
		glUseProgram(0);


		//Processa todos os eventos da fila de evnetos do GLFW
		//Eventos podem ser do teclado, mouse, gamepad
		glfwPollEvents();

		//Envia o conteudo do framebuffer da janaela para ser desenhada na tela
		glfwSwapBuffers(Window);

		//Processa os Inputs do teclado
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
			Camera.MoveFoward(1.0f * DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
			Camera.MoveFoward(-1.0f * DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
			Camera.MoveRight(-1.0f * DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
			Camera.MoveRight(1.0f * DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwTerminate();
		}
	}

	//Desalocar o VertexBuffer
	glDeleteVertexArrays(1, &QuadVao);

	//Encerra a biblioteca GLFW
	glfwTerminate();
	return 0;
}