#include "Render/Mesh.hpp"

#include <fstream>
#include <sstream>

Mesh::Mesh()
    : m_faces{},
    m_vertices{}
{
}

size_t Mesh::addVertex(const Vertex& vertex)
{
    auto it = std::find(m_vertices.begin(), m_vertices.end(), vertex);
    if (it != m_vertices.end())
        return it - m_vertices.begin();

    m_vertices.push_back(vertex);
    return m_vertices.size() - 1;
}

void Mesh::addFace(const std::vector<size_t>& indices)
{
    m_faces.push_back(indices);
}

void Mesh::load(const std::string& filename)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::ifstream file(filename);
    if (file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (line.substr(0, 2) == "v ") {
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
                    vert.uv = uvs[std::atoi(r.c_str()) - 1];
                    std::getline(ss2, r, '/');
                    vert.normal = normals[std::atoi(r.c_str()) - 1];
                    
                    vertIds.push_back(addVertex(vert));
                }

                addFace(vertIds);
            }
        }

        file.close();
    }
}