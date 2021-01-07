#define GLEW_STATIC

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
    /* GLFW stuffs */
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* GLEW stuffs */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // print GL version
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // Give a data to OpenGL
        float positions[] = {
            -0.5f,  -0.5f,
            0.5f,   -0.5f,
            0.5f,   0.5f,
            -0.5f,  0.5f
        };

        unsigned int indices[] = {
            0,  1,  2,
            2,  3,  0
        };

        VertexBuffer vb(positions, 4 * 2 * sizeof(float)); // glGenBuffers glBindBuffer glBufferData
        IndexBuffer ib(indices, 6); // glGenBuffers glBindBuffer glBufferData
        VertexBufferLayout layout;
        layout.Push<float>(2);


        VertexArray va;
        vb.Unbind();
        ib.Unbind();
        
        va.AddBuffer(vb, layout); // glEnableVertexAttribArray glVertexAttribPointer


        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.f, 1.f, 0.f, 1.f);

        Renderer renderer;
        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < -0.0f)
            {
                increment = 0.05f;
            }
            r += increment;


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}