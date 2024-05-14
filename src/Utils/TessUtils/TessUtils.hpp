#pragma once

#include "../../Lyra/SDK/Client/Render/MinecraftUIRenderContext.hpp"
#include "../Vector.hpp"

class TessUtils {
public:
    static ScreenContext* getScreenContext();
    static mce::MaterialPtr* getUIMaterial();
    static Tessellator* getTessellator();

    static void update(MinecraftUIRenderContext* renderContext);
    static void updateLevel(ScreenContext* screenCtx);

    static void begin(VertexFormat format = VertexFormat::TRIANGLE_LIST);
    static void render();

    static void drawBox3D(const Vec3& lower, const Vec3& upper, float scale, const mce::Color& color = mce::Color());
    static void addBox3D(const Vec3& lower, const Vec3& upper, float scale, const mce::Color& color = mce::Color());
};
