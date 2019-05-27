#version 330	// ������ �˷��ֱ�

in vec4 vPosition;	// in ���ؽ� ���� ������ �� ��� (�Ӹ��� v)
in vec4 vColor;

uniform int uTmp;	// uniform �߰����� �۾��� �ϰ� �Ͱų� ������ �Ϸ��� �� �� ������ ������ (cpu���� ������ ��) (�Ӹ��� u)
uniform mat4 uMat;	// ��Ʈ����

out vec4 fColor;    // ������ ���� fragment shader�� �����ִ� ����

void main(){
	// gl_Position�� �̹� ������� �ִ� ��
	gl_Position = uMat * vPosition;	// ���ؽ� ������ ����

	if(uTmp == 0)	//�̷� ������..
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if(uTmp == 1)	
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(uTmp == 2)	
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
	else if(uTmp == 3)	
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(uTmp == 4)	
		fColor = vec4(0.0, 1.0, 0.0, 1.0);

	fColor = vColor;	// fragment�� out��Ű�� ����
}