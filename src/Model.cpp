#include "Model.hpp"

Model::Model(const std::string& path, bool gamma) {
	loadModel(path);
    gammaCorrection = gamma;
}

void Model::loadModel(string const &path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);

    // Error checking
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "Error! Assimp: " << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    //vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector; // plceholder glm vector. convert assimp to glm::vec3

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
        }
        else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        // texture coords
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec; // again placeholder glm::vec2
            
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            //std::cout << "Model has no tex coords" << std::endl;
        }

        vertices.push_back(vertex);
    }

    // process indices or each "face" / mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Material matProps = loadMaterialColors(material);

    /*vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/

    // return final mesh
    return Mesh(vertices, indices, matProps);
}

Material Model::loadMaterialColors(aiMaterial* mat) {
    Material material;

    aiColor3D defaultColor(0.0f, 0.0f, 0.0f);
    float shininess = 32.0f;

    // Load colors
    // Ambient
    if (mat->Get(AI_MATKEY_COLOR_AMBIENT, defaultColor) == AI_SUCCESS) {
        material.Ambient = glm::vec3(defaultColor.r, defaultColor.g, defaultColor.b);
    } else {
        material.Ambient = glm::vec3(0.1f);
    }

    // Diffuse
    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, defaultColor) == AI_SUCCESS) {
        material.Diffuse = glm::vec3(defaultColor.r, defaultColor.g, defaultColor.b);
    } else {
        material.Diffuse = glm::vec3(1.0f);
    }

    // Specular
    if (mat->Get(AI_MATKEY_COLOR_SPECULAR, defaultColor) == AI_SUCCESS) {
        material.Specular = glm::vec3(defaultColor.r, defaultColor.g, defaultColor.b);
    } else {
        material.Specular = glm::vec3(0.5f); // Default specular
    }

    // Shininess
    if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        material.Shininess = shininess;
    }
    else {
        material.Shininess = 32.0f; // Default specular
    }
    return material;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        // new unloaded texture
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}


unsigned int Model::TextureFromFile(const char* path, const string& directory, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        stbi_image_free(data);
    }

    return textureID;
}

void Model::Draw(Shader &graphics, Shader &light) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        if (m_isLightEmitter) {
            light.useProgram();
            meshes[i].DrawLight(light);
        }
        else {
            graphics.useProgram();
            meshes[i].DrawColor(graphics);
        }
    }
}

void Model::CleanUp() {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].CleanUp();
    }
}