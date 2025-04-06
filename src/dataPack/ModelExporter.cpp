#include "ModelExporter.h"
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/material.h> 
#include <filesystem>
#include <fstream>


ModelExporter::ModelExporter(Model& model)
    : m_model(model) {
}

ModelExporter::~ModelExporter()
{
}
void ModelExporter::Export(const std::string& filepath)
{
    // Get the export directory from the filepath
    std::filesystem::path exportPath(filepath);
    std::filesystem::path exportDir = exportPath.parent_path();

    // Create the export directory if it doesn't exist
    std::filesystem::create_directories(exportDir);

    Assimp::Exporter exporter;

    // 1) Create scene and root node with proper initialization
    aiScene* scene = new aiScene();
    scene->mFlags = 0;  // Initialize flags
    scene->mRootNode = new aiNode();
    scene->mRootNode->mTransformation = aiMatrix4x4(); // Identity matrix
    scene->mRootNode->mParent = nullptr;
    scene->mRootNode->mName = aiString(m_model.GetName());

    // 2) Prepare containers for all meshes & materials
    std::vector<aiMesh*> aiMeshes;
    std::vector<aiMaterial*> aiMaterials;

    try {
        // 3) Convert each of your Model's meshParts → aiMesh + aiMaterial
        for (const auto& meshPart : m_model.getMeshParts())
        {
            if (!meshPart) continue; // Skip invalid mesh parts

            // --- Create aiMesh
            aiMesh* ai_mesh = new aiMesh();
            ai_mesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE; // Set primitive type
            ai_mesh->mNumVertices = meshPart->getVertexCount();

            // Allocate vertex data only if we have vertices
            if (ai_mesh->mNumVertices > 0) {
                ai_mesh->mVertices = new aiVector3D[ai_mesh->mNumVertices];

                // Only allocate these if the vertex type needs them
                const auto& vertices = meshPart->getVertices();
                if (!vertices.empty()) {
                    bool hasNormals = false;
                    bool hasTexCoords = false;
                    bool hasTangents = false;

                    // Check first vertex type to determine what data we need
                    switch (vertices[0].type) {
                    case VertexType::Pos3TexNorm:
                    case VertexType::Pos3TexNormTan:
                        hasNormals = true;
                        hasTexCoords = true;
                        break;
                    case VertexType::Pos3Tex:
                        hasTexCoords = true;
                        break;
                    }

                    if (hasNormals) {
                        ai_mesh->mNormals = new aiVector3D[ai_mesh->mNumVertices];
                    }
                    if (hasTexCoords) {
                        ai_mesh->mTextureCoords[0] = new aiVector3D[ai_mesh->mNumVertices];
                        ai_mesh->mNumUVComponents[0] = 2;
                    }
                    if (hasTangents) {
                        ai_mesh->mTangents = new aiVector3D[ai_mesh->mNumVertices];
                    }

                    // Fill vertices, normals, uvs, tangents...
                    for (unsigned i = 0; i < ai_mesh->mNumVertices; ++i) {
                        switch (vertices[i].type) {
                        case VertexType::pos3:
                            ai_mesh->mVertices[i] = aiVector3D(
                                vertices[i].pos.x,
                                vertices[i].pos.y,
                                vertices[i].pos.z
                            );
                            break;

                        case VertexType::Pos3Col:
                            ai_mesh->mVertices[i] = aiVector3D(
                                vertices[i].posCol.pos.x,
                                vertices[i].posCol.pos.y,
                                vertices[i].posCol.pos.z
                            );
                            break;

                        case VertexType::Pos3Tex:
                            ai_mesh->mVertices[i] = aiVector3D(
                                vertices[i].posTex.pos.x,
                                vertices[i].posTex.pos.y,
                                vertices[i].posTex.pos.z
                            );
                            if (hasTexCoords) {
                                ai_mesh->mTextureCoords[0][i] = aiVector3D(
                                    vertices[i].posTex.tex.x,
                                    1.0f - vertices[i].posTex.tex.y,
                                    0.0f
                                );
                            }
                            break;

                        case VertexType::Pos3TexNorm:
                            ai_mesh->mVertices[i] = aiVector3D(
                                vertices[i].posTexNorm.pos.x,
                                vertices[i].posTexNorm.pos.y,
                                vertices[i].posTexNorm.pos.z
                            );
                            if (hasTexCoords) {
                                ai_mesh->mTextureCoords[0][i] = aiVector3D(
                                    vertices[i].posTexNorm.tex.x,
                                    1.0f - vertices[i].posTexNorm.tex.y,
                                    0.0f
                                );
                            }
                            if (hasNormals) {
                                ai_mesh->mNormals[i] = aiVector3D(
                                    vertices[i].posTexNorm.norm.x,
                                    vertices[i].posTexNorm.norm.y,
                                    vertices[i].posTexNorm.norm.z
                                );
                            }
                            break;

                        case VertexType::Pos3TexNormTan:
                            ai_mesh->mVertices[i] = aiVector3D(
                                vertices[i].posTexNormTan.pos.x,
                                vertices[i].posTexNormTan.pos.y,
                                vertices[i].posTexNormTan.pos.z
                            );
                            if (hasTexCoords) {
                                ai_mesh->mTextureCoords[0][i] = aiVector3D(
                                    vertices[i].posTexNormTan.tex.x,
                                    1.0f - vertices[i].posTexNormTan.tex.y,
                                    0.0f
                                );
                            }
                            if (hasNormals) {
                                ai_mesh->mNormals[i] = aiVector3D(
                                    vertices[i].posTexNormTan.norm.x,
                                    vertices[i].posTexNormTan.norm.y,
                                    vertices[i].posTexNormTan.norm.z
                                );
                            }
                            if (hasTangents) {
                                ai_mesh->mTangents[i] = aiVector3D(
                                    vertices[i].posTexNormTan.tangent.x,
                                    vertices[i].posTexNormTan.tangent.y,
                                    vertices[i].posTexNormTan.tangent.z
                                );
                            }
                            break;
                        }

                    }
                }
            }

            // Build faces
            const auto& indices = meshPart->getIndices();
            if (!indices.empty()) {
                ai_mesh->mNumFaces = static_cast<unsigned int>(indices.size() / 3);
                if (ai_mesh->mNumFaces > 0) {
                    ai_mesh->mFaces = new aiFace[ai_mesh->mNumFaces];
                    for (unsigned f = 0; f < ai_mesh->mNumFaces; ++f) {
                        aiFace& face = ai_mesh->mFaces[f];
                        face.mNumIndices = 3;
                        face.mIndices = new unsigned int[3];
                        face.mIndices[0] = indices[f * 3 + 0];
                        face.mIndices[1] = indices[f * 3 + 1];
                        face.mIndices[2] = indices[f * 3 + 2];
                    }
                }
            }

            // Default material index = 0
            ai_mesh->mMaterialIndex = 0;
            aiMeshes.push_back(ai_mesh);

           
            // --- Create aiMaterial if your meshPart has one
            if (auto mat = meshPart->GetMaterial())
            {
                aiMaterial* ai_mat = new aiMaterial();
                const auto& md = mat->materialBuffer.data.materialStruct;

                // Diffuse/Base color
                aiColor4D base(md.baseColor.x, md.baseColor.y, md.baseColor.z, md.baseColor.w);
                ai_mat->AddProperty(&base, 1, AI_MATKEY_COLOR_DIFFUSE);

                // Emissive
                aiColor4D emi(md.emissive.x, md.emissive.y, md.emissive.z, md.emissive.w);
                ai_mat->AddProperty(&emi, 1, AI_MATKEY_COLOR_EMISSIVE);

                // PBR
                ai_mat->AddProperty(&md.metallic, 1, AI_MATKEY_METALLIC_FACTOR);
                ai_mat->AddProperty(&md.roughness, 1, AI_MATKEY_ROUGHNESS_FACTOR);

                std::filesystem::path exportDirPath = std::filesystem::path(filepath).parent_path();
                std::filesystem::path textureDirPath = exportDirPath / "textures";

                // Textures
                for (const auto& [type, tex] : mat->textures)
                {
                    if (!tex) continue;
                    std::string texturePath = tex->GetPath();
                    if (texturePath.empty()) {
                        std::cerr << "Empty texture path encountered, skipping..." << std::endl;
                        continue;
                    }

                    try {
                        // Convert to filesystem path and validate
                        std::filesystem::path srcTexPath(texturePath);
                        if (!std::filesystem::exists(srcTexPath)) {
                            std::cerr << "Texture file not found: " << srcTexPath << std::endl;
                            continue;
                        }

                        // Create valid filename for the destination
                        std::string validFilename = srcTexPath.filename().string();

                        // Create destination path and ensure directory exists
                        std::filesystem::create_directories(textureDirPath);
                        std::filesystem::path destTexPath = textureDirPath / validFilename;

                        // Copy the texture file
                        std::filesystem::copy_file(
                            srcTexPath,
                            destTexPath,
                            std::filesystem::copy_options::overwrite_existing
                        );

                        // Create relative path from GLTF file to texture
                        std::string relativePathStr = "textures/" + validFilename;
                        aiString relativePath(relativePathStr);


                        switch (type) {
                        case Material::TextureType::Albedo:
                        {
                            // Base color texture
                            ai_mat->AddProperty(&relativePath, "$tex.file", aiTextureType_BASE_COLOR, 0);
                            // Set UV index
                            int uvIndex = 0;
                            ai_mat->AddProperty(&uvIndex, 1, "$tex.uvwsrc", aiTextureType_BASE_COLOR, 0);
                        }
                        break;

                        case Material::TextureType::Normal:
                        {
                            ai_mat->AddProperty(&relativePath, "$tex.file", aiTextureType_NORMALS, 0);
                            float normalScale = 1.0f;
                            ai_mat->AddProperty(&normalScale, 1, "$tex.mapping", aiTextureType_NORMALS, 0);
                        }
                        break;

                        case Material::TextureType::Metallic:
                        {
                            // For metallic texture
                            ai_mat->AddProperty(&relativePath, "$tex.file", aiTextureType_METALNESS, 0);
                            int uvIndex = 0;
                            ai_mat->AddProperty(&uvIndex, 1, "$tex.uvwsrc", aiTextureType_METALNESS, 0);
                        }
                        break;

                        case Material::TextureType::Roughness:
                        {
                            // For roughness texture
                            ai_mat->AddProperty(&relativePath, "$tex.file", aiTextureType_DIFFUSE_ROUGHNESS, 0);
                            int uvIndex = 0;
                            ai_mat->AddProperty(&uvIndex, 1, "$tex.uvwsrc", aiTextureType_DIFFUSE_ROUGHNESS, 0);
                        }
                        break;

                        case Material::TextureType::AmbientOcclusion:
                        {
                            // For ambient occlusion texture
                            ai_mat->AddProperty(&relativePath, "$tex.file", aiTextureType_AMBIENT_OCCLUSION, 0);
                            float aoStrength = 1.0f;
                            ai_mat->AddProperty(&aoStrength, 1, "$tex.strength", aiTextureType_AMBIENT_OCCLUSION, 0);
                        }
                        break;

                        case Material::TextureType::Specular:
                        {
                            // For specular texture
                            ai_mat->AddProperty(&relativePath, "$tex.file", aiTextureType_SPECULAR, 0);

                            // Enable KHR_materials_specular extension
                            bool specularWorkflow = true;
                            ai_mat->AddProperty(&specularWorkflow, 1, "$mat.gltf.specularWorkflow", 0, 0);
                        }
                        break;
                        }

                       
            std::cout << "Successfully added texture: " << relativePathStr << " for type: " << static_cast<int>(type) << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to process texture: " << e.what() << std::endl;
        }

                }
                // assign material index on mesh
                ai_mesh->mMaterialIndex = static_cast<unsigned int>(aiMaterials.size());
                aiMaterials.push_back(ai_mat);
            }
        }

        // 4) Wire up scene's mesh array
        scene->mNumMeshes = static_cast<unsigned int>(aiMeshes.size());
        if (scene->mNumMeshes > 0) {
            scene->mMeshes = new aiMesh * [scene->mNumMeshes];
            for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
                scene->mMeshes[i] = aiMeshes[i];
            }
        }

        // 5) Wire up scene's material array (ensure at least one material exists)
        if (aiMaterials.empty()) {
            aiMaterials.push_back(new aiMaterial());
            for (auto* m : aiMeshes) {
                if (m) m->mMaterialIndex = 0;
            }
        }

        scene->mNumMaterials = static_cast<unsigned int>(aiMaterials.size());
        scene->mMaterials = new aiMaterial * [scene->mNumMaterials];
        for (unsigned i = 0; i < scene->mNumMaterials; ++i) {
            scene->mMaterials[i] = aiMaterials[i];
        }

        // 6) Attach all meshes to the root node
        scene->mRootNode->mNumMeshes = scene->mNumMeshes;
        if (scene->mNumMeshes > 0) {
            scene->mRootNode->mMeshes = new unsigned int[scene->mNumMeshes];
            for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
                scene->mRootNode->mMeshes[i] = i;
            }
        }

        // Debug log
        std::cout << "Exporting scene: "
            << scene->mNumMeshes << " meshes, "
            << scene->mNumMaterials << " materials\n";

        // 7) Export to GLTF2
        aiReturn result = exporter.Export(scene, "gltf2", filepath);
        if (result != aiReturn_SUCCESS) {
            std::string err = exporter.GetErrorString();
            throw std::runtime_error("Assimp export failed: " + err);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error during export: " << e.what() << std::endl;
    }

   
    delete scene;
}


