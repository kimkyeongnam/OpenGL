#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

void init();
void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

float rec_positions[] =		// 사각형을 그리기 위한 6개의 vertex(x, y)에 대한 정보를 담은 1차원 배열
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
	if (!glfwInit()) // glfw 라이브러리 초기화
		return -1;

	window = glfwCreateWindow(512, 512, "assign03-5", NULL, NULL);	// window 생성
	if (!window)	// window 생성 실패 시 프로그램 종료
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // window context 생성

	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew 라이브러리 초기화 및 초기화 실패 시 에러 메세지
														// glew 라이브러리는 반드시 window context 생성 후 초기화 아니면 에러

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

	
	init();

	//// 키보드 입력
	glfwSetKeyCallback(window, myKeyCallback);

	///////////////////////////////////////////////////////////////////////////
	// window 창이 종료될 때까지 무한루프 
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//////////////////////////////////////////////////////////////////////
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawArrays 함수는 중복되는 vertex를 포함하는 vertex 정보를 사용함.
		// vertex의 index를 참조해가는 방식이 아닌,
		// vertex 정보가 들어있는 positions 배열을 차례로 읽어 3개씩 하나의 삼각형으로 처리하는 방식.

	//////////////////////////////////////////////////////////////////////
		glfwSwapBuffers(window);	// front buffer와 back buffer 교체
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void init()
{
	GLuint va;	// vertex array 선언, 메모리 할당, 바인드
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	GLuint vb;	// vertex buffer 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	if (!state)
		glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), rec_positions, GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tri_position, GL_STATIC_DRAW);
	// 6*2*sizeof(float) = (vertex 갯수)*(각 vertex의 요소 갯수)*(각 요소 데이터 타입의 크기)

	GLuint program = InitShader("vshader(3_5).glsl", "fshader(3_5).glsl");	// shader program 가져오기
	glUseProgram(program);	// 어떤 shader program을 사용할 것인지

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex에 대한 정보를 shader program 안의 어떤 변수와 연결시킬 것인가
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void myKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Q/q: 사각형 그리기
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		std::cout << "Key Q is pressed: Rectangle\n";
		state = 0;
		init();
	}

	// T/t: 삼각형 그리기
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		std::cout << "Key T is pressed: Triangle\n";
		state = 1;
		init();
	}
}