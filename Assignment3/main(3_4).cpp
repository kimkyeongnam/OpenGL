#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include "vec.h"
#include "mat.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

GLuint va; // vertex array 선언, 메모리 할당, 바인드
GLuint buffer; // vertex buffer 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
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
	if (!glfwInit()) // glfw 라이브러리 초기화
		return -1;

	window = glfwCreateWindow(512, 512, "assign03-4", NULL, NULL);	// window 생성
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

	///////////////////////////////////////////////////////////////////////////
	// window 창이 종료될 때까지 무한루프 
	while (!glfwWindowShouldClose(window))
	{
		display();
		idle();

		glfwSwapBuffers(window);	// front buffer와 back buffer 교체
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
	glBindBuffer(GL_ARRAY_BUFFER, buffer);	// bind 항상 해주는 게 안전
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);	// point 데이터 추가
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);	// color 데이터 추가
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

	
	// va 초기화
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	// 새로운 버퍼 객체 id 생성
	glGenBuffers(1, &buffer);
	 
	// 버퍼 객체를 메모리 공간에 할당, vertex 데이터를 버퍼 객체에 복사
	glBindBuffer(GL_ARRAY_BUFFER, buffer);	
	// 인자: target, size, data, usage -> data로 지정된 메모리로부터 size크기만큼 복사
	// usage:
	//          GL_STATIC_DRAW: 한번 vertex 데이터 업로드 후, 변경이 없는 경우
	//			GL_DYNAMIC_DRAW: 애니메이션처럼 vertex데이터가 자주 바뀌는 경우 사용. 바뀔 시 다시 업로드
	//			GL_STREAM_DRAW: 사용자 인터페이스처럼 계속해서 vertex데이터가 변경되는 경우. 바뀔 시 다시 업로드
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);	// 버퍼에 데이터를 넣는 함수 //////////////
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * NumVertices, sizeof(vec3) * NumVertices, colors);	
	// 6*2*sizeof(float) = (vertex 갯수)*(각 vertex의 요소 갯수)*(각 요소 데이터 타입의 크기)

	program = InitShader("vshader(3_4).glsl", "fshader(3_4).glsl");	// shader program 가져오기
	glUseProgram(program);	// 어떤 shader program을 사용할 것인지

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
	glUniform1i(uTmp, 0);	//1개의 int정보를 보내줌 숫자 + 알파벳 (개수 + 변수타입)
	// -> uTmp의 값에 따라 vShader.glsl 파일에서 색을 바꿔줌 (if문)

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
		1, // 매트릭스의 개수
		GL_FALSE,
		rotation);	//4x4 매트릭스 float 타입, v는 행렬

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);	
}

void idle()
{
	theta += 0.05f;
}