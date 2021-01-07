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
#include "Texture.h"


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
            -0.5f,  -0.5f,  0.0f,   0.0f, // 0
            0.5f,   -0.5f,  1.0f,   0.0f, // 1
            0.5f,   0.5f,   1.0f,   1.0f, // 2
            -0.5f,  0.5f,   0.0f,   1.0f // 3
        };

        float positions2[] = {
            -0.25f,  -0.25f,  0.0f,   0.0f, // 0
            0.75f,   -0.25f,  1.0f,   0.0f, // 1
            0.75f,   0.75f,   1.0f,   1.0f, // 2
            -0.25f,  0.75f,   0.0f,   1.0f // 3
        };

        unsigned int indices[] = {
            0,  1,  2,
            2,  3,  0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexArray va2;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBuffer vb2(positions2, 4 * 4 * sizeof(float));
        IndexBuffer ib(indices, 6);
        VertexBufferLayout layout;

        layout.Push<float>(2); // for vertex coordinate
        layout.Push<float>(2); // for texture
        va.AddBuffer(vb, layout);
        va2.AddBuffer(vb2, layout);

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.f, 0.f, 0.f, 0.3f);
        Shader shader2("res/shaders/basic.shader");
        shader2.Bind();
        shader2.SetUniform4f("u_Color", 0.f, 0.f, 1.f, 0.3f);

        /*Texture texture("res/textures/javatwo.jpg");
        texture.Bind();
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);*/

        /*va.Unbind();
        vb.Unbind();
        vb2.Unbind();
        ib.Unbind();
        shader.Unbind();*/
        //texture.Unbind();

        Renderer renderer;
        /*float r = 0.0f;
        float increment = 0.05f;*/
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //texture.Bind();

            renderer.Draw(va, ib, shader);
            renderer.Draw(va2, ib, shader2);

            /*if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < -0.0f)
            {
                increment = 0.05f;
            }
            r += increment;*/


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}