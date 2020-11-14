#include <iostream>

#define GLM_SWIZZLE
#include <glm/glm.hpp>	
#include <glm/gtx/string_cast.hpp>

void Constructors() {
	std::cout << std::endl;

	glm::vec2 Point0{ 10.0f, 10.0f };
	glm::vec3 Point1{10.0f, 0.0f, 0.0f};
	glm::vec4 Point2{ 10.0f, 10.0f, 10.0f, 10.0f };

	std::cout << "Point0: " << glm::to_string(Point0) << std::endl;
	std::cout << "Point1: " << glm::to_string(Point1) << std::endl;
	std::cout << "Point2: " << glm::to_string(Point2) << std::endl;
}
void Components() {
	std::cout << std::endl;
	glm::vec3 P{ 1,2,3 };

	std::cout << "X:" << P.x << " Y:" << P.y << " Z:" << P.z << std::endl;
}

void Operations() {
	std::cout << std::endl;

	glm::vec3 P0{ 10.0f, 10.0f, 0.0f };
	glm::vec3 P1{ 10.0f, 10.0f, 10.0f };

	glm::vec3 R;
	//Soma
	R = P0 + P1;
	std::cout << glm::to_string(R) << std::endl;

	//Subtração
	R = P0 - P1;
	std::cout << glm::to_string(R) << std::endl;

	//Scala
	R = P0 * 10.0f;
	R = P0 / 10.0f;
	std::cout << glm::to_string(R) << std::endl;

	//Multiplicação
	R = P0 * P1;
	std::cout << glm::to_string(R) << std::endl;

	//Divisão
	R = P0 / P1;
	std::cout << glm::to_string(R) << std::endl;

	//Diferente de P0.length()
	float L = glm::length(P0);
	std::cout << L << std::endl;

	//Normalizar um vetor
	glm::vec3 Norm = glm::normalize(P0);

	//Produto Escalar (Dot Product)
	float Dot = glm::dot(P0, P1);

	//Produto Vetorial (cross Product)
	glm::vec3 Cross = glm::cross(P0, P1);

	//Distancia
	float Distancia = glm::distance(P0, P1);

	//Refração
	glm::vec3 Refle = glm::refract(P0, Norm, 1.0f);

	//Reflect
	glm::vec3 Reflect = glm::reflect(P0,Norm);
}

void Swizzle() {
	std::cout << std::endl;
	glm::vec3 P{ 1,2,3 };

	glm::vec3 Q = P.xxx;
	glm::vec3 R = P.xyx;

	std::cout << glm::to_string(Q) << std::endl;
}

int main() {
	
	Constructors();
	Components();
	Swizzle();
}