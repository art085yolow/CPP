#include <iostream>
#include "chip8_unit.cpp"
#include "glad.h"
#include <GLFW/glfw3.h>

void framebuffer_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);
};


int main()
{
    std::cout<< "Hello chip8emu!!" << std::endl;

	if (!glfwInit()) {
		return -1;
	}
	
//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(800,600,"OpenGL Window CHIP 8 EMU", nullptr, nullptr);

	if(!window) {
		glfwTerminate();
	};

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
	};

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);

	glViewport(0,0,800,600);

	while (!glfwWindowShouldClose(window)) {
		// some staff
		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	};

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
