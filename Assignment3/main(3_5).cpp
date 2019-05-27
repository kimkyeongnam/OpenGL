#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

void init();
void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

float rec_positions[] =		// �簢���� �׸��� ���� 6���� vertex(x, y)�� ���� ������ ���� 1���� �迭
{
	-0.5f, -0.5f,	// 0
	 0.5f, -0.5f,	// 1
	 0.5f,  0.5f,	// 2

	 0.5f,  0.5f,	// 3
	-0.5f,  0.5f,	// 4
	-0.5f, -0.5f	// 5
};

float tri_position[] =
{
	0,0.5f,
	-0.5f,-0.5f,
	0.5f,-0.5f,
	0,0.5f,
	-0.5f,-0.5f,
	0.5f,-0.5f
};

int state = 0;

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) // glfw ���̺귯�� �ʱ�ȭ
		return -1;

	window = glfwCreateWindow(512, 512, "assign03-5", NULL, NULL);	// window ����
	if (!window)	// window ���� ���� �� ���α׷� ����
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // window context ����

	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew ���̺귯�� �ʱ�ȭ �� �ʱ�ȭ ���� �� ���� �޼���
														// glew ���̺귯���� �ݵ�� window context ���� �� �ʱ�ȭ �ƴϸ� ����

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

	
	init();

	//// Ű���� �Է�
	glfwSetKeyCallback(window, myKeyCallback);

	///////////////////////////////////////////////////////////////////////////
	// window â�� ����� ������ ���ѷ��� 
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//////////////////////////////////////////////////////////////////////
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawArrays �Լ��� �ߺ��Ǵ� vertex�� �����ϴ� vertex ������ �����.
		// vertex�� index�� �����ذ��� ����� �ƴ�,
		// vertex ������ ����ִ� positions �迭�� ���ʷ� �о� 3���� �ϳ��� �ﰢ������ ó���ϴ� ���.

	//////////////////////////////////////////////////////////////////////
		glfwSwapBuffers(window);	// front buffer�� back buffer ��ü
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void init()
{
	GLuint va;	// vertex array ����, �޸� �Ҵ�, ���ε�
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	GLuint vb;	// vertex buffer ����, �޸� �Ҵ�, ���ε�, data�� ������ ���Ŀ� �°� buffer data �غ�
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	if (!state)
		glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), rec_positions, GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tri_position, GL_STATIC_DRAW);
	// 6*2*sizeof(float) = (vertex ����)*(�� vertex�� ��� ����)*(�� ��� ������ Ÿ���� ũ��)

	GLuint program = InitShader("vshader(3_5).glsl", "fshader(3_5).glsl");	// shader program ��������
	glUseProgram(program);	// � shader program�� ����� ������

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex�� ���� ������ shader program ���� � ������ �����ų ���ΰ�
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Q/q: �簢�� �׸���
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		std::cout << "Key Q is pressed: Rectangle\n";
		state = 0;
		init();
	}

	// T/t: �ﰢ�� �׸���
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		std::cout << "Key T is pressed: Triangle\n";
		state = 1;
		init();
	}
}