#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <nvapi.h>
#include "../GPUInfo.h"

int main()
{
	assert(glfwInit() == 1);

	GLFWwindow* window = glfwCreateWindow(300, 420, "Toma", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	int gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	GpuInfo info = GetGpuInfo();

	std::cout << "GPU Name: " << info.name << std::endl;
	std::cout << "Core Count: " << info.coreCount << std::endl;
	std::cout << "VR Ready: " << (info.isVrReady ? "Yes" : "No") << std::endl;
	std::cout << "Power Utilization: " << info.powerUtilization << "%" << std::endl;
	std::cout << "Memory Size: " << info.memorySizeMB << " MB" << std::endl;
	std::cout << "GPU Clock: " << info.gpuClockKHz << " kHz" << std::endl;
	std::cout << "VRAM Clock: " << info.vramClockKHz << " kHz" << std::endl;

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
