#pragma once

#include "VertexFormat.hpp"
#include "../Core/mce.hpp"

#include "../../glm-src/glm/glm.hpp"
#include "../../glm-src/glm/ext/matrix_transform.hpp"

class Tessellator {
public:
    void begin(VertexFormat vertexFormat = VertexFormat::TRIANGLE_STRIP);
    void vertex(float x, float y, float z);
    void color(const mce::Color& color);
    void color(float r, float g, float b, float a);
    void renderMesh(class ScreenContext *screenContext, mce::MaterialPtr *material);
};
