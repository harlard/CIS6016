#include "Mesh.h"
#include <iostream>
#include <map>

Mesh::Mesh(const std::string& filePath) : shaderProgram(0), rootNode(nullptr), scene(nullptr) {
    loadMesh(filePath);
    loadAnimations(scene);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setShader(ShaderProgram* shaderProgram) {
    this->shaderProgram = shaderProgram;
}

void Mesh::draw() {
    // set texture uniforms
    int textureIndex = 0;
    for (const Texture& texture : textures) {
        std::string uniformName = "texture_" + std::to_string(textureIndex++);
        shaderProgram->setInt(uniformName.c_str(), textureIndex);
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::loadMesh(const std::string& filePath) {
    scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model (" << filePath << "): " << importer.GetErrorString() << std::endl;
        return;
    }

    globalInverseTransform = glm::inverse(glm::transpose(glm::make_mat4(&scene->mRootNode->mTransformation.a1)));

    rootNode = scene->mRootNode;

    std::vector<Vertex> meshVertices;
    std::vector<unsigned int> meshIndices;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        loadBones(mesh, bones);

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            Vertex vertex;

            vertex.position = glm::make_vec3(&mesh->mVertices[j].x);
            vertex.normal = glm::make_vec3(&mesh->mNormals[j].x);

            if (mesh->HasTextureCoords(0)) {
                vertex.texCoords = glm::make_vec2(&mesh->mTextureCoords[0][j].x);
            }

            meshVertices.push_back(vertex);
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                meshIndices.push_back(face.mIndices[k]);
            }
        }
    }

    vertices = meshVertices;
    indices = meshIndices;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}
void Mesh::loadAnimations(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        aiAnimation* anim = scene->mAnimations[i];
        Animation animation;
        animation.name = anim->mName.C_Str();
        animation.duration = anim->mDuration;

        for (unsigned int j = 0; j < anim->mNumChannels; ++j) {
            aiNodeAnim* nodeAnim = anim->mChannels[j];
            animation.nodeAnimations.push_back(nodeAnim);
        }

        animations.push_back(animation);
    }
}

void Mesh::loadBones(aiMesh* mesh, std::vector<Bone>& bones) {
    std::map<int, int> bonesMap;
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        aiBone* bone = mesh->mBones[i];
        Bone newBone;
        newBone.name = bone->mName.C_Str();
        newBone.offset = glm::make_mat4(&bone->mOffsetMatrix[0][0]);

        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            aiVertexWeight weight = bone->mWeights[j];
            int vertexId = weight.mVertexId;
            float weightValue = weight.mWeight;

            if (bonesMap.find(vertexId) == bonesMap.end()) {
                bonesMap[vertexId] = bones.size();
                bones.push_back(newBone);
            }

            bones[bonesMap[vertexId]].addWeight(i, weightValue);
        }
    }
}
