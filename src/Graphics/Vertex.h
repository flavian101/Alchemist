#pragma once
#include <DirectXMath.h>

using namespace DirectX;
enum class VertexType {
    Pos3Col,
    Pos3Tex,
    Pos3TexNorm,
    pos3TexNormTan,
};

class Vertex
{
public:
    union
    {
        struct Pos3Col
        {
            
            XMFLOAT3 pos;
            XMFLOAT4 col;
            
        } pos3Col;

        struct Pos3TexCol
        {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
        } pos3Tex;
        struct Pos3TexNorm
        {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
            XMFLOAT3 norm;
        }pos3TexNorm;
        struct Pos3TexNormTan
        {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
            XMFLOAT3 norm;
            XMFLOAT3 tangent;
        }pos3TexNormTan;
    };
    Vertex()
    {}
    Vertex(float x, float y, float z, float r, float g, float b):type(VertexType::Pos3Col)
    {
        pos3Col.pos = XMFLOAT3(x, y, z);
       // pos3Col.padding1 = 0.0f; // Initialize padding
        pos3Col.col = XMFLOAT4(r, g, b, 1.0f);
    }

    Vertex(float x, float y,float z, float u, float v):type(VertexType::Pos3Tex)
    {
        pos3Tex.pos = XMFLOAT3(x, y, z);
        pos3Tex.tex = XMFLOAT2(u, v);
    }
    Vertex(float x, float y, float z, float u, float v,float xn, float yn, float zn) :type(VertexType::Pos3TexNorm)
    {
        pos3TexNorm.pos = XMFLOAT3(x, y, z);
        pos3TexNorm.tex = XMFLOAT2(u, v);
        pos3TexNorm.norm = XMFLOAT3(xn, yn, zn);
    }

    Vertex(float x, float y, float z, float u, float v, float xn, float yn,
        float zn, float tx, float ty, float tz) :type(VertexType::pos3TexNormTan)
    {
        pos3TexNorm.pos = XMFLOAT3(x, y, z);
        pos3TexNorm.tex = XMFLOAT2(u, v);
        pos3TexNorm.norm = XMFLOAT3(xn, yn, zn);
        pos3TexNormTan.tangent = (XMFLOAT3(tx, ty, tz));
    }

    // Ensure proper destruction of union members
    ~Vertex() {}

    VertexType type;
};



