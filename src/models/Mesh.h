#pragma once
#include "MeshParts.h"
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh
{
public:
    Mesh(Graphics& g);

    void AddMeshPart(MeshParts parts);
    void Bind();
    void Render();


private:
    Graphics& m_graphics;
    std::vector<MeshParts> m_meshParts;
};
