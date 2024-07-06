#include "ModelLoader.h"

ModelLoader::ModelLoader(Graphics& g, std::shared_ptr<ShaderManager> manager)
	:
	RenderableObject(m_name, g, manager),
    m_graphics(g)
{
}

bool ModelLoader::LoadModel(const std::string& filepath)
{
	Assimp::Importer importer;

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
		m_meshes.push_back(this->ProcessMesh(mesh, scene));
	}
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}

}

Mesh ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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
    part = new MeshParts(m_graphics);
    part->Initialize(m_indices, m_vertices);
    Mesh* mmesh = new Mesh(m_graphics);
    mmesh->AddMeshPart(*part);
    return *mmesh;

}

void ModelLoader::Update(float deltaTime)
{
    RenderableObject::Update(deltaTime);
}

void ModelLoader::Render()
{
    RenderableObject::Render();
    for (auto meshes : m_meshes)
    {
        meshes.Bind();
        meshes.Render();
    }
}
