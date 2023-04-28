#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        aiString path;
    };

    struct Bone {
        std::string name;
        int id;
        glm::mat4 offset;
        std::vector<std::pair<int, float>> weights;

        void addWeight(int vertexId, float weight) {
          weights.emplace_back(vertexId, weight);
        }
    };

    struct Animation {
        std::string name;
        float duration;
        std::vector<aiNodeAnim*> nodeAnimations;
    };

    Mesh(const std::string& filePath);
    ~Mesh();

    void setShader(unsigned int shaderProgram);
    void draw();

private:
    void loadMesh(const std::string& filePath);
    void loadBones(aiMesh* mesh, std::vector<Bone>& bones);
    void loadAnimations(const aiScene* scene);
    void updateAnimation(float animationTime, const std::string& animationName, glm::mat4 parentTransform = glm::mat4(1.0f));

    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    std::vector<Bone> bones;
    std::vector<Animation> animations;

    glm::mat4 globalInverseTransform;
    aiNode* rootNode;
    const aiScene* scene;
    Assimp::Importer importer;
};
