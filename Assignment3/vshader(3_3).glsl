#version 330	// 버전을 알려주기

in vec4 vPosition;	// in 버텍스 정보 가져올 때 사용 (머리말 v)
in vec4 vColor;

uniform int uTmp;	// uniform 추가적인 작업을 하고 싶거나 뭔가를 하려고 할 때 값들을 가져옴 (cpu에서 가져온 것) (머리말 u)
uniform mat4 uMat;	// 매트릭스

out vec4 fColor;    // 가져온 색을 fragment shader로 보내주는 변수

void main(){
	// gl_Position는 이미 만들어져 있는 것
	gl_Position = uMat * vPosition;	// 버텍스 포지션 지정

	if(uTmp == 0)	//이런 식으로..
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if(uTmp == 1)	
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(uTmp == 2)	
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
	else if(uTmp == 3)	
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(uTmp == 4)	
		fColor = vec4(0.0, 1.0, 0.0, 1.0);

	fColor = vColor;	// fragment로 out시키는 동작
}