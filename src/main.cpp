#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <nvapi.h>

int main()
{
	assert(glfwInit() == 1);

	GLFWwindow* window = glfwCreateWindow(300, 420, "Toma", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	if (NvAPI_Initialize() != NVAPI_OK) {
		return -1;
	}

	NvU32 gpuCount = 0;
	NvPhysicalGpuHandle gpuHandles[32];

	if (NvAPI_EnumPhysicalGPUs(gpuHandles, &gpuCount) != NVAPI_OK || gpuCount == 0) {
		NvAPI_Unload();
		return -1;
	}

	std::cout << "GPUS: " << gpuCount << std::endl;

	for (NvU32 i = 0; i < gpuCount; ++i) {
		NvPhysicalGpuHandle gpuHandle = gpuHandles[i];

		NvAPI_ShortString gpuName;
		if (NvAPI_GPU_GetFullName(gpuHandle, gpuName) == NVAPI_OK) {
			std::cout << "GPU " << i + 1 << " Name: " << gpuName << std::endl;
		}
	}

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
