#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

int main()
{
	glewInit();
	GLFWwindow* window;
	if (!glfwInit()) /* Initialize the library */
		return -1;
	window = glfwCreateWindow(640, 480, "Rectangle", NULL, NULL);
	glfwMakeContextCurrent(window); /* Make the window's context current */
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_QUADS);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
		glEnd();
		glfwSwapBuffers(window);	/* Swap front and back buffers */
		glfwPollEvents();	/* process events */
	}
	glfwTerminate();
	return 0;
}