#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

