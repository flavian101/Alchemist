#include "ModelLoader.h"
#include "models/Vertex.h"
#include "Graphics/Graphics.h"
#include "Scene/Shaders/ShaderManager.h"
#include "Mesh.h"

ModelLoader::ModelLoader(const std::string& filepath,Graphics& g, std::shared_ptr<ShaderManager> manager)
	:
    m_graphics(g),
    m_model(filepath,g,manager)
{
    //m_filepath = filepath;
    Assimp::Importer importer;
    basePath = std::filesystem::path(filepath).parent_path().string();

    const auto pScene = importer.ReadFile(filepath.c_str(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace
    );
    if (pScene == nullptr)
        return;

    // setName(pScene->mRootNode->mName.C_Str());
    ProcessNode(pScene->mRootNode, pScene, XMMatrixIdentity());
}



void ModelLoader::controlWindow()
{
    m_model.controlWindow();
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const DirectX::FXMMATRIX& parentTransform)
{
    // Load the node's transformation matrix (relative to its parent)
    XMMATRIX nodeLocalTransform = XMMatrixTranspose(XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&node->mTransformation)));
    
  //  // For debugging, print out nodeLocalTransform
  //  XMFLOAT4X4 localTransformDebug;
  //  XMStoreFloat4x4(&localTransformDebug, nodeLocalTransform);
  //  std::cout << "Node Local Transform: " << std::endl;
  //  std::cout << localTransformDebug._11 << " " << localTransformDebug._12 << " " << localTransformDebug._13 << " " << localTransformDebug._14 << std::endl;
  //  std::cout << localTransformDebug._21 << " " << localTransformDebug._22 << " " << localTransformDebug._23 << " " << localTransformDebug._24 << std::endl;
  //  std::cout << localTransformDebug._31 << " " << localTransformDebug._32 << " " << localTransformDebug._33 << " " << localTransformDebug._34 << std::endl;
  //  std::cout << localTransformDebug._41 << " " << localTransformDebug._42 << " " << localTransformDebug._43 << " " << localTransformDebug._44 << std::endl;

    // Calculate the accumulated transform
    XMMATRIX globalTransform = nodeLocalTransform * parentTransform ;

  //  // For debugging, print out accumulatedTransform
  //  XMFLOAT4X4 accumulatedTransformDebug;
  //  XMStoreFloat4x4(&accumulatedTransformDebug, accumulatedTransform);
  //  std::cout << "Accumulated Transform: " << std::endl;
  //  std::cout << accumulatedTransformDebug._11 << " " << accumulatedTransformDebug._12 << " " << accumulatedTransformDebug._13 << " " << accumulatedTransformDebug._14 << std::endl;
  //  std::cout << accumulatedTransformDebug._21 << " " << accumulatedTransformDebug._22 << " " << accumulatedTransformDebug._23 << " " << accumulatedTransformDebug._24 << std::endl;
  //  std::cout << accumulatedTransformDebug._31 << " " << accumulatedTransformDebug._32 << " " << accumulatedTransformDebug._33 << " " << accumulatedTransformDebug._34 << std::endl;
  //  std::cout << accumulatedTransformDebug._41 << " " << accumulatedTransformDebug._42 << " " << accumulatedTransformDebug._43 << " " << accumulatedTransformDebug._44 << std::endl;

    // Process meshes attached to this node
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh* processedMesh = ProcessMesh(mesh, scene, globalTransform);
        // Set the mesh's transform relative to the accumulated transform
        // Add the mesh to the model
        m_model.AddMesh(processedMesh);
    }

    // Recursively process each child node
    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, globalTransform);
    }

}

Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::FXMMATRIX& nodeTransform)
{
    std::vector<unsigned short> m_indices;
    std::vector<Vertex> m_vertices;

    // Get the vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        // Transform the vertex position
        XMFLOAT3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        XMVECTOR posVec = XMLoadFloat3(&pos);
        posVec = XMVector3Transform(posVec, nodeTransform);
        XMFLOAT3 transformedPos;
        XMStoreFloat3(&transformedPos, posVec);

        // Transform the normal if it exists
        XMFLOAT3 transformedNormal;
        if (mesh->HasNormals())
        {
            XMFLOAT3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            XMVECTOR normalVec = XMLoadFloat3(&normal);
            normalVec = XMVector3TransformNormal(normalVec, nodeTransform);
            XMStoreFloat3(&transformedNormal, XMVector3Normalize(normalVec));

        }

        // Transform the tangent if it exists
        XMFLOAT3 transformedTangent;
        if (mesh->HasTangentsAndBitangents())
        {
            XMFLOAT3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            XMVECTOR tangentVec = XMLoadFloat3(&tangent);
            tangentVec = XMVector3TransformNormal(tangentVec, nodeTransform);
            XMStoreFloat3(&transformedTangent, XMVector3Normalize(tangentVec));

        }

        if (mesh->HasNormals() && mesh->HasTextureCoords(0) && mesh->HasTangentsAndBitangents())
        {
            m_vertices.emplace_back(
                transformedPos.x, transformedPos.y, transformedPos.z,
                mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
                transformedNormal.x, transformedNormal.y, transformedNormal.z,
                transformedTangent.x, transformedTangent.y, transformedTangent.z
            );
        }
        else if (mesh->HasNormals() && mesh->HasTextureCoords(0))
        {
            m_vertices.emplace_back(
                transformedPos.x, transformedPos.y, transformedPos.z,
                mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
                transformedNormal.x, transformedNormal.y, transformedNormal.z
            );
        }
        else if (mesh->HasTextureCoords(0))
        {
            m_vertices.emplace_back(
                transformedPos.x, transformedPos.y, transformedPos.z,
                mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y
            );
        }
        else
        {
            m_vertices.emplace_back(
                transformedPos.x, transformedPos.y, transformedPos.z
            );
        }
    }

    // Get the indices
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            m_indices.push_back(face.mIndices[j]);
        }
    }

    // Create a new Mesh object
    auto* newMesh = new Mesh(m_graphics, m_indices, m_vertices);

    // Create and set the material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        auto material = std::make_shared<Material>(m_graphics);
        material->HasAlbedo(LoadMaterialTextures(mat, aiTextureType_DIFFUSE, Material::TextureType::Albedo, *material));
        material->HasNormal(LoadMaterialTextures(mat, aiTextureType_NORMALS, Material::TextureType::Normal, *material));
        material->HasMetallic(LoadMaterialTextures(mat, aiTextureType_METALNESS, Material::TextureType::Metallic, *material));
        material->HasRoughness(LoadMaterialTextures(mat, aiTextureType_DIFFUSE_ROUGHNESS, Material::TextureType::Roughness, *material));
        material->HasAO(LoadMaterialTextures(mat, aiTextureType_AMBIENT_OCCLUSION, Material::TextureType::AmbientOcclusion, *material));
        newMesh->SetMaterial(material);
    }


    return newMesh;

}

bool ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, Material::TextureType textureType, Material& material)
{
    bool hasTexture = false;
    for (UINT i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string texturePath = basePath + "/" + str.C_Str();
        material.LoadTexture(textureType, texturePath);
        hasTexture = true;
    }
    return hasTexture;
}


void ModelLoader::Update(float deltaTime)
{
    m_model.Update(deltaTime);
}

void ModelLoader::Render()
{
    m_model.Render();
}
