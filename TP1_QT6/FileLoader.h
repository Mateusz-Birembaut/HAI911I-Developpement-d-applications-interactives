#ifndef FILELOADER_H
#define FILELOADER_H

#include "Mesh.h"

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

    std::getline(file, line);
    if(line != "OFF" && line.find("COFF") == std::string::npos) {
        throw std::runtime_error("Not a valid OFF file!");
    }

    header.hasColor = (line.find("COFF") != std::string::npos);

    do {
        std::getline(file, line);
    } while(line[0] == '#');

    std::istringstream headerStream(line);
    headerStream >> header.numVertices >> header.numFaces;

    return header;
}

void loadOff(std::ifstream& file, Mesh& m, int numVertices, int numFaces){
    std::string line;
    m.m_vertices.reserve(numVertices);

    for(int i{0}; i < numVertices; ++i){
        std::getline(file, line);
        std::istringstream vertexStream(line);
        float x, y ,z;
        vertexStream >> x >> y >> z;
        m.m_vertices.push_back({
                                {x,y,z},
                                {},
                                {},
                                });
    }

    for(int i {0}; i < numFaces; ++i){
        std::getline(file, line);
        std::istringstream facesStream(line);
        int n;
        facesStream >> n;
        std::vector<unsigned int> indices(n);
        for(int j{0}; j < n; j++) {
            facesStream >> indices[j];
        }
        for(int j{1}; j < n-1; j++){
            m.m_indices.push_back(indices[0]);
            m.m_indices.push_back(indices[j]);
            m.m_indices.push_back(indices[j+1]);
        }
    }
}
}



#endif // FILELOADER_H
