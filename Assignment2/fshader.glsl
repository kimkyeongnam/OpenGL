#version 330	// 버전을 알려주기

in vec4 fColor;		// vertex shader에서 가져오는 것

out vec4 displayColor;

void main(){
	displayColor = fColor;
}