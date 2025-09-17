#ifndef FILELOADER_H
#define FILELOADER_H

#include "Mesh.h"

#include <filesystem>
#include <iostream>
#include <fstream>




namespace FileLoader {

struct OffHeader {
    int numVertices;
    int numFaces;
    bool hasColor = false;
    bool hasNormals = false;
};

OffHeader readOffHeader(std::ifstream& file) {
    OffHeader header;
    std::string line;

    do {
        if(!std::getline(file, line)) throw std::runtime_error("Empty file!");
    } while(line.empty() || line[0] == '#');

    if(line != "OFF" && line != "COFF") {
        throw std::runtime_error("Not a valid OFF/COFF file!");
    }

    header.hasColor = (line == "COFF");

    do {
        if(!std::getline(file, line)) throw std::runtime_error("Missing header line!");
    } while(line.empty() || line[0] == '#');

    std::istringstream headerStream(line);
    int numEdges;
    headerStream >> header.numVertices >> header.numFaces >> numEdges;
    return header;
}

void loadOff(std::ifstream& file, Mesh& m, int numVertices, int numFaces, bool hasColor){
    std::string line;
    m.m_vertices.reserve(numVertices);

    for(int i{0}; i < numVertices;){
        std::getline(file, line);
        if(line.empty() || line[0] == '#') continue;
        std::istringstream vertexStream(line);
        float x, y ,z;
        vertexStream >> x >> y >> z;
        Vec3<float> color;
        if(hasColor){
            float r,g,b=1.0f;
            if(vertexStream >> r >> g >> b) {
                color = {r,g,b};
            }
        }
        m.m_vertices.push_back({
                                {x,y,z},
                                {},
                                {},
                                });
        ++i;
    }

    for(int i {0}; i < numFaces; ){
        std::getline(file, line);
        if(line.empty() || line[0] == '#') continue;
        std::istringstream facesStream(line);
        int n;
        facesStream >> n;
        std::vector<unsigned int> indices(n);
        for(int j{0}; j < n; j++) {
            facesStream >> indices[j];
        }
        for (int i = 0; i < n-2; ++i) {
            m.m_indices.push_back(indices[0]);
            m.m_indices.push_back(indices[i+1]);
            m.m_indices.push_back(indices[i+2]);
        }
        ++i;
    }
}

void loadOffColor(std::ifstream& file, Mesh& m, int numVertices, int numFaces, bool hasColor){
    std::string line;
    m.m_vertices.reserve(numVertices);

    // Lecture des sommets
    for(int i=0; i<numVertices; ){
        if(!std::getline(file, line)) break;
        if(line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        float x, y, z;
        ss >> x >> y >> z;

        m.m_vertices.push_back({
                                {x,y,z},
                                {},
                                {color},
                                });
        ++i;
    }

    // Lecture des faces et triangulation
    for(int i=0; i<numFaces; ){
        if(!std::getline(file, line)) break;
        if(line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        int n;
        ss >> n;
        std::vector<unsigned int> indices(n);
        for(int j=0; j<n; ++j) ss >> indices[j];
        for(int j=1; j<n-1; ++j){
            m.m_indices.push_back(indices[0]);
            m.m_indices.push_back(indices[j]);
            m.m_indices.push_back(indices[j+1]);
        }
        ++i;
    }
}

void loadFile(const std::filesystem::path& filePath, Mesh& m){
    std::ifstream file(filePath);
    if(!file.is_open()) {
        std::cout << "File did not open !" << std::endl;
        return;
    }

    std::string extension = filePath.extension().string();

    if(extension == ".off" || extension == ".OFF") {
        FileLoader::OffHeader header = FileLoader::readOffHeader(file);
        m.m_vertices.clear();
        m.m_indices.clear();
        FileLoader::loadOff(file, m, header.numVertices, header.numFaces, header.hasColor);
        m.computeNormals();
    }
    else if(extension == ".obj" || extension == ".OBJ") {
        //FileLoader::loadObj(filePath);
    }
    else {
        std::cerr << "Unsupported file format: " << extension << std::endl;
        return;
    }
    file.close();
}

}



#endif // FILELOADER_H
