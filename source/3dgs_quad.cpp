#include<3dgs_quad.hpp>
#include<algorithm>
gs::gs( std::string Path){
    this->Path = Path;
    this->defaultModth = POINT_SPIRIT_INSTANCED;
    //shader
}



bool gs::load() {
    try {
        if (Path.empty()) {
            fprintf(stderr, "Error: PLY path is empty!\n");
            return false;
        }
        happly::PLYData ply = happly::PLYData(this->Path);
        if (!ply.hasElement("vertex")) {
            fprintf(stderr, "Error: PLY file has no 'vertex' element!\n");
            return false;
        }
        auto& vertexElement = ply.getElement("vertex");
        std::vector<float>& x = vertexElement.getProperty<float>(propertyName[0]);
        std::vector<float>& y = vertexElement.getProperty<float>(propertyName[1]);
        std::vector<float>& z = vertexElement.getProperty<float>(propertyName[2]);
        std::vector<float>& opacity_log = vertexElement.getProperty<float>(propertyName[3]);
        std::vector<float>& scale_x_log = vertexElement.getProperty<float>(propertyName[4]);
        std::vector<float>& scale_y_log = vertexElement.getProperty<float>(propertyName[5]);
        std::vector<float>& scale_z_log = vertexElement.getProperty<float>(propertyName[6]);
        std::vector<float>& rot_y = vertexElement.getProperty<float>(propertyName[7]);
        std::vector<float>& rot_z = vertexElement.getProperty<float>(propertyName[8]);
        std::vector<float>& rot_w = vertexElement.getProperty<float>(propertyName[9]);
        std::vector<float>& rot_x = vertexElement.getProperty<float>(propertyName[10]);
        std::vector<float>& dc_r = vertexElement.getProperty<float>(propertyName[11]);
        std::vector<float>& dc_g = vertexElement.getProperty<float>(propertyName[12]);
        std::vector<float>& dc_b = vertexElement.getProperty<float>(propertyName[13]);

        num = x.size();
        if (num == 0) {
            fprintf(stderr, "Error: PLY file has 0 vertices!\n");
            return false;
        }
        if (y.size() != num || z.size() != num || opacity_log.size() != num ||
            scale_x_log.size() != num || scale_y_log.size() != num || scale_z_log.size() != num ||
            rot_y.size() != num || rot_z.size() != num || rot_w.size() != num || rot_x.size() != num ||
            dc_r.size() != num || dc_g.size() != num || dc_b.size() != num) {
            fprintf(stderr, "Error: PLY property length mismatch!\n");
            return false;
        }
        gaussian.reserve(num); 
        gaussian.clear();

        for (size_t i = 0; i < num; ++i) {
            splatPoint point;
            //position(xyz)
            point.position = glm::vec3(x[i], y[i], z[i]);
            //scale(xyz)
            float scale_x = std::exp(scale_x_log[i]);
            float scale_y = std::exp(scale_y_log[i]);
            float scale_z = std::exp(scale_z_log[i]);
            point.scale = glm::vec3( glm::clamp(scale_x, 0.001f, 10.0f), glm::clamp(scale_y, 0.001f, 10.0f), glm::clamp(scale_z, 0.001f, 10.0f));
            //qual
            point.rot = glm::vec4(rot_x[i], rot_y[i], rot_z[i], rot_w[i]);
            //color(rgba)
            float opacity = 1.0f / (1.0f + std::exp(-opacity_log[i]));
            opacity = glm::clamp(opacity, 0.01f, 1.0f);
            glm::vec3 dc(dc_r[i], dc_g[i], dc_b[i]);
            dc = dc * SH_C0 + glm::vec3(0.5f);
            point.color= glm::vec4(dc.r,dc.g,dc.b,opacity);
            //cov3d
            computecov3d(point.scale, point.rot, point.cov3d_upper, point.cov3d_lower);
            gaussian.push_back(point);
        }
        if (!gaussian.empty()) {
            const splatPoint &p = gaussian[0];
            printf("First splatPoint[0]: pos=(%f,%f,%f) scale=(%f,%f,%f) rot=(%f,%f,%f,%f) color=(%f,%f,%f,%f) cov_up=(%f,%f,%f) cov_lo=(%f,%f,%f)\n",
                   p.position.x, p.position.y, p.position.z,
                   p.scale.x, p.scale.y, p.scale.z,
                   p.rot.x, p.rot.y, p.rot.z, p.rot.w,
                   p.color.r, p.color.g, p.color.b, p.color.a,
                   p.cov3d_upper.x, p.cov3d_upper.y, p.cov3d_upper.z,
                   p.cov3d_lower.x, p.cov3d_lower.y, p.cov3d_lower.z);
        }
        init();
        printf("=== PLY Load Debug ===\n");
        printf("Total points: %zu\n", num);
        printf("Successfully loaded %s (num: %zu)\n", Path.c_str(), num);
        return true;
    } catch (const std::exception& e) {
        fprintf(stderr, "Unexpected error in load(): %s\n", e.what());
        return false;
    }
}
void gs::init(){
    if(this->defaultModth == POINT_SPIRIT){
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,gaussian.size()*sizeof(splatPoint),&gaussian[0],GL_STREAM_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,cov3d_upper));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,cov3d_lower));
    }else if(this->defaultModth == POINT_SPIRIT_INSTANCED){
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(pointVerts),&pointVerts[0],GL_STREAM_COPY);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

        glGenBuffers(1,&instVbo);
        glBindBuffer(GL_ARRAY_BUFFER,instVbo);
        glBufferData(GL_ARRAY_BUFFER,gaussian.size()*sizeof(splatPoint),&gaussian[0],GL_STREAM_COPY);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)0);
        glVertexAttribDivisor(1,1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,color));
        glVertexAttribDivisor(2,1);
        // 使用两个 vec3 存储协方差矩阵的上三角部分
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,cov3d_upper));
        glVertexAttribDivisor(3,1);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,cov3d_lower));
        glVertexAttribDivisor(4,1);
    }else{
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(quadVerts),&quadVerts[0],GL_STREAM_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

        glGenBuffers(1,&instVbo);
        glBindBuffer(GL_ARRAY_BUFFER,instVbo);
        glBufferData(GL_ARRAY_BUFFER,gaussian.size()*sizeof(splatPoint),&gaussian[0],GL_DYNAMIC_DRAW);
        // debug: print instance count
        std::cout << "gs::init - instance count: " << gaussian.size() << std::endl;
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)0);
        glVertexAttribDivisor(1,1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,scale));
        glVertexAttribDivisor(2,1);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,rot));
        glVertexAttribDivisor(3,1);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,color));
        glVertexAttribDivisor(4,1);
        // 使用两个 vec3 存储协方差矩阵的上三角部分
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,cov3d_upper));
        glVertexAttribDivisor(5,1);
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6,3,GL_FLOAT,GL_FALSE,sizeof(splatPoint),(void*)offsetof(splatPoint,cov3d_lower));
        glVertexAttribDivisor(6,1);

    }
    
}
void gs::computecov3d(const glm::vec3& scale, const glm::vec4 rot, glm::vec3& upper, glm::vec3& lower){
    glm::mat3 S2(0.0);
    S2[0][0] = scale.x * scale.x, S2[1][1] = scale.y * scale.y, S2[2][2] = scale.z * scale.z;
    glm::quat q(rot.x,rot.y,rot.z,rot.w);
    glm::mat3 R = glm::mat3_cast(q);
    glm::mat3 cov3d = R * S2 * glm::transpose(R);
    // 提取上三角部分
    upper = glm::vec3(cov3d[0][0], cov3d[0][1], cov3d[0][2]);  // a, b, c
    lower = glm::vec3(cov3d[1][1], cov3d[1][2], cov3d[2][2]);  // d, e, f
}
void gs::Draw(Shader* shader){
    if(this->defaultModth == POINT_SPIRIT){
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS,0,gaussian.size());

    }else if(this->defaultModth == POINT_SPIRIT_INSTANCED){
        glBindVertexArray(vao);
        glDrawArraysInstanced(GL_POINTS,0,1,gaussian.size());
    }
    else{
        glBindVertexArray(vao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,(GLsizei)gaussian.size());

    }


}