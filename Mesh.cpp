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

void Mesh::setShader(unsigned int shaderProgram) {
    this->shaderProgram = shaderProgram;
}

void Mesh::draw() {
    glUseProgram(shaderProgram);

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
        glUniform1i(glGetUniformLocation(shaderProgram, textures[i].type.c_str()), i);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
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

void Mesh::loadBones(aiMesh* mesh, std::vector<Bone>& bones) {
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        aiBone* bone = mesh->mBones[i];

        // Check if the bone already exists
        int boneIndex = -1;
        for (unsigned int j = 0; j < bones.size(); j++) {
            if (bones[j].name == bone->mName.C_Str()) {
                boneIndex = j;
                break;
            }
        }

        // If the bone doesn't exist, create a new one
        if (boneIndex == -1) {
            boneIndex = bones.size();
            Bone newBone;
            newBone.name = bone->mName.C_Str();
            newBone.id = boneIndex;
            newBone.offset = glm::transpose(glm::make_mat4(&bone->mOffsetMatrix.a1));
            bones.push_back(newBone);
        }

        // Add the bone weights to the vertices
        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            unsigned int vertexId = bone->mWeights[j].mVertexId;
            float weight = bone->mWeights[j].mWeight;
            for (unsigned int k = 0; k < 4; k++) {
                if (vertices[vertexId].boneIds[k] == -1) {
                    vertices[vertexId].boneIds[k] = boneIndex;
                    vertices[vertexId].weights[k] = weight;
                    break;
                }
            }
        }
    }
}

void Mesh::updateAnimation(float animationTime, const std::string& animationName, glm::mat4 parentTransform)
{
    const Animation* animation = nullptr;
    for (const Animation& anim : animations) {
        if (anim.name == animationName) {
            animation = &anim;
            break;
        }
    }

    if (!animation) {
        return;
    }

    std::vector<glm::mat4> boneTransforms(bones.size());

    for (unsigned int i = 0; i < boneTransforms.size(); ++i) {
        aiNodeAnim* nodeAnim = animation->nodeAnimations[bones[i].id];
        glm::mat4 boneTransform;

        if (nodeAnim) {
            // Interpolate scaling and generate scaling transformation matrix
            aiVector3D scaling;
            calcInterpolatedScaling(scaling, animationTime, nodeAnim);
            glm::mat4 scalingMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

            // Interpolate rotation and generate rotation transformation matrix
            aiQuaternion rotation;
            calcInterpolatedRotation(rotation, animationTime, nodeAnim);
            glm::mat4 rotationMat = glm::mat4_cast(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));

            // Interpolate translation and generate translation transformation matrix
            aiVector3D translation;
            calcInterpolatedPosition(translation, animationTime, nodeAnim);
            glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

            // Combine the above transformations
            boneTransform = translationMat * rotationMat * scalingMat;
        }
        else {
            boneTransform = bones[i].offset;
        }

        boneTransforms[i] = parentTransform * boneTransform;
    }

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        glm::mat4 boneTransform = glm::mat4(0.0f);

        for (unsigned int j = 0; j < vertices[i].boneIds.size(); ++j) {
            int boneId = vertices[i].boneIds[j];
            glm::mat4 boneWeight = vertices[i].boneWeights[j];
            boneTransform += boneWeight * boneTransforms[boneId];
        }

        vertices[i].position = glm::vec3(boneTransform * glm::vec4(vertices[i].originalPosition, 1.0f));
        vertices[i].normal = glm::normalize(glm::mat3(glm::transpose(glm::inverse(boneTransform))) * vertices[i].originalNormal);
    }
}
