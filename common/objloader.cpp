#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <tuple>
#include <map>

#include <glm/glm.hpp>

#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(
    const char *path,
    std::vector<glm::vec3> &out_vertices,
    std::vector<glm::vec2> &out_uvs,
    std::vector<glm::vec3> &out_normals,
    std::vector<unsigned int> &out_indices
) {
    printf("Loading OBJ file %s...\n", path);

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    std::map<std::tuple<unsigned int, unsigned int, unsigned int>, unsigned int> indexMap;

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file! Are you in the right path? See Tutorial 1 for details.\n");
        getchar();
        return false;
    }

    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y; // Invert V coordinate
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(
                file,
                "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]
            );

            if (matches != 9) {
                printf("File can't be read by our simple parser. Try exporting with other options.\n");
                fclose(file);
                return false;
            }

            for (int i = 0; i < 3; i++) {
                // Create a tuple for the vertex/UV/normal combination
                auto key = std::make_tuple(vertexIndex[i] - 1, uvIndex[i] - 1, normalIndex[i] - 1);

                // Check if the combination already exists
                if (indexMap.find(key) == indexMap.end()) {
                    // Add new vertex, UV, and normal to the output vectors
                    out_vertices.push_back(temp_vertices[std::get<0>(key)]);
                    out_uvs.push_back(temp_uvs[std::get<1>(key)]);
                    out_normals.push_back(temp_normals[std::get<2>(key)]);

                    // Assign a new index for this combination
                    unsigned int newIndex = static_cast<unsigned int>(out_vertices.size() - 1);
                    indexMap[key] = newIndex;
                    out_indices.push_back(newIndex);
                } else {
                    // Use the existing index
                    out_indices.push_back(indexMap[key]);
                }
            }
        } else {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    fclose(file);
    return true;
}
