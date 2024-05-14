#pragma once

struct Vec2 {
    float x, y;

    Vec2(float x = 0.f, float y = 0.f);

    [[nodiscard]] Vec2 add(float val) const;
    [[nodiscard]] Vec2 add(const Vec2& vec) const;
    [[nodiscard]] Vec2 add(float x, float y) const;

    [[nodiscard]] Vec2 sub(float val) const;
    [[nodiscard]] Vec2 sub(const Vec2& vec) const;
    [[nodiscard]] Vec2 sub(float x, float y) const;

    [[nodiscard]] Vec2 mul(float val) const;
    [[nodiscard]] Vec2 mul(const Vec2& vec) const;
    [[nodiscard]] Vec2 mul(float x, float y) const;

    [[nodiscard]] Vec2 div(float val) const;
    [[nodiscard]] Vec2 div(const Vec2& vec) const;
    [[nodiscard]] Vec2 div(float x, float y) const;

    [[nodiscard]] float dist(const Vec2& vec) const;

    [[nodiscard]] float len() const;

    bool operator==(const Vec2& o) const;
};

struct BlockPos;

struct Vec3 {
    float x, y, z;

    Vec3(float x = 0.f, float y = 0.f, float z = 0.f);

    [[nodiscard]] Vec3 add(float val) const;
    [[nodiscard]] Vec3 add(const Vec3& vec) const;
    [[nodiscard]] Vec3 add(float x, float y, float z) const;

    [[nodiscard]] Vec3 sub(float val) const;
    [[nodiscard]] Vec3 sub(const Vec3& vec) const;
    [[nodiscard]] Vec3 sub(float x, float y, float z) const;

    [[nodiscard]] Vec3 mul(float val) const;
    [[nodiscard]] Vec3 mul(const Vec3& vec) const;
    [[nodiscard]] Vec3 mul(float x, float y, float z) const;

    [[nodiscard]] Vec3 div(float val) const;
    [[nodiscard]] Vec3 div(const Vec3& vec) const;
    [[nodiscard]] Vec3 div(float x, float y, float z) const;

    [[nodiscard]] float dist(const Vec3& vec) const;

    [[nodiscard]] float len() const;

    [[nodiscard]] BlockPos toBlockPos() const;

    bool operator==(const Vec3& o) const;
};

struct BlockPos {
    int x, y, z;

    BlockPos(int x = 0, int y = 0, int z = 0);

    [[nodiscard]] BlockPos add(int val) const;
    [[nodiscard]] BlockPos add(const BlockPos& pos) const;
    [[nodiscard]] BlockPos add(int x, int y, int z) const;

    [[nodiscard]] BlockPos sub(int val) const;
    [[nodiscard]] BlockPos sub(const BlockPos& pos) const;
    [[nodiscard]] BlockPos sub(int x, int y, int z) const;

    [[nodiscard]] BlockPos mul(int val) const;
    [[nodiscard]] BlockPos mul(const BlockPos& pos) const;
    [[nodiscard]] BlockPos mul(int x, int y, int z) const;

    [[nodiscard]] BlockPos div(int val) const;
    [[nodiscard]] BlockPos div(const BlockPos& pos) const;
    [[nodiscard]] BlockPos div(int x, int y, int z) const;

    [[nodiscard]] float dist(const BlockPos& pos) const;

    [[nodiscard]] float len() const;

    [[nodiscard]] Vec3 toVec3() const;

    bool operator==(const BlockPos& o) const;
};

struct Vec4 {
    float x, y, z, w;

    Vec4(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f);

    [[nodiscard]] Vec4 add(float val) const;
    [[nodiscard]] Vec4 add(const Vec4& vec) const;
    [[nodiscard]] Vec4 add(float x, float y, float z, float w) const;

    [[nodiscard]] Vec4 sub(float val) const;
    [[nodiscard]] Vec4 sub(const Vec4& vec) const;
    [[nodiscard]] Vec4 sub(float x, float y, float z, float w) const;

    [[nodiscard]] Vec4 mul(float val) const;
    [[nodiscard]] Vec4 mul(const Vec4& vec) const;
    [[nodiscard]] Vec4 mul(float x, float y, float z, float w) const;

    [[nodiscard]] Vec4 div(float val) const;
    [[nodiscard]] Vec4 div(const Vec4& vec) const;
    [[nodiscard]] Vec4 div(float x, float y, float z, float w) const;

    [[nodiscard]] float dist(const Vec4& vec) const;

    [[nodiscard]] float len() const;

    bool operator==(const Vec4& o) const;
};

struct AABB {
    Vec3 lower;
    Vec3 upper;

    AABB() {}
    AABB(const Vec3& l, const Vec3& h) : lower(l), upper(h) {}
    AABB(const AABB &aabb);
    AABB(Vec3 lower, float width, float height, float eyeHeight);

    bool operator==(const AABB& o) const;

    bool isFullBlock() const;

    AABB expanded(float amount) const;
    AABB expandedXZ(float amount) const;

    Vec3 centerPoint() const;

    bool intersects(const AABB& aabb) const;
    bool intersectsXZ(const AABB& aabb) const;
};
