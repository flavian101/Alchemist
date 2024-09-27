#pragma once
#include "stdafx.h"

enum class VertexType {
    pos3,
    Pos3Col,
    Pos3Tex,
    Pos3TexNorm,
    Pos3TexNormTan,
};

class Vertex
{
public:
    union {
        XMFLOAT3 pos;           // Common position
        struct {
            XMFLOAT3 pos;
            XMFLOAT4 col;
        } posCol;

        struct {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
        } posTex;

        struct {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
            XMFLOAT3 norm;
        } posTexNorm;

        struct {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
            XMFLOAT3 norm;
            XMFLOAT3 tangent;
        } posTexNormTan;
    };

    // Type of vertex
    VertexType type;
    Vertex()
    {

    }
    // Constructors based on different layouts
    Vertex(float x, float y, float z) : type(VertexType::pos3)
    {
        pos = XMFLOAT3(x, y, z);
    }

    Vertex(float x, float y, float z, float r, float g, float b, float a) : type(VertexType::Pos3Col)
    {
        posCol.pos = XMFLOAT3(x, y, z);
        posCol.col = XMFLOAT4(r, g, b, a);
    }

    Vertex(float x, float y, float z, float u, float v) : type(VertexType::Pos3Tex)
    {
        posTex.pos = XMFLOAT3(x, y, z);
        posTex.tex = XMFLOAT2(u, v);
    }

    Vertex(float x, float y, float z, float u, float v, float xn, float yn, float zn) : type(VertexType::Pos3TexNorm)
    {
        posTexNorm.pos = XMFLOAT3(x, y, z);
        posTexNorm.tex = XMFLOAT2(u, v);
        posTexNorm.norm = XMFLOAT3(xn, yn, zn);
    }

    Vertex(float x, float y, float z, float u, float v, float xn, float yn, float zn, float tx, float ty, float tz)
        : type(VertexType::Pos3TexNormTan)
    {
        posTexNormTan.pos = XMFLOAT3(x, y, z);
        posTexNormTan.tex = XMFLOAT2(u, v);
        posTexNormTan.norm = XMFLOAT3(xn, yn, zn);
        posTexNormTan.tangent = XMFLOAT3(tx, ty, tz);
    }

    // Destructor
    ~Vertex() {}
};
