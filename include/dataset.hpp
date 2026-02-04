#pragma once
#include "glad/glad.h"
#include<happly/happly.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/gtc/quaternion.hpp>
#include <algorithm>

namespace Dataset{
    const float SH_C0 = 0.28209479177387814;
    const std::string propertyName[14] = { "x","y","z",
                                    "opacity",
                                    "scale_0","scale_1","scale_2",
                                    "rot_0","rot_1","rot_2","rot_3",
                                    "f_dc_0","f_dc_1","f_dc_2"};
    struct splatPoint{ 
        alignas(16) glm::vec3 position; 
        alignas(16) glm::vec4 color;
        alignas(16) glm::vec3 cov3d_upper;  // a, b, c cov3d_upper = (cov[0][0], cov[0][1], cov[0][2])
        alignas(16) glm::vec3 cov3d_lower;  // d, e, f cov3d_lower = (cov[1][1], cov[1][2], cov[2][2])
    };
    struct gsWithImportance {
        int index;
        float importance;
    };
    struct gsWithDepth {
        int index;
        int depth;
    };

    using SplatBuffer = std::vector<splatPoint>;
    using SplatImportance = std::vector<gsWithImportance>;
    using SplatDepth = std::vector<gsWithDepth>;



    inline void computecov3d(const glm::vec3& scale, const glm::vec4& rot, glm::vec3& upper, glm::vec3& lower){
        glm::mat3 S2(0.0);
        S2[0][0] = scale.x * scale.x, S2[1][1] = scale.y * scale.y, S2[2][2] = scale.z * scale.z;
        glm::quat q(rot.x,rot.y,rot.z,rot.w);
        glm::mat3 R = glm::mat3_cast(q);
        glm::mat3 cov3d = R * S2 * glm::transpose(R);
        // 提取上三角部分
        upper = glm::vec3(cov3d[0][0], cov3d[0][1], cov3d[0][2]);  // a, b, c
        lower = glm::vec3(cov3d[1][1], cov3d[1][2], cov3d[2][2]);  // d, e, f
    }
    class Data{
        public:
            Data(const std::string& path):path_(path){ num_vertices_ = 0;}
            const SplatBuffer& buffer() const { return buffer_;}
            const int& num_vertices() const { return num_vertices_;}
            const std::vector<int>& sortedIndex() const { return sortedIndex_;}
            const std::string& get_ply_path() const { return path_; }
            bool load() {
                happly::PLYData ply = happly::PLYData(path_);
                auto& vertexElement = ply.getElement("vertex");
                std::vector<float>& x = vertexElement.getProperty<float>(propertyName[0]);
                std::vector<float>& y = vertexElement.getProperty<float>(propertyName[1]);
                std::vector<float>& z = vertexElement.getProperty<float>(propertyName[2]);
                std::vector<float>& opacity_log = vertexElement.getProperty<float>(propertyName[3]);
                std::vector<float>& scale_x_log = vertexElement.getProperty<float>(propertyName[4]);
                std::vector<float>& scale_y_log = vertexElement.getProperty<float>(propertyName[5]);
                std::vector<float>& scale_z_log = vertexElement.getProperty<float>(propertyName[6]);
                std::vector<float>& rot_w = vertexElement.getProperty<float>(propertyName[7]);
                std::vector<float>& rot_x = vertexElement.getProperty<float>(propertyName[8]);
                std::vector<float>& rot_y = vertexElement.getProperty<float>(propertyName[9]);
                std::vector<float>& rot_z = vertexElement.getProperty<float>(propertyName[10]);
                std::vector<float>& dc_r = vertexElement.getProperty<float>(propertyName[11]);
                std::vector<float>& dc_g = vertexElement.getProperty<float>(propertyName[12]);
                std::vector<float>& dc_b = vertexElement.getProperty<float>(propertyName[13]);
                num_vertices_ = x.size();
                buffer_.clear();
                buffer_.reserve(num_vertices_);
                SplatImportance importances(num_vertices_);

                for (size_t i = 0; i < num_vertices_; ++i) {
                    splatPoint point;
                    gsWithImportance importance;
                    //position(xyz)
                    point.position = glm::vec3(x[i], y[i], z[i]);
                    //scale(xyz)
                    float scale_x = std::exp(scale_x_log[i]);
                    float scale_y = std::exp(scale_y_log[i]);
                    float scale_z = std::exp(scale_z_log[i]);
                    glm::vec3 scale( scale_x, scale_y, scale_z);
                    //qual
                    glm::vec4 rot(rot_x[i], rot_y[i], rot_z[i], rot_w[i]);
                    //color(rgba)
                    float opacity = 1.0f / (1.0f + std::exp(-opacity_log[i]));
                    glm::vec3 dc = glm::vec3(dc_r[i], dc_g[i], dc_b[i]) * SH_C0 + glm::vec3(0.5f);
                    point.color = glm::vec4(dc,opacity);
                    computecov3d(scale, rot, point.cov3d_upper, point.cov3d_lower);
                    
                    buffer_.push_back(point);
                    importance.importance = opacity * scale_x * scale_x * scale_z;
                    importance.index = i;
                    importances[i] = importance;
                }
                sortedIndex_.reserve(num_vertices_);
                sortedIndex_.clear();
                std::sort(importances.begin(),importances.end(),
                        [](const gsWithImportance& a,const gsWithImportance& b){
                            return a.importance > b.importance;
                        });
                for(int i = 0; i < num_vertices_; ++i)
                    sortedIndex_.push_back(importances[i].index);
                printf("=== PLY Load Debug ===\n"); 
                printf("Successfully loaded %s (num: %zu)\n", path_.c_str(), num_vertices_);
                return true;
            }
        private:
            std::string path_;
            SplatBuffer buffer_;
            std::vector<int> sortedIndex_;
            int num_vertices_;
        };
}



