#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Vertex
{
public:
    union
    {
        struct Pos3Col
        {
            XMFLOAT3 pos;
            XMFLOAT4 col;
           // float padding1; // Add padding to ensure consistent size
        } pos3Col;

        struct Pos2TexCol
        {
            XMFLOAT3 pos;
            XMFLOAT2 tex;
        } pos3Tex;

        //more
    };

    Vertex(float x, float y, float z, float r, float g, float b)
    {
        pos3Col.pos = XMFLOAT3(x, y, z);
       // pos3Col.padding1 = 0.0f; // Initialize padding
        pos3Col.col = XMFLOAT4(r, g, b, 1.0f);
    }

    Vertex(float x, float y,float z, float u, float v)
    {
        pos3Tex.pos = XMFLOAT3(x, y, z);
        pos3Tex.tex = XMFLOAT2(u, v);
    }

    // Ensure proper destruction of union members
    ~Vertex() {}
};
