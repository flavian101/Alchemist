#pragma once
#include "MeshParts.h"
#include <vector>
#include <memory>
#include "Material.h"


class Mesh
{
public:
    Mesh(Graphics& g);

    void AddMeshPart(MeshParts parts);
    void Bind();
    void Render();
    void SetMaterial(std::unique_ptr<Material> material); // New method
    void controlWindow();


private:
    Graphics& m_graphics;
    std::vector<MeshParts> m_meshParts;
    std::unique_ptr<Material> m_material; 

};
