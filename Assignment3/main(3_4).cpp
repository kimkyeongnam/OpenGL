#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include "vec.h"
#include "mat.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

GLuint va; // vertex array ����, �޸� �Ҵ�, ���ε�
GLuint buffer; // vertex buffer ����, �޸� �Ҵ�, ���ε�, data�� ������ ���Ŀ� �°� buffer data �غ�
GLuint program;

vec3* points;
vec3* colors;

int index = 0;
int cnt = 0;
int NumVertices;
float theta = 0;

void init();
void quad(vec3 a, vec3 b, vec3 c);
void allocateColor(vec3 a, vec3 b, vec3 c, unsigned int coli);
void setVertices();
void rectangle(vec3 a, vec3 b, vec3 c, vec3 d);
void display();
void idle();

vec3 baseVertices[] =
{
	vec3(-0.5f, -0.5f, 0.0f),
	vec3(-0.5f, 0.5f, 0.0f),//
	vec3(0.5f, 0.5f, 0.0f),//
	vec3(0.5f, -0.5f, 0.0f),
	vec3(-0.5f, -0.5f, -0.5f)
};

vec3 baseColors[] =
{
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.75f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 0.0f)
};

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) // glfw ���̺귯�� �ʱ�ȭ
		return -1;

	window = glfwCreateWindow(512, 512, "assign03-4", NULL, NULL);	// window ����
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

	///////////////////////////////////////////////////////////////////////////
	// window â�� ����� ������ ���ѷ��� 
	while (!glfwWindowShouldClose(window))
	{
		display();
		idle();

		glfwSwapBuffers(window);	// front buffer�� back buffer ��ü
		glfwPollEvents();
	}
	glfwTerminate();
	free(points);
	free(colors);
	return 0;
}

void allocateColor(vec3 a, vec3 b, vec3 c, unsigned int coli)
{
	points[index] = a;	colors[index++] = baseColors[coli];
	points[index] = b;	colors[index++] = baseColors[coli];
	points[index] = c;	colors[index++] = baseColors[coli];
}

void quad(vec3 a, vec3 b, vec3 c)
{
	float scale = 1.0f - 0.1f * cnt;
	printf("%.2f\n", scale);
	vec3 aa = vec3(a*scale);
	vec3 bb = vec3(b*scale);
	vec3 cc = vec3(c*scale);
	if (scale > 0.6f) {
		allocateColor(aa, bb, cc, cnt);
		cnt++;
		quad(a, b, c);
	}
	else {
		allocateColor(aa, bb, cc, cnt);
	}
}

void setVertices()
{
	rectangle(baseVertices[0], baseVertices[1], baseVertices[2], baseVertices[3]);
	
	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);	// bind �׻� ���ִ� �� ����
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);	// point ������ �߰�
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);	// color ������ �߰�
}

void rectangle(vec3 a, vec3 b, vec3 c, vec3 d)
{
	index = 0;
	quad(a, b, c);
	cnt = 0;
	quad(a, c, d);
}

void init()
{
	NumVertices = 4 * 10;

	points = (vec3*)malloc(sizeof(vec3) * NumVertices);
	colors = (vec3*)malloc(sizeof(vec3) * NumVertices);
	
	setVertices();

	
	// va �ʱ�ȭ
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	// ���ο� ���� ��ü id ����
	glGenBuffers(1, &buffer);
	 
	// ���� ��ü�� �޸� ������ �Ҵ�, vertex �����͸� ���� ��ü�� ����
	glBindBuffer(GL_ARRAY_BUFFER, buffer);	
	// ����: target, size, data, usage -> data�� ������ �޸𸮷κ��� sizeũ�⸸ŭ ����
	// usage:
	//          GL_STATIC_DRAW: �ѹ� vertex ������ ���ε� ��, ������ ���� ���
	//			GL_DYNAMIC_DRAW: �ִϸ��̼�ó�� vertex�����Ͱ� ���� �ٲ�� ��� ���. �ٲ� �� �ٽ� ���ε�
	//			GL_STREAM_DRAW: ����� �������̽�ó�� ����ؼ� vertex�����Ͱ� ����Ǵ� ���. �ٲ� �� �ٽ� ���ε�
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);	// ���ۿ� �����͸� �ִ� �Լ� //////////////
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * NumVertices, sizeof(vec3) * NumVertices, colors);	
	// 6*2*sizeof(float) = (vertex ����)*(�� vertex�� ��� ����)*(�� ��� ������ Ÿ���� ũ��)

	program = InitShader("vshader(3_4).glsl", "fshader(3_4).glsl");	// shader program ��������
	glUseProgram(program);	// � shader program�� ����� ������

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(
		vColor, //
		3,  //size
		GL_FLOAT, //type
		GL_FALSE,  //normalized
		0, //stride
		BUFFER_OFFSET(sizeof(vec3) * NumVertices));	


	GLuint uTmp = glGetUniformLocation(program, "uTmp");
	glUniform1i(uTmp, 0);	//1���� int������ ������ ���� + ���ĺ� (���� + ����Ÿ��)
	// -> uTmp�� ���� ���� vShader.glsl ���Ͽ��� ���� �ٲ��� (if��)

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	//csglViewport(0, 0, width / 2.0f, height / 2.0f);

	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glUseProgram(program);

	
	mat4 rotation = RotateY(theta);

	GLuint uMat = glGetUniformLocation(program, "uMat");
	glUniformMatrix4fv(uMat,
		1, // ��Ʈ������ ����
		GL_FALSE,
		rotation);	//4x4 ��Ʈ���� float Ÿ��, v�� ���

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);	
}

void idle()
{
	theta += 0.05f;
}