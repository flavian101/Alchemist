#include "ModelLoader.h"
#include "models/Vertex.h"
#include "Graphics/Graphics.h"
#include "Scene/Shaders/ShaderManager.h"
#include "MeshParts.h"
#include "Mesh.h"

ModelLoader::ModelLoader(Graphics& g, std::shared_ptr<ShaderManager> manager)
	:
	RenderableObject(m_name, g, manager),
    m_graphics(g)
{
}

bool ModelLoader::LoadModel(const std::string& filepath)
{
    m_filepath = filepath;
	Assimp::Importer importer;
    basePath = std::filesystem::path(filepath).parent_path().string();

    const aiScene* pScene = importer.ReadFile(filepath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
		return false;

    setName(pScene->mRootNode->mName.C_Str());
	this->ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
       // Mesh newMesh = this->ProcessMesh(mesh, scene);
        m_meshes.push_back(&ProcessMesh(mesh, scene));
    }
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}

}

Mesh& ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<unsigned short> m_indices;
    std::vector<Vertex> m_vertices;

    //get the vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->HasNormals() && mesh->HasTextureCoords(0) && mesh->HasTangentsAndBitangents())
        {
            m_vertices.emplace_back(
                mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
                mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
                mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z,
                mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z
            );
        }
        else if (mesh->HasNormals() && mesh->HasTextureCoords(0))
        {
            m_vertices.emplace_back(
                mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
                mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
                mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z
            );
        }
        else if (mesh->HasTextureCoords(0))
        {
            m_vertices.emplace_back(
                mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
                mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y
            );
        }
        else
        {
            m_vertices.emplace_back(
                mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z
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
    auto material = std::make_unique<Material>(m_graphics);

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        material->hasAlbedoMap = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, Material::TextureType::Albedo, *material);
        material->hasNormalMap = LoadMaterialTextures(mat, aiTextureType_NORMALS, Material::TextureType::Normal, *material);
        material->hasMetallicMap = LoadMaterialTextures(mat, aiTextureType_METALNESS, Material::TextureType::Metallic, *material);
        material->hasRoughnessMap = LoadMaterialTextures(mat, aiTextureType_DIFFUSE_ROUGHNESS, Material::TextureType::Roughness, *material);
        material->hasAOMap = LoadMaterialTextures(mat, aiTextureType_AMBIENT_OCCLUSION, Material::TextureType::AmbientOcclusion, *material);

    }


    part = new MeshParts(m_graphics, m_indices, m_vertices);
    Mesh* mmesh = new Mesh(m_graphics);
    mmesh->AddMeshPart(part);
    mmesh->SetMaterial(std::move(material)); // Set the material for the mesh

    return *mmesh;

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
    RenderableObject::Update(deltaTime);
}

void ModelLoader::Render()
{
    RenderableObject::Render();
    for (auto& meshes : m_meshes)
    {
        meshes->Bind();
        meshes->Render();
    }
}
