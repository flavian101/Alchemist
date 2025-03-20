#include "ModelLoader.h"
#include "models/Vertex.h"
#include "Graphics/Graphics.h"
#include "Scene/Shaders/ShaderManager.h"
#include "Mesh.h"
#include "Node.h"


ModelLoader::ModelLoader(const std::string& filepath,Graphics& gfx)
	:Model(filepath)
{
    Assimp::Importer importer;
    basePath = std::filesystem::path(filepath).parent_path().string();

    const auto pScene = importer.ReadFile(filepath.c_str(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace
    );
    if (pScene == nullptr)
        return;
    meshPtrs.reserve(pScene->mNumMeshes);

    for (size_t i = 0; i < pScene->mNumMeshes; i++)
    {
        meshPtrs.emplace_back(parseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
    }
    int nextID = 0;
    pRoot = parseNode(nextID, *pScene->mRootNode);
}

std::unique_ptr<Mesh> ModelLoader::parseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{

    std::vector<unsigned short> m_indices;
    std::vector<Vertex> m_vertices;

    //reserve mesh space
    m_vertices.reserve(mesh.mNumVertices);
    m_indices.reserve(mesh.mNumFaces * 3);

  
    // Process mesh vertices
    for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
        XMFLOAT3 position;
        position.x = mesh.mVertices[i].x;
        position.y = mesh.mVertices[i].y;
        position.z = mesh.mVertices[i].z;

        XMFLOAT3 normal = { 0.0f, 0.0f, 0.0f };
        if (mesh.HasNormals()) {
            normal.x = mesh.mNormals[i].x;
            normal.y = mesh.mNormals[i].y;
            normal.z = mesh.mNormals[i].z;
        }

        XMFLOAT2 texCoord = { 0.0f, 0.0f };
        if (mesh.mTextureCoords[0]) {
            texCoord.x = mesh.mTextureCoords[0][i].x;
            texCoord.y = mesh.mTextureCoords[0][i].y;
        }

        XMFLOAT3 tangent = { 0.0f, 0.0f, 0.0f };
        if (mesh.HasTangentsAndBitangents()) {
            tangent.x = mesh.mTangents[i].x;
            tangent.y = mesh.mTangents[i].y;
            tangent.z = mesh.mTangents[i].z;
        }

        Vertex vertex;
        if (mesh.HasTangentsAndBitangents()) {
            vertex = Vertex(position.x, position.y, position.z, texCoord.x, texCoord.y, normal.x, normal.y, normal.z, tangent.x, tangent.y, tangent.z);
        }
        else if (mesh.HasNormals()) {
            vertex = Vertex(position.x, position.y, position.z, texCoord.x, texCoord.y, normal.x, normal.y, normal.z);
        }
        else {
            vertex = Vertex(position.x, position.y, position.z, texCoord.x, texCoord.y);
        }

        m_vertices.emplace_back(vertex);
    }
    // Create and set the material
   


    // Process mesh indices
    m_indices.reserve(mesh.mNumFaces * 3);
    for (unsigned int i = 0; i < mesh.mNumFaces; i++)
    {
        const auto& face = mesh.mFaces[i];
        assert(face.mNumIndices == 3);
        m_indices.emplace_back(face.mIndices[0]);
        m_indices.emplace_back(face.mIndices[1]);
        m_indices.emplace_back(face.mIndices[2]);
    }
    // Create a new Mesh object
    auto newMesh = std::make_unique<Mesh>(gfx, m_indices, m_vertices);

    //material flags 
    bool hasDiffuseMap = false;
    bool hasNormalMap = false;
    bool hasMetallicMap = false;
    bool hasRoughnessMap = false;
    bool hasAOMap = false;
    bool hasSpecularMap = false;

    //process mesh materials 
    if (mesh.mMaterialIndex >= 0)
    {
        auto& mat = *pMaterials[mesh.mMaterialIndex];
        auto material = std::make_unique<Material>(gfx);
        if (LoadMaterialTextures(gfx, mat, aiTextureType_DIFFUSE, Material::TextureType::Albedo, *material))
        {
            material->HasAlbedo(true);
            hasDiffuseMap = true;
        }
        else
        {
            material->HasBaseColor(true);
            mat.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(material->materialBuffer.data.materialStruct.baseColor));
        }
        if (LoadMaterialTextures(gfx, mat, aiTextureType_NORMALS, Material::TextureType::Normal, *material))
        {
            material->HasNormal(true);
            hasNormalMap = true;
        }

        if (LoadMaterialTextures(gfx, mat, aiTextureType_METALNESS, Material::TextureType::Metallic, *material))
        {
            material->HasMetallic(true);
            hasMetallicMap = true;
        }
        else
        {
            //mat.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));

        }
        if (LoadMaterialTextures(gfx, mat, aiTextureType_DIFFUSE_ROUGHNESS, Material::TextureType::Roughness, *material))
        {
            material->HasRoughness(true);
            hasRoughnessMap = true;
        }
 
        if (LoadMaterialTextures(gfx, mat, aiTextureType_AMBIENT_OCCLUSION, Material::TextureType::AmbientOcclusion, *material))
        {
            material->HasAO(true);
            hasAOMap = true;

        }
    
        if (LoadMaterialTextures(gfx, mat, aiTextureType_SPECULAR, Material::TextureType::Specular, *material))
        {
            material->HasSpecularMap(true);
            hasSpecularMap = true;
        }
        else
        {
            mat.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(material->materialBuffer.data.materialStruct.specular));
        }
         newMesh->SetMaterial(std::move(material));
    }
  


    return newMesh;
}

std::unique_ptr<Node> ModelLoader::parseNode(int& nextId, const aiNode& node)
{
    const auto transform = XMMatrixTranspose(XMLoadFloat4x4(
        reinterpret_cast<const XMFLOAT4X4*>(&node.mTransformation)
    ));
    std::vector<Mesh*> curMeshPtrs;
    curMeshPtrs.reserve(node.mNumMeshes);
    for (size_t i = 0; i < node.mNumMeshes; i++)
    {
        const auto meshIdx = node.mMeshes[i];
        curMeshPtrs.emplace_back(meshPtrs.at(meshIdx).get());
    }

    auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
    for (size_t i = 0; i < node.mNumChildren; i++)
    {
        pNode->AddChild(parseNode(nextId, *node.mChildren[i]));
    }

    return pNode;
}

bool ModelLoader::LoadMaterialTextures(Graphics& gfx,const aiMaterial& mat, aiTextureType type, Material::TextureType textureType, Material& material)
{
    bool hasTexture = false;
    for (UINT i = 0; i < mat.GetTextureCount(type); i++)
    {
        aiString str;
        mat.GetTexture(type, i, &str);
        std::string texturePath = basePath + "/" + str.C_Str();
        material.LoadTexture(gfx,textureType, texturePath);
        hasTexture = true;
    }
    return hasTexture;
}


void ModelLoader::Update(Graphics& gfx,float deltaTime)
{
    Model::Update(gfx,deltaTime);
    
}

void ModelLoader::Render(Graphics& gfx)
{
    Model::Render(gfx);
}
