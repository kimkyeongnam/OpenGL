#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include "vec.h"
#include "mat.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

unsigned int width = 512;
unsigned int height = 512;

GLuint vao;
GLuint buffer;
GLuint simpleShader;

unsigned int NumSubdivides = 2;	// 면개수
unsigned int NumTriangles;	// 삼각형 개수
unsigned int NumVertices;	// 모든 정점 개수

vec3* points;
vec3* colors;
int index = 0;
float theta = 0.0f;	// 회전 시킬 각도
float scale = 0.5f;	// 크기 조절
float scaleUnit = 0.05f;	// while을 한번 돌때마다 얼마나 커지고 얼마나 작아질건지에 대한 변수

//////////////////////////////과제를 위해 만든 변수들/////////////////////////////////
int rot = 0; // 회전
int tetra = 0; //사면체 분할
int size = 0; //사면체 크기 조절
int order = 1; // 색상 순서 순차적으로
//////////////////////////////////////////////////////////////////////////////////////

vec3 baseVertices[] =
{
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.942809f, -0.333333f),//
	vec3(-0.816497f, -0.471405f, -0.333333f),//
	vec3(0.816497f, -0.471405f, -0.333333f)//
};

vec3 baseColors[] =
{
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.0f, 1.0f),
	vec3(0.75f, 0.75f, 0.75f)
};

void init();
void display();
void idle();
void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4);

void gasketKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);	// scancode는 사용하지 않음, action은 키의 상태, mods는 사용하지 않음
void gasketMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli);
void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli);


void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli) {


	if (cnt > 0) {
		// 각 선의 중간지점 계산
		vec3 ab = (a + b) / 2.0f;
		vec3 ac = (a + c) / 2.0f;
		vec3 bc = (b + c) / 2.0f;

		// 삼각형 3개로 분할 (recursive 하게)
		divide_triangle(a, ab, ac, cnt - 1, coli);
		divide_triangle(c, ac, bc, cnt - 1, coli);
		divide_triangle(b, bc, ab, cnt - 1, coli);

		//divide_triangle(ab, bc, bc, cnt - 1, 4);
	}
	else {
		triangle(a, b, c, coli);
	}
}

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli) {
	points[index] = a;	colors[index++] = baseColors[coli];
	points[index] = b;	colors[index++] = baseColors[coli];
	points[index] = c;	colors[index++] = baseColors[coli];
}

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4) {

	index = 0;	// init에서 이미 0으로 설정되기 때문에 불러올 때마다 항상 0으로 초기화 해야함

	divide_triangle(baseVertices[0], baseVertices[1], baseVertices[2], NumSubdivides, col1);
	divide_triangle(baseVertices[3], baseVertices[2], baseVertices[1], NumSubdivides, col2);
	divide_triangle(baseVertices[0], baseVertices[3], baseVertices[1], NumSubdivides, col3);
	divide_triangle(baseVertices[0], baseVertices[2], baseVertices[3], NumSubdivides, col4);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);	// bind 항상 해주는 게 안전
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);	// point 데이터 추가
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);	// color 데이터 추가
}

// 사각형 함수
void quad(vec3 a, vec3 b, vec3 c, vec3 d, unsigned int coli) {
	// triangle 두번을 호출해서 4개를 3개씩 묶어줌
	triangle(a, b, c, coli);
	triangle(a, c, d, coli);
}

// 정육면체 함수
void cube(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4, unsigned int col5, unsigned int col6) {
	// front of base Vectices
	quad(baseVertices[0], baseVertices[1], baseVertices[2], baseVertices[3], col1);
	quad(baseVertices[1], baseVertices[5], baseVertices[6], baseVertices[2], col2);
	quad(baseVertices[5], baseVertices[4], baseVertices[7], baseVertices[6], col3);
	quad(baseVertices[4], baseVertices[0], baseVertices[3], baseVertices[7], col4);
	quad(baseVertices[3], baseVertices[2], baseVertices[6], baseVertices[7], col5);
	quad(baseVertices[4], baseVertices[5], baseVertices[1], baseVertices[0], col6);
}

int main()
{
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;

	window = glfwCreateWindow(width, height, "HW #2: SW 17011654 KKN", NULL, NULL);
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

	//// 키보드 입력
	glfwSetKeyCallback(window, gasketKeyCallback);
	//glfwSetKeyCallback(window, gasketKeyCallback);
	//// 마우스 버튼 입력
	glfwSetMouseButtonCallback(window, gasketMouseButtonCallback);

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

void init()
{
	NumTriangles = 4 * pow(4.0f, NumSubdivides);
	NumVertices = 3 * NumTriangles; // 삼각형마다 버텍스 3개 * 삼각형 개수

	points = (vec3*)malloc(sizeof(vec3)*NumVertices);
	colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

	setGasketVertices(0, 1, 2, 3);


	// vao 초기화
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// buffer 초기화 및 데이터 로드
	glGenBuffers(1, &buffer);

	//회색
	triangle(baseVertices[0], baseVertices[1], baseVertices[2], 4);
	triangle(baseVertices[3], baseVertices[2], baseVertices[1], 4);
	triangle(baseVertices[0], baseVertices[3], baseVertices[1], 4);
	triangle(baseVertices[0], baseVertices[2], baseVertices[3], 4);

	// 전달 데이터 명시
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);	// 버퍼에 데이터를 넣는 함수 //////////////
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * NumVertices, sizeof(vec3) * NumVertices, colors);	

	simpleShader = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(simpleShader);

	GLuint vPosition = glGetAttribLocation(simpleShader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(simpleShader, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3) * NumVertices));	//BUFFER_OFFSET(sizeof(vec3*NumVertices))

	GLuint uTmp = glGetUniformLocation(simpleShader, "uTmp");
	glUniform1i(uTmp, 0);	//1개의 int정보를 보내줌 숫자 + 알파벳 (개수 + 변수타입)
	// -> uTmp의 값에 따라 vShader.glsl 파일에서 색을 바꿔줌 (if문)

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
	if (rot == 1) rotation = RotateZ(theta); //4x4 행렬
	if (rot == 2) rotation = RotateX(theta);
	if (rot == 3) rotation = RotateY(theta);

	mat4 scaling = Scale(scale);
	GLuint uMat = glGetUniformLocation(simpleShader, "uMat");
	glUniformMatrix4fv(uMat,
		1, // 매트릭스의 개수
		GL_FALSE,
		rotation * scaling);	//4x4 매트릭스 float 타입, v는 행렬

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle()
{
	// 회전(x,y,z축 회전은 display()에서 결정)
	theta += 0.05f;


	// 사면체 크기
	if (size == 1) {
		scale += scaleUnit;
		std::cout << "Current size: " << scale << std::endl;
	}
	if (size == -1) {
		if (scale < 0.1f) { //0.1f 크기 이하로는 크기감소x
			std::cout << "Min size" << std::endl;
			size = 0;
			return;
		}
		scale -= scaleUnit;
		std::cout << "Current size: " << scale << std::endl;
	}
	size = 0;
}

void gasketKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Q/q: 프로그램 종료
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		std::cout << "Key Q is pressed: Exit\n";
		glfwSetWindowShouldClose(window, GL_TRUE);
	}



	// +: 사면체 크기 증가
	if ( (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL) && action == GLFW_PRESS) {
		std::cout << "Key + is pressed: Enlarge Tetrahedron, ";
		size = 1;
	}
	
	// -: 사면체 크기 감소(음수로 떨어지는거 방지는 idle()에서 구현)
	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		std::cout << "Key - is pressed: Shrink Tetrahedron, ";
		size = -1;
	}



	// Key1 ~ 4: 색상 변경
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		std::cout << "Key 1 is pressed: Change Color 1\n";
		setGasketVertices(1, 2, 3, 0);
		order = 2;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		std::cout << "Key 2 is pressed: Change Color 2\n";
		setGasketVertices(2, 3, 0, 1);
		order = 3;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		std::cout << "Key 3 is pressed: Change Color 3\n";
		setGasketVertices(3, 0, 1, 2);
		order = 4;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		std::cout << "Key 4 is pressed: Change Color 4 (default)\n";
		setGasketVertices(0, 1, 2, 3);
		order = 1;
	}



	// U/u: 사면체 분할 횟수 증가
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		std::cout << "Key U is pressed: Increase Tetrahedron, Number of division: ";
		NumSubdivides++;
		std::cout << NumSubdivides << std::endl;
		init();
	}

	// D/d: 사면체 분할 횟수 감소
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		std::cout << "Key D is pressed: Decrease tetrahedron, Number of division: ";
		if (NumSubdivides > 1) {
			NumSubdivides--;
			init();
			std::cout << NumSubdivides;
			if (NumSubdivides == 1) std::cout << "(Min)\n";
			else std::cout << std::endl;
		}
		else if(NumSubdivides==1)
			std::cout << "1(Min)\n";
	}

}

void gasketMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	// Left: 색 순차적으로 변경
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "Mouse left button is pressed, ";
		switch (order) {
		case 1:
			std::cout << "Current color: Key 1 color\n";
			setGasketVertices(1, 2, 3, 0);
			order = 2;
			break;
		case 2:
			std::cout << "Current color: Key 2 color\n";
			setGasketVertices(2, 3, 0, 1);
			order = 3;
			break;
		case 3:
			std::cout << "Current color: Key 3 color\n";
			setGasketVertices(3, 0, 1, 2);
			order = 4;
			break;
		case 4:
			std::cout << "Current color: Key 4 color\n";
			setGasketVertices(0, 1, 2, 3);
			order = 1;
			break;
		default: 
			break;
		}
	}

	// Right: 회전
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		switch (rot){// 기존의 rot값을 참고하여 변경
		case 0:
			std::cout << "Mouse Right button is pressed: Rotate axis Z\n";
			rot++;
			break;
		case 1:
			std::cout << "Mouse Right button is pressed: Rotate axis X\n";
			rot++;
			break;
		case 2:
			std::cout << "Mouse Right button is pressed: Rotate axis Y\n";
			rot++;
			break;
		case 3:
			std::cout << "Mouse Right button is pressed: No Rotate (default)\n";
			rot = 0; //default값으로 변경
			break;
		default: break;
		}
	}
}
