#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>

int main()
{
	assert(glfwInit() == 1);

	GLFWwindow* window = glfwCreateWindow(300, 420, "Toma", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	int gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
