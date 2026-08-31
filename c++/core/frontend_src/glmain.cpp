#include "shader.h"
#include "myimgui.h"
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <cmath>

#define GL_CHECK()                                      \
	{                                                   \
		GLenum err = glGetError();                      \
		if (err != GL_NO_ERROR)                         \
			std::cout << "GL ERROR: " << err            \
					  << " line: " << __LINE__ << '\n'; \
	}

std::vector<std::pair<float, float>> GL::gridvert;

uint16_t GL::gWindowHeight, GL::gWindowWidth;
std::array<float, 3> GL::gCamera;
std::array<std::array<float, 3>, 3> GL::gCameraDesc;


Shader *GL::shader;
uint64_t GL::Compile(SimDet &det)
{

	/*for (size_t i{}; i < cs.edge.size(); i++)
	{
		std::cout << "Edge" << " "<< cs.edge[i] << std::endl;
	}
	std::cout << "size" << " " << cs.vpc.size() << std::endl;*/
	/*for(size_t i =0; i< cs.triangles.size(); i++) {
	std::cout << "Triangle: "<< cs.triangles[i][0] << " " << cs.triangles[i][1] << " " << cs.triangles[i][2] << std::endl;
	};
	for (size_t i {}; i <cs.vpc.size(); i++) {
		std::cout << cs.vpc[i] << std::endl;
	}*/
	//std::cout << "Reached GL::Compile" << std::endl;

	glGenVertexArrays(1, &det.VAO);
	//std::cout << "Past det.VAO" << std::endl;
	glBindVertexArray(det.VAO);

	glGenBuffers(1, &det.VBO);
	glGenBuffers(1, &det.EBO);

	glGenBuffers(1, &det.lEBO);

	/*if(det.VAO != 0)
	{
		std::cout << "VAO done" << std::endl;
	}*/
	// ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, det.EBO);
	//std::cout << "det ebo data" << std::endl;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, det.cs->triangles.size() * 3 * sizeof(uint32_t), det.cs->triangles.data(), GL_STATIC_DRAW);
	//std::cout << "Past ebo data" << std::endl;
	/*if(det.EBO != 0)
	{
		std::cout << "EBO done" << std::endl;
	}*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, det.lEBO);
	//std::cout << "det lebo data" << std::endl;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, det.cs->edge.size() * sizeof(uint32_t), det.cs->edge.data(), GL_STATIC_DRAW);
	//std::cout << "Past det lebo data" << std::endl;
	/*if(det.lEBO != 0)
	{
		std::cout << "lEBO done" << std::endl;
	}*/
	// position vbo
	glBindBuffer(GL_ARRAY_BUFFER, det.VBO);
	//std::cout << "det vbo data" << std::endl;
	glBufferData(GL_ARRAY_BUFFER, det.cs->vpc.size() * sizeof(float), det.cs->vpc.data(), GL_DYNAMIC_DRAW);
	//std::cout << "Past det vbo data" << std::endl;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	/*if(det.VBO != 0)
	{
		std::cout << "VBO done" << std::endl;
	}*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// std::cout << "obj size" << obj.objects.size() << std::endl;
	// std::cout << "success" << std::endl;
	return 0;
}
void GL::DrawGrid(float prevx, float prevy, GLuint &grVAO, GLuint &grVBO) {
	GLint colorLoc = glGetUniformLocation(GL::shader->program_index, "uColor");
	GLint boolloc = glGetUniformLocation(GL::shader->program_index, "meshvgrid");

	/*   GLboolean enabled;
	glGetBooleanv(GL_CULL_FACE, &enabled);

	GLint mode;
	glGetIntegerv(GL_CULL_FACE_MODE, &mode);

	GLint front;
	glGetIntegerv(GL_FRONT_FACE, &front);

	std::cout
		<< "Cull enabled: " << enabled
		<< " mode: " << mode
		<< " front: " << front
		<< '\n';*/

	/*float minx= std::min({GL::gCameraDesc[0][0]-GL::gCamera[0],GL::gCameraDesc[1][0]-GL::gCamera[0], GL::gCameraDesc[2][0]-GL::gCamera[0], -GL::gCameraDesc[0][0]-GL::gCamera[0],-GL::gCameraDesc[1][0]-GL::gCamera[0], -GL::gCameraDesc[2][0]-GL::gCamera[0]});
	float maxx= std::max({GL::gCameraDesc[0][0]-GL::gCamera[0],GL::gCameraDesc[1][0]-GL::gCamera[0], GL::gCameraDesc[2][0]-GL::gCamera[0], -GL::gCameraDesc[0][0]-GL::gCamera[0],-GL::gCameraDesc[1][0]-GL::gCamera[0], -GL::gCameraDesc[2][0]-GL::gCamera[0]});
	float minz= std::min({GL::gCameraDesc[0][2]-GL::gCamera[2],GL::gCameraDesc[1][2]-GL::gCamera[2], GL::gCameraDesc[2][2]-GL::gCamera[2], -GL::gCameraDesc[0][2]-GL::gCamera[2],-GL::gCameraDesc[1][2]-GL::gCamera[2], -GL::gCameraDesc[2][2]-GL::gCamera[2]});
	float maxz= std::max({GL::gCameraDesc[0][2]-GL::gCamera[2],GL::gCameraDesc[1][2]-GL::gCamera[2], GL::gCameraDesc[2][2]-GL::gCamera[2], -GL::gCameraDesc[0][2]-GL::gCamera[2],-GL::gCameraDesc[1][2]-GL::gCamera[2], -GL::gCameraDesc[2][2]-GL::gCamera[2]});

	minx = std::roundf(minx * 100);
	maxx = std::roundf(maxx * 100);
	minz = std::roundf(minz * 100);

	maxz = std::roundf(maxz * 100);*/
	// std::cout << "1\n";
	glBindVertexArray(grVAO);

	// std::cout << "2\n";
	glBindBuffer(GL_ARRAY_BUFFER, grVBO);

	if (std::abs(prevx - GL::gCamera[0]) > 10e-2 || std::abs(prevy - GL::gCamera[2]) > 10e-2)
	{
		GL::gridvert.clear();
		int minx = std::floor(GL::gCamera[0] - 100) / 10;
		int maxx = std::floor(GL::gCamera[0] + 100) / 10;
		int minz = std::floor(GL::gCamera[2] - 100) / 10;
		int maxz = std::floor(GL::gCamera[2] + 100) / 10;
		for (int x = minx; x <= maxx; x++)
		{
			GL::gridvert.emplace_back(std::pair<float, float>{x * 10.f, minz * 10.f});
			GL::gridvert.emplace_back(std::pair<float, float>{x * 10.f, maxz * 10.f});
		}

		for (int y = minz; y <= maxz; y++)
		{
			GL::gridvert.emplace_back(std::pair<float, float>{minx * 10.f, y * 10.f});
			GL::gridvert.emplace_back(std::pair<float, float>{maxx * 10.f, y * 10.f});
		}
	}
	// std::cout << "vertices = " << gridvert.size() << std::endl;
	// std::cout << "3\n";
	glUniform1i(boolloc, GL_TRUE);
	glUniform3f(colorLoc, 1.f, 1.f, 1.f);
	glDisable(GL_DEPTH_TEST);
	
	glClear( GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, GL::gWindowWidth, GL::gWindowHeight);

	// std::cout << "4\n";
	glBufferData(GL_ARRAY_BUFFER, GL::gridvert.size() * sizeof(std::pair<float, float>), GL::gridvert.data(), GL_DYNAMIC_DRAW);
	// std::cout << "5\n";
	glDrawArrays(GL_LINES, 0, GL::gridvert.size());
	glEnable(GL_DEPTH_TEST);
};

void GL::Draw(SimDef &currentProj)
{

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	
	glClear( GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, GL::gWindowWidth, GL::gWindowHeight);
	GLint colorLoc = glGetUniformLocation(GL::shader->program_index, "uColor");
	GLint boolloc = glGetUniformLocation(GL::shader->program_index, "meshvgrid");

	// std::cout << "6\n";
	// glEnable(GL_CULL_FACE);
	for (size_t i{}; i < currentProj.action.size(); i++)
	{

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		glUniform1i(boolloc, GL_FALSE);
		// std::cout << colorLoc << std::endl;
		glUniform3f(colorLoc, 0.75f, 0.75f, 0.75f);
		// std::cout << "7\n";
		glBindVertexArray(currentProj.action[i].VAO);

		// glCullFace(GL_BACK);
		// GL_CHECK();
		// glFrontFace(GL_CCW);
		// GL_CHECK();
		// std::cout << "8\n";
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentProj.action[i].EBO);
		// std::cout << "9\n";
		glDrawElements(GL_TRIANGLES, currentProj.action[i].cs->triangles.size() * 3, GL_UNSIGNED_INT, nullptr);
		// std::cout << colorLoc << std::endl;
		glUniform3f(colorLoc, 0.f, 0.f, 0.f);
		glDisable(GL_POLYGON_OFFSET_FILL);
		// glDisable(GL_DEPTH_TEST);
		// std::cout << "10\n";
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentProj.action[i].lEBO);
		glLineWidth(2.0f);
		// std::cout << "11\n";
		glDrawElements(GL_LINES, currentProj.action[i].cs->edge.size(), GL_UNSIGNED_INT, nullptr);

		/*float minx= std::min({GL::gCameraDesc[0][0]-GL::gCamera[0],GL::gCameraDesc[1][0]-GL::gCamera[0], GL::gCameraDesc[2][0]-GL::gCamera[0], -GL::gCameraDesc[0][0]-GL::gCamera[0],-GL::gCameraDesc[1][0]-GL::gCamera[0], -GL::gCameraDesc[2][0]-GL::gCamera[0]});
		float maxx= std::max({GL::gCameraDesc[0][0]-GL::gCamera[0],GL::gCameraDesc[1][0]-GL::gCamera[0], GL::gCameraDesc[2][0]-GL::gCamera[0], -GL::gCameraDesc[0][0]-GL::gCamera[0],-GL::gCameraDesc[1][0]-GL::gCamera[0], -GL::gCameraDesc[2][0]-GL::gCamera[0]});
		float minz= std::min({GL::gCameraDesc[0][2]-GL::gCamera[2],GL::gCameraDesc[1][2]-GL::gCamera[2], GL::gCameraDesc[2][2]-GL::gCamera[2], -GL::gCameraDesc[0][2]-GL::gCamera[2],-GL::gCameraDesc[1][2]-GL::gCamera[2], -GL::gCameraDesc[2][2]-GL::gCamera[2]});
		float maxz= std::max({GL::gCameraDesc[0][2]-GL::gCamera[2],GL::gCameraDesc[1][2]-GL::gCamera[2], GL::gCameraDesc[2][2]-GL::gCamera[2], -GL::gCameraDesc[0][2]-GL::gCamera[2],-GL::gCameraDesc[1][2]-GL::gCamera[2], -GL::gCameraDesc[2][2]-GL::gCamera[2]});

		minx = std::roundf(minx * 100);
		maxx = std::roundf(maxx * 100);
		minz = std::roundf(minz * 100);
		maxz = std::roundf(maxz * 100);*/
		// glEnable(GL_DEPTH_TEST);
		// std::cout << "12\n";
	}
	glDisable(GL_CULL_FACE);
}
