#include "Model.hpp"
#include "Mesh.hpp"
#include "Log.hpp"

#include <fstream>
#include <sstream>

Model::Model()
    : m_meshes{},
    m_materials{}
{
}

Model::~Model()
{
    for (auto mesh : m_meshes) {
        delete mesh;
    }

    for (auto material : m_materials) {
        delete material;
    }
}

bool Model::loadMeshes(const std::string& filename)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::ifstream file(filename);
    if (file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (line.substr(0, 2) == "o ") {
                m_meshes.push_back(new Mesh);
            }
            else if (line.substr(0, 7) == "usemtl ") {
                if (m_meshes.size() == 0) {
                    m_meshes.push_back(new Mesh);
                }

                std::stringstream ss(line.substr(7));
                m_meshes.back()->m_materialName = ss.str();
            }
            else if (line.substr(0, 2) == "v ") {
                // vertex
                std::stringstream ss(line.substr(2));
                float x, y, z;
                ss >> x >> y >> z;
                vertices.push_back(glm::vec3(x, y, z));
            }
            else if (line.substr(0, 3) == "vt ") {
                // uv
                std::stringstream ss(line.substr(3));
                float u, v;
                ss >> u >> v;
                uvs.push_back(glm::vec2(u, v));
            }
            else if (line.substr(0, 3) == "vn ") {
                // normal
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                normals.push_back(glm::vec3(x, y, z));
            }
            else if (line.substr(0, 2) == "f ") {
                // face
                std::stringstream ss(line.substr(2));
                std::vector<size_t> vertIds;

                for (std::string result; std::getline(ss, result, ' ');) {
                    std::stringstream ss2(result);
                    std::string r;
                    Vertex vert;

                    std::getline(ss2, r, '/');
                    vert.position = vertices[std::atoi(r.c_str()) - 1];
                    std::getline(ss2, r, '/');
                    if (r != "") {
                        vert.uv = uvs[std::atoi(r.c_str()) - 1];
                    }
                    std::getline(ss2, r, '/');
                    vert.normal = normals[std::atoi(r.c_str()) - 1];

                    auto id = m_meshes.back()->addVertex(vert);
                    vertIds.push_back(id);
                }

                m_meshes.back()->addFace(vertIds);
            }
        }

        file.close();
    }

    return true;
}

bool Model::loadMaterials(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (line.substr(0, 7) == "newmtl ") {
                std::stringstream ss(line.substr(7));
                m_materials.push_back(new Material(ss.str()));
            }
            else if (line.substr(0, 3) == "Ka ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_materials.back()->m_Ka = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Kd ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_materials.back()->m_Kd = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ks ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_materials.back()->m_Ks = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ke ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_materials.back()->m_Ke = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ni ") {
                std::stringstream ss(line.substr(3));
                float x;
                ss >> x;
                m_materials.back()->m_Ni = x;
            }
            else if (line.substr(0, 3) == "Ns ") {
                std::stringstream ss(line.substr(3));
                float x;
                ss >> x;
                m_materials.back()->m_Ns = x;
            }
            else if (line.substr(0, 2) == "d ") {
                /*std::stringstream ss(line.substr(2));
                float x;
                ss >> x;
                materials[i].m_d = x;*/
            }
            else if (line.substr(0, 6) == "illum ") {
                //std::stringstream ss(line.substr(6));
            }
        }

        file.close();
        return true;
    }

    return false;
}

void Model::create(
    const std::vector<Mesh*>& meshes,
    const std::vector<Material*>& materials)
{
    m_meshes = meshes;
    m_materials = materials;
    build();
}

bool Model::load(const std::string& filename)
{
    if (loadMeshes(filename + ".obj") &&
        loadMaterials(filename + ".mtl"))
    {
        build();
        return true;
    }
    return false;
}

void Model::build() const
{
    for (auto mesh : m_meshes) {
        mesh->build();
    }
}

void Model::draw(const ShaderProgram& program) const
{
    program.uniform("modelMatrix", transform());

    for (auto mesh : m_meshes) {
        for (auto material : m_materials) {
            if (material->name() == mesh->m_materialName) {
                material->use(program);
                break;
            }
        }

        mesh->draw();
    }
}