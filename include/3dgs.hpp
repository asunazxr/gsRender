#pragma once
#include<shader.hpp>
#include<dataset.hpp>
#include<memory>

class gs
{
private:
    float quadVerts[8] = {
        -1.0f, -1.0f, // 左下
         1.0f, -1.0f, // 右下
         1.0f,  1.0f, // 右上
        -1.0f,  1.0f, // 左上
    };

    int num;
    GLuint vao, vbo, spaltssbo,indexssbo;           // OpenGL缓冲区对象
public:
    gs(const std::shared_ptr<Dataset::Data>& data){
        num = data->num_vertices();
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(quadVerts),&quadVerts[0],GL_STATIC_COPY);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

        glGenBuffers(1,&spaltssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER,spaltssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, num * sizeof(Dataset::splatPoint),data->buffer().data(),GL_STATIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        glGenBuffers(1,&indexssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER,indexssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, num * sizeof(int),data->sortedIndex().data(),GL_STATIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    }
    void Draw(Shader* shader){
        glBindVertexArray(vao);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,spaltssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,1,indexssbo);
        glDrawArraysInstanced(GL_TRIANGLE_FAN,0,4,num);

    }
    
};