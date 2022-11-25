#pragma once

#include "Resource.hpp"
#include "GraphicsMath.hpp"

#include <vector>

class Rect
{
protected:
    VertexArray VAO;
    Buffer<2> Buffers;
    const GLint numIndices = 6;
    const unsigned int indices[6] = { 0, 2, 1, 1, 2, 3 };
    const float textureCoords[8] = { 0, 0, 1, 0, 0, 1, 1, 1 };
    GMath::Vec2f m_pos, m_size;

public:
    Rect(const GMath::Vec2f& position, const GMath::Vec2f& size)
    : m_pos(position), m_size(size)
    {

        const std::vector<GLfloat> vertices = 
        {
            position[0], position[1],
            position[0] + size[0], position[1],
            position[0], position[1] + size[1],
            position[0] + size[0], position[1] + size[1]
        };

        VAO->bind();
        glBindBuffer(GL_ARRAY_BUFFER, Buffers->ID[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        /*glBindBuffer(GL_ARRAY_BUFFER, Buffers->ID[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);*/
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers->ID[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    virtual void render() const
    {
        VAO->bind();
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

};
