#pragma once
#include "BufferLayout.h"
#include "Math/float3.h"
#include "Math/float2.h"

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;

	int boneIDs[4] = { -1, -1, -1, -1 };
	float weights[4];

	float3 tangents;
	float3 biTangents;
};

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int c);
	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();

	void SetData(const std::vector<Vertex>&);
	void SetData(float* vertices);
	void SetData(unsigned int size);
	void SetData(void* vertices, uint32_t size);


	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
	inline const BufferLayout& GetLayout() const { return layout; }

	// Make sure to do this BEFORE adding the VertexBuffer into the VertexArray.
	inline void SetLayout(const BufferLayout& l) { layout = l; }

private:
	unsigned int buffer;
	unsigned int count;

	BufferLayout layout;
};