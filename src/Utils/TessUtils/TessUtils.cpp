#include "TessUtils.hpp"
#include "../../Lyra/SDK/SDK.hpp"

static MinecraftUIRenderContext *renderCtx = nullptr;
static mce::MaterialPtr *uiMaterial = nullptr;
static ScreenContext *screenContext = nullptr;
static Tessellator *tessellator = nullptr;
Vec3 origin;

void TessUtils::update(MinecraftUIRenderContext *renderContext) {
    if(!SDK::clientInstance) return;
    renderCtx = renderContext;
    screenContext = renderContext->getscreenContext();
    tessellator = screenContext->getTessellator();

    if (SDK::clientInstance->getLevelRenderer() != nullptr)
        origin = SDK::clientInstance->getLevelRenderer()->getOrigin();

    if (uiMaterial == nullptr)
        uiMaterial = mce::MaterialPtr::createMaterial(HashedString("selection_overlay_double_sided"));
}

void TessUtils::updateLevel(ScreenContext *screenCtx) {
    if(!SDK::clientInstance) return;
    screenContext = screenCtx;
    tessellator = screenContext->getTessellator();

    if (SDK::clientInstance->getLevelRenderer() != nullptr)
        origin = SDK::clientInstance->getLevelRenderer()->getOrigin();
}

ScreenContext *TessUtils::getScreenContext() {
    return screenContext;
}

mce::MaterialPtr *TessUtils::getUIMaterial() {
    return uiMaterial;
}

Tessellator *TessUtils::getTessellator() {
    return tessellator;
}

void TessUtils::render() {
    if (tessellator == nullptr)
        return;

    tessellator->renderMesh(screenContext, uiMaterial);
}

void TessUtils::begin(const VertexFormat format) {
    if (tessellator == nullptr)
        return;

    tessellator->begin(format);
}

void TessUtils::drawBox3D(const Vec3 &lower, const Vec3 &upper, const float scale, const mce::Color &color) {
    begin(VertexFormat::LINE_LIST);
    addBox3D(lower, upper, scale, color);
    render();
}

void TessUtils::addBox3D(const Vec3 &lower, const Vec3 &upper, const float scale, const mce::Color &color) {
    if (tessellator == nullptr || screenContext == nullptr || uiMaterial == nullptr)
        return;

    Vec3 diff;
    diff.x = upper.x - lower.x;
    diff.y = upper.y - lower.y;
    diff.z = upper.z - lower.z;

    const auto newLower = lower.sub(origin);

    Vec3 vertices[8];
    vertices[0] = Vec3(newLower.x, newLower.y, newLower.z);
    vertices[1] = Vec3(newLower.x + diff.x, newLower.y, newLower.z);
    vertices[2] = Vec3(newLower.x, newLower.y, newLower.z + diff.z);
    vertices[3] = Vec3(newLower.x + diff.x, newLower.y, newLower.z + diff.z);

    vertices[4] = Vec3(newLower.x, newLower.y + diff.y, newLower.z);
    vertices[5] = Vec3(newLower.x + diff.x, newLower.y + diff.y, newLower.z);
    vertices[6] = Vec3(newLower.x, newLower.y + diff.y, newLower.z + diff.z);
    vertices[7] = Vec3(newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z);

    const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
    const Vec3 newLowerReal = newLower.add(0.5f, 0.5f, 0.5f);

    for (auto& vertex : vertices) {
        const glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex.x - newLowerReal.x, vertex.y - newLowerReal.y, vertex.z - newLowerReal.z, 0.0f);
        vertex = Vec3{rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z};
    }

    tessellator->color(color);

#define line(m, n)                 \
    tessellator->vertex(m.x, m.y, m.z); \
    tessellator->vertex(n.x, n.y, n.z);

    line(vertices[4], vertices[5]);
    line(vertices[5], vertices[7]);
    line(vertices[7], vertices[6]);
    line(vertices[6], vertices[4]);

    line(vertices[0], vertices[1]);
    line(vertices[1], vertices[3]);
    line(vertices[3], vertices[2]);
    line(vertices[2], vertices[0]);

    line(vertices[0], vertices[4]);
    line(vertices[1], vertices[5]);
    line(vertices[2], vertices[6]);
    line(vertices[3], vertices[7]);
#undef line
}
