#include "TestTexture2D.h"

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestTexture2D::TestTexture2D()
        //: m_View(view), m_Model1(model1), m_Model2(model2),
        //m_Shader1(shaderPath), m_Shader2(shaderPath), m_Texture1(texturePath1), m_Texture2(texturePath2),

    {
        m_Model1 = new glm::vec3(0.0f, 0.0f, 0.0f);
        m_Model2 = new glm::vec3(0.0f, 0.0f, 0.0f);
        m_View = new glm::vec3(0.0f, 0.0f, 0.0f);

        m_VBData1 = new float[]{
            -50.0f,  -50.0f,  0.0f,   0.0f, // 0
            50.0f,   -50.0f,  1.0f,   0.0f, // 1
            50.0f,   50.0f,   1.0f,   1.0f, // 2
            -50.0f,  50.0f,   0.0f,   1.0f // 3
        };

        m_VBData2 = new float[] {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
                50.0f, -50.0f, 1.0f, 0.0f, // 1
                50.0f, 50.0f, 1.0f, 1.0f, // 2
                -50.0f, 50.0f, 0.0f, 1.0f // 3
        };
        m_Indeces = new unsigned int[] {
            0, 1, 2,
                2, 3, 0
        };

        m_VB1 = new VertexBuffer(m_VBData1, 4 * 4 * sizeof(float));
        m_VB2 = new VertexBuffer(m_VBData2, 4 * 4 * sizeof(float));
        m_IB = new IndexBuffer(m_Indeces, 6);

        VertexBufferLayout layout;

        layout.Push<float>(2); // for vertex coordinate
        layout.Push<float>(2); // for texture
        m_VA.AddBuffer(*m_VB1, layout);
        m_VA.AddBuffer(*m_VB2, layout);


        m_Shader1 = new Shader("res/shaders/basic.shader");
        m_Shader2 = new Shader("res/shaders/basic.shader");
        m_Texture1 = new Texture("res/textures/Gunter_in_Sign.png");
        m_Texture2 = new Texture("res/textures/javatwo.png");

        m_Shader1->Bind();
        m_Shader1->SetUniform4f("u_Color", 1.f, 0.f, 0.f, 0.3f);
        m_Shader1->Unbind();

        m_Shader2->Bind();
        m_Shader2->SetUniform4f("u_Color", 0.f, 0.f, 1.f, 0.3f);
        m_Shader2->Unbind();


        m_Texture1->Bind(0);
        m_Shader1->Bind();
        m_Shader1->SetUniform1i("u_Texture", 0);
        m_Texture1->Unbind();
        m_Shader1->Unbind();

        m_Texture2->Bind(0);
        m_Shader2->Bind();
        m_Shader2->SetUniform1i("u_Texture", 0);
        m_Texture2->Unbind();
        m_Shader2->Unbind();



	}
	TestTexture2D::~TestTexture2D()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        delete m_VB1;
        delete m_VB2;
        delete m_IB;
        delete m_Shader1;
        delete m_Shader2;
        delete m_Texture1;
        delete m_Texture2;
        delete m_View;
        delete m_Model1;
        delete m_Model2;
        delete[] m_Indeces;
        delete[] m_VBData1;
        delete[] m_VBData2;
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
        glm::mat4 proj = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 1.0f);

        const glm::mat4& view = glm::translate(glm::mat4(1.0f), *m_View);
        const glm::mat4& model1 = glm::translate(glm::mat4(1.0f), *m_Model1);
        const glm::mat4& model2 = glm::translate(glm::mat4(1.0f), *m_Model2);
        glm::mat4 mvp1 = proj * view * model1;
        glm::mat4 mvp2 = proj * view * model2;

        Renderer renderer;

        m_Shader1->Bind();
        m_Shader1->SetUniformMat4("u_MVP", mvp1);
        m_Shader2->Bind();
        m_Shader2->SetUniformMat4("u_MVP", mvp2);


        m_Texture1->Bind();
        m_Renderer.Draw(m_VA, *m_IB, *m_Shader1);
        m_Texture1->Unbind();

        m_Texture2->Bind();
        m_Renderer.Draw(m_VA, *m_IB, *m_Shader2);
        m_Texture2->Unbind();

	}
	void TestTexture2D::OnImGuiRender()
	{
        ImGui::Begin("Debuging Panel");                          
        ImGui::SliderFloat3("Model1 Translate", &m_Model1->x, -500.0f, 500.0f);
        ImGui::SliderFloat3("Model2 Translate", &m_Model2->x, -500.0f, 500.0f);
        ImGui::SliderFloat3("View Translate", &m_View->x, -500.0f, 500.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

	}
}

	
