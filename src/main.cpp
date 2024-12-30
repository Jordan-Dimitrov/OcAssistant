#include <filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Headers/Texture.h"
#include "Headers/shaderClass.h"
#include "Headers/VAO.h"
#include "Headers/VBO.h"
#include "Headers/EBO.h"
#include <vector>
#include <thread>
#include <atomic>
#include <windows.h>

void handleCLIInput(std::atomic<bool>& stopFlag, std::atomic<bool>& runFlag, HANDLE hPipe) {
    char buffer[128];
    DWORD bytesRead;
    while (!stopFlag) {
        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            std::string input(buffer);
            if (input == "stop") {
                stopFlag = true;
            }
            else if (input == "start") {
                runFlag = true;
            }
        }
    }
    CloseHandle(hPipe);
}

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    1.0f, 0.0f
};

GLuint indices[] = {
    0, 2, 1,
    0, 3, 2
};

int main()
{
    std::atomic<bool> stopFlag(false);
    std::atomic<bool> runFlag(false);

    HANDLE hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\BenchmarkPipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create named pipe." << std::endl;
        return -1;
    }

    std::thread cliThread(handleCLIInput, std::ref(stopFlag), std::ref(runFlag), hPipe);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OcAssistant", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        stopFlag = true;
        cliThread.join();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 1920, 1080);

    Shader shaderProgram("src/Resources/default.vert", "src/Resources/default.frag");

    VAO VAO1;
    VAO1.Bind();
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    const int instanceCount = 9999999;
    std::vector<GLfloat> offsets(instanceCount * 2);
    for (int i = 0; i < instanceCount; ++i)
    {
        offsets[2 * i] = (i % 100) * 0.02f - 1.0f;
        offsets[2 * i + 1] = (i / 100) * 0.02f - 1.0f;
    }

    VBO VBO2(offsets.data(), offsets.size() * sizeof(GLfloat));
    VAO1.LinkAttrib(VBO2, 3, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(3, 1);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniScale = glGetUniformLocation(shaderProgram.ID, "scale");

    std::string texPath = "src/Resources/";
    Texture ryuk((texPath + "ryuk.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    ryuk.texUnit(shaderProgram, "tex0", 0);

    while (!glfwWindowShouldClose(window) && !stopFlag)
    {
        if (runFlag) {
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.Activate();
            glUniform1f(uniScale, 0.5f);
            ryuk.Bind();
            VAO1.Bind();

            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instanceCount);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    stopFlag = true;
    cliThread.join();

    VAO1.Delete();
    VBO1.Delete();
    VBO2.Delete();
    EBO1.Delete();
    ryuk.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}