#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

unsigned int width = 512;
unsigned int height = 512;

GLuint vao;
GLuint buffer;
GLuint simpleShader;

vec3* points;
vec3* colors;
int index = 0;
float theta = 0.0f;	// ȸ�� ��ų ����
float scale = 0.5f;	// ũ�� ����
float scaleUnit = 0.05f;	// while�� �ѹ� �������� �󸶳� Ŀ���� �󸶳� �۾��������� ���� ����

//////////////////////////////������ ���� ���� ������/////////////////////////////////
int rot = 0; // ȸ��
int tetra = 0; //���ü ����
int size = 0; //���ü ũ�� ����
int order = 1; // ���� ���� ����������
//////////////////////////////////////////////////////////////////////////////////////
int changecol = 0;


vec3 baseVertices[] =
{
	// ��
	vec3(0.0f, 0.85f, 0.0f),
	vec3(-0.6f, -0.45f, 0.0f),
	vec3(0.6f, -0.45f, 0.0f),
	
	//��
	vec3(0.0f, -0.85f, 0.0f),
	vec3(-0.6f, 0.45f, 0.0f),
	vec3(0.6f, 0.45f, 0.0f),
};

vec3 baseColors[] =
{
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(0.0f, 1.0f, 1.0f),
};

void init();
void display();
void idle();
void changeColor(unsigned int col1);
void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);	// scancode�� ������� ����, action�� Ű�� ����, mods�� ������� ����
void star(vec3 a, vec3 b, vec3 c, vec3 d, vec3 e, vec3 f, unsigned int coli);



int main()
{
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;

	window = glfwCreateWindow(width, height, "assign 03-2", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); /* Make the window's context current */

	if (glewInit() != GLEW_OK) std::cout << "Error\n";

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	init();

	//// Ű���� �Է�
	glfwSetKeyCallback(window, myKeyCallback);
	//glfwSetKeyCallback(window, myKeyCallback);

	///////////////////////////////////////////////////////////////////////////
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		display();
		idle();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	free(points);
	free(colors);
	return 0;
}

void star(vec3 a, vec3 b, vec3 c, vec3 d, vec3 e, vec3 f, unsigned int coli) 
{
	points[index] = a;	colors[index++] = baseColors[coli];
	points[index] = b;	colors[index++] = baseColors[coli];
	points[index] = c;	colors[index++] = baseColors[coli];
	points[index] = d;	colors[index++] = baseColors[coli];
	points[index] = e;	colors[index++] = baseColors[coli];
	points[index] = f;	colors[index++] = baseColors[coli];
}

void changeColor(unsigned int col1) 
{
	index = 0;

	star(baseVertices[0], baseVertices[1], baseVertices[2], baseVertices[3], baseVertices[4], baseVertices[5], col1);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);	// bind �׻� ���ִ� �� ����
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*6, points);	// point ������ �߰�
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*6, sizeof(vec3)*6, colors);	// color ������ �߰�
}

void init()
{
	points = (vec3*)malloc(sizeof(vec3) * 6);
	colors = (vec3*)malloc(sizeof(vec3) * 6);

	changeColor(changecol);
	// vao �ʱ�ȭ
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// buffer �ʱ�ȭ �� ������ �ε�
	glGenBuffers(1, &buffer);

	// ���� ������ ���
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*6, NULL, GL_STATIC_DRAW);	// ���ۿ� �����͸� �ִ� �Լ� //////////////
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*6, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * 6, sizeof(vec3) * 6, colors);

	simpleShader = InitShader("vshader(3_2).glsl", "fshader(3_2).glsl");
	glUseProgram(simpleShader);

	GLuint vPosition = glGetAttribLocation(simpleShader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(simpleShader, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3) * 6));	//BUFFER_OFFSET(sizeof(vec3*NumVertices))

	GLuint uTmp = glGetUniformLocation(simpleShader, "uTmp");
	glUniform1i(uTmp, 0);	//1���� int������ ������ ���� + ���ĺ� (���� + ����Ÿ��)
	// -> uTmp�� ���� ���� vShader.glsl ���Ͽ��� ���� �ٲ��� (if��)

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//csglViewport(0, 0, width / 2.0f, height / 2.0f);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUseProgram(simpleShader);

	mat4 rotation;
	if (rot == 1) rotation = RotateZ(theta); //4x4 ���
	if (rot == 2) rotation = RotateX(theta);
	if (rot == 3) rotation = RotateY(theta);

	mat4 scaling = Scale(scale);
	GLuint uMat = glGetUniformLocation(simpleShader, "uMat");
	glUniformMatrix4fv(uMat,
		1, // ��Ʈ������ ����
		GL_FALSE,
		rotation * scaling);	//4x4 ��Ʈ���� float Ÿ��, v�� ���

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void idle()
{
	
}

void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		std::cout << "Key 1 is pressed: Change Color 'Red'\n";
		changecol = 0;
		init();
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		std::cout << "Key 2 is pressed: Change Color 'Green'\n";
		changecol = 1;
		init();
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		std::cout << "Key 3 is pressed: Change Color 'Blue'\n";
		changecol = 2;
		init();
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		std::cout << "Key 4 is pressed: Change Color 'Yellow'\n";
		changecol = 3;
		init();
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		std::cout << "Key 5 is pressed: Change Color 'Blue'\n";
		changecol = 4;
		init();
	}
}