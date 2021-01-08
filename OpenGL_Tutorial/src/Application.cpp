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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

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
    window = glfwCreateWindow(1280, 720, "Hello OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 150";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


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
        // position coord, texture coord
        float positions[] = {
            -50.0f,  -50.0f,  0.0f,   0.0f, // 0
            50.0f,   -50.0f,  1.0f,   0.0f, // 1
            50.0f,   50.0f,   1.0f,   1.0f, // 2
            -50.0f,  50.0f,   0.0f,   1.0f // 3
        };

        float positions2[] = {
            -50.0f,  -50.0f,  0.0f,   0.0f, // 0
            50.0f,   -50.0f,  1.0f,   0.0f, // 1
            50.0f,   50.0f,   1.0f,   1.0f, // 2
            -50.0f,  50.0f,   0.0f,   1.0f // 3
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

        //glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
        //const glm::mat4& view = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 0.0f, 0.0f));
        //const glm::mat4& model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -200.0f, 0.0f));
        //glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.f, 0.f, 0.f, 0.3f);
        shader.Unbind();

        Shader shader2("res/shaders/basic.shader");
        shader2.Bind();
        shader2.SetUniform4f("u_Color", 0.f, 0.f, 1.f, 0.3f);
        shader2.Unbind();


        Texture texture("res/textures/Gunter_in_Sign.png");
        texture.Bind(0); // glActiveTexture glBindTexture
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);
        texture.Unbind(); // glActiveTexture glBindTexture
        shader.Unbind();

        Texture texture2("res/textures/javatwo.png");
        texture2.Bind(0); // glActiveTexture glBindTexture
        shader2.Bind();
        shader2.SetUniform1i("u_Texture", 0);
        texture2.Unbind(); // glActiveTexture glBindTexture
        shader2.Unbind();

        va.Unbind();
        vb.Unbind();
        shader.Unbind();
        texture.Unbind();
        va2.Unbind();
        vb2.Unbind();
        shader2.Unbind();
        texture2.Unbind();
        ib.Unbind();

        glm::vec3 view_translation(0.0f, 0.0f, 0.0f);
        glm::vec3 model1_translation(0.0f, 0.0f, 0.0f);
        glm::vec3 model2_translation(0.0f, 0.0f, 0.0f);

        Renderer renderer;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 proj = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 1.0f);
            const glm::mat4& view = glm::translate(glm::mat4(1.0f), view_translation);
            const glm::mat4& model1 = glm::translate(glm::mat4(1.0f), model1_translation);
            const glm::mat4& model2 = glm::translate(glm::mat4(1.0f), model2_translation);
            glm::mat4 mvp1 = proj * view * model1;
            glm::mat4 mvp2 = proj * view * model2;

            shader.Bind();
            shader.SetUniformMat4("u_MVP", mvp1);
            shader2.Bind();
            shader2.SetUniformMat4("u_MVP", mvp2);


            texture.Bind(); 
            renderer.Draw(va, ib, shader);
            texture.Unbind();

            texture2.Bind(); 
            renderer.Draw(va2, ib, shader2);
            texture2.Unbind();



            {
                ImGui::Begin("Debuging Panel");                          // Create a window called "Hello, world!" and append into it.
                ImGui::SliderFloat3("Model1 Translate", &model1_translation.x, -500.0f, 500.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("Model2 Translate", &model2_translation.x, -500.0f, 500.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                //ImGui::SliderFloat("Model Translate Y", &model_translation.y, -500.0f, 500.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("View Translate", &view_translation.x, -500.0f, 500.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                //ImGui::SliderFloat("View Translate Y", &view_translation.y, -500.0f, 500.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }


            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwTerminate();
    return 0;
}