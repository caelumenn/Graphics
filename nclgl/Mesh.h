#pragma once
#include "../../nclgl/OGLRenderer.h"
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, TANGENT_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	static Mesh* GenerateTriangle();
	static Mesh* GenerateCube();
	void SetTexture(GLuint tex) {texture = tex;}
	GLuint GetTexture() {return texture;}
	static Mesh* GenerateQuad();

	void SetBumpMap(GLuint tex) {bumpTexture = tex;}
	GLuint GetBumpMap() {return bumpTexture;}
protected:
	void BufferData();
	void GenerateNormals();
	Vector3* normals;

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	GLuint numIndices;
	unsigned int* indices;

	Vector3* vertices;
	Vector4* colours;

	GLuint texture;
	Vector2* textureCoords;

	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3& a, const Vector3& b, const Vector3& c, const Vector2& ta, const Vector2& tb, const Vector2& tc);
	Vector3* tangents;
	GLuint bumpTexture;

};