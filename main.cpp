#include <iostream>
#include <cassert>
#include<array>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include <glm/ext.hpp> 


const int Width = 800; 
const int Height = 600;

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

	//Definindo um triangulo em coordenadas normalizadas
	std::array<glm::vec3, 3> Triangle = {
		glm::vec3{-1.0f,-1.0f,0.0f},
		glm::vec3{1.0f,-1.0f,0.0f},
		glm::vec3{0.0f,1.0f,0.0f}
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
	for (glm::vec3& Vertex : Triangle) 
	{
		glm::vec4 ProjectedVertex = ModelViewProjection * glm::vec4{ Vertex, 1.0f };
		ProjectedVertex /= ProjectedVertex.w;
		Vertex = ProjectedVertex;
	}
	//________________________________________________________________________Fim da Camera________________________________________________________________________

	//Agora, vamos copiar os vértices do triangulo para a memória da GPU
	GLuint VertexBuffer;

	//Pedir para o OpenGL gerar o identificador do VertexBuffer
	glGenBuffers(1, &VertexBuffer);

	//Ativar o VertexBuffer como sendo o buffer para onde vamos copiar os dados do triangulo
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//Copiar os dados para a memória de vídeo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(),GL_STATIC_DRAW);


	//Definir a cor de fundo
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//Entra no loop de eventos da aplicação
	while(!glfwWindowShouldClose(Window)){
		// glClear vai limpar o framebuffer. GLCOLOR_BUFFER_BIT diz para limpar o buffer de cor. Depois de limpar, ele vai preencher com a cor que foi configurada no glClearColor.
		//Quando formos desenhar geometris 3D, vamos voltar ao glClear porque teremos que limpar o buffer de profundidade(depth buffer)
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		//Diz para o OpenGL que o VertexBuffer vai ser o buffer ativo no momento
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

		//Informa ao OpenGL onde, dentro do vertexBuffer, onde os vertices estão
		//NO caso o array Triangles é contiguoo em memória, logo basta dizer quantos vertices vamos usar para desenhar o triangulo
		glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0, nullptr);

		//Finalmente diz para o OpenGL desenhar o triangulo com os dados que estão armazenados no VertexBuffer
		glDrawArrays(GL_TRIANGLES, 0,3);

		//Reverter o estado que nós criamos
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);

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