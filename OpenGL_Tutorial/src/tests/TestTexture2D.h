#pragma once
#include "Test.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D() ;
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		VertexArray m_VA;
		VertexBuffer* m_VB1;
		VertexBuffer* m_VB2;
		IndexBuffer* m_IB;
		Shader* m_Shader1;
		Shader* m_Shader2;
		Texture* m_Texture1;
		Texture* m_Texture2;
		glm::vec3* m_View;
		glm::vec3* m_Model1;
		glm::vec3* m_Model2;
		float* m_VBData1;
		float* m_VBData2;
		unsigned int* m_Indeces;
		Renderer m_Renderer;
	};
}
