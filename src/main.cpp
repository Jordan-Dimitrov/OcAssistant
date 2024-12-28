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
#include <chrono>

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
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OcAssistant", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
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
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniScale = glGetUniformLocation(shaderProgram.ID, "scale");
    GLuint uniTranslation = glGetUniformLocation(shaderProgram.ID, "translation");

    std::string texPath = "src/Resources/";
    Texture ryuk((texPath + "ryuk.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    ryuk.texUnit(shaderProgram, "tex0", 0);

    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (!glfwWindowShouldClose(window))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        frameCount++;
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count() >= 1.0)
        {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        glUniform1f(uniScale, 0.5f);
        ryuk.Bind();
        VAO1.Bind();

        const int objectCount = 10000000;
        for (int i = 0; i < objectCount; ++i)
        {
            float xOffset = (i % 100) * 0.02f - 1.0f;
            float yOffset = (i / 100) * 0.02f - 1.0f;

            glUniform2f(uniTranslation, 0, 0);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    ryuk.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
