#include <iostream>
#include <cassert>
#include<array>
#include <fstream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include <glm/ext.hpp> 

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

const int Width = 800;
const int Height = 600;

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

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 UV;
};

int main() {

	//Inicializa a biblioteca GLFW
	assert(glfwInit() == GLFW_TRUE);

	//Criar uma janela 
	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	assert(Window);

	//Ativa o contexto criado na janela do Windows
	glfwMakeContextCurrent(Window);

	//Inicializa a biblioteca GLEW
	assert(glewInit() == GLEW_OK);

	//Veifica a versão do OpenGL que esta sendo usada
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "OpenGL Version: " << GLMajorVersion << "," << GLMinorVersion << std::endl;

	//Usar o glGetString() para obter informações do driver que estamos usando
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	GLuint TextureId = LoadTexture("textures/earth_2k.jpg");

	//Definindo um triangulo em coordenadas normalizadas
	std::array<Vertex, 3> Triangle = {
		Vertex{glm::vec3{-1.0f,-1.0f,0.0f},glm::vec3{1.0f,0.0f,0.0f}, glm::vec2{0.0f,0.0f}},
		Vertex{glm::vec3{1.0f,-1.0f,0.0f},glm::vec3{0.0f,1.0f,0.0f},glm::vec2{1.0f,0.0f} },
		Vertex{glm::vec3{0.0f,1.0f,0.0f},glm::vec3{0.0f,0.0f,1.0f},glm::vec2{0.5f,1.0f} } 
	};

	//________________________________________________________________________Inicio da Camera________________________________________________________________________
	//Model Matrix
	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	//View Matrix
	glm::vec3 Eye{ 0,0,5 };
	glm::vec3 Center{ 0,0,0 };
	glm::vec3 Up{ 0,1,0 };
	glm::mat4 ViewMatrix = glm::lookAt(Eye, Center, Up);

	//Projection Matrix
	constexpr float FoV = glm::radians(45.0f);
	const float AspectRation = Width / Height;
	const float Near = 0.001f;
	const float Far = 1000.0f;
	glm::mat4 ProjectionMatrix = glm::perspective(FoV, AspectRation, Near, Far);

	//ModelViewProjection
	glm::mat4 ModelViewProjection = ProjectionMatrix * ViewMatrix * ModelMatrix;


	//Aplicar a ModelViewProjection nos vértices do trianglo
	/*for (Vertex& Vertex : Triangle) USA O CALCULO NO PROCESSADOR
	{
		glm::vec4 ProjectedVertex = ModelViewProjection * glm::vec4{ Vertex.Position, 1.0f };
		ProjectedVertex /= ProjectedVertex.w;
		Vertex.Position = ProjectedVertex;
	}*/
	//________________________________________________________________________Fim da Camera________________________________________________________________________

	//Agora, vamos copiar os vértices do triangulo para a memória da GPU
	GLuint VertexBuffer;

	//Pedir para o OpenGL gerar o identificador do VertexBuffer
	glGenBuffers(1, &VertexBuffer);

	//Ativar o VertexBuffer como sendo o buffer para onde vamos copiar os dados do triangulo
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//Copiar os dados para a memória de vídeo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(), GL_STATIC_DRAW);


	//Definir a cor de fundo
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	//Entra no loop de eventos da aplicação
	while (!glfwWindowShouldClose(Window)) {
		// glClear vai limpar o framebuffer. GLCOLOR_BUFFER_BIT diz para limpar o buffer de cor. Depois de limpar, ele vai preencher com a cor que foi configurada no glClearColor.
		//Quando formos desenhar geometris 3D, vamos voltar ao glClear porque teremos que limpar o buffer de profundidade(depth buffer)
		glClear(GL_COLOR_BUFFER_BIT);

		//Ativar o programa de shader
		glUseProgram(ProgramId);


		GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramId, "ModelViewProjection");
		glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "TextureSampler");
		glUniform1i(TextureSamplerLoc, 0);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Diz para o OpenGL que o VertexBuffer vai ser o buffer ativo no momento
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

		//Informa ao OpenGL onde, dentro do vertexBuffer, onde os vertices estão
		//NO caso o array Triangles é contiguoo em memória, logo basta dizer quantos vertices vamos usar para desenhar o triangulo
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex),
							reinterpret_cast<void*>(offsetof(Vertex, Color)));

		glVertexAttribPointer(2,2,GL_FLOAT,GL_TRUE,sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

		//Finalmente diz para o OpenGL desenhar o triangulo com os dados que estão armazenados no VertexBuffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Reverter o estado que nós criamos
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//Desabilitar o programa ativo
		glUseProgram(0);


		//Processa todos os eventos da fila de evnetos do GLFW
		//Eventos podem ser do teclado, mouse, gamepad
		glfwPollEvents();

		//Envia o conteudo do framebuffer da janaela para ser desenhada na tela
		glfwSwapBuffers(Window);
	}

	//Desalocar o VertexBuffer
	glDeleteBuffers(1, &VertexBuffer);

	//Encerra a biblioteca GLFW
	glfwTerminate();
	return 0;
}