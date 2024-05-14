#include "Vector.hpp"
#include <math.h>

Vec2::Vec2(const float x, const float y) {
    this->x = x;
    this->y = y;
}

Vec2 Vec2::add(const float val) const {
    return Vec2(this->x + val, this->y + val);
}

Vec2 Vec2::add(const Vec2 &vec) const {
    return Vec2(this->x + vec.x, this->y + vec.y);
}

Vec2 Vec2::add(const float x, const float y) const {
    return Vec2(this->x + x, this->y + y);
}

Vec2 Vec2::sub(const float val) const {
    return Vec2(this->x - val, this->y - val);
}

Vec2 Vec2::sub(const Vec2 &vec) const {
    return Vec2(this->x - vec.x, this->y - vec.y);
}

Vec2 Vec2::sub(const float x, const float y) const {
    return Vec2(this->x - x, this->y - y);
}

Vec2 Vec2::mul(const float val) const {
    return Vec2(this->x * val, this->y * val);
}

Vec2 Vec2::mul(const Vec2 &vec) const {
    return Vec2(this->x * vec.x, this->y * vec.y);
}

Vec2 Vec2::mul(const float x, const float y) const {
    return Vec2(this->x * x, this->y * y);
}

Vec2 Vec2::div(const float val) const {
    return Vec2(this->x / val, this->y / val);
}

Vec2 Vec2::div(const Vec2 &vec) const {
    return Vec2(this->x / vec.x, this->y / vec.y);
}

Vec2 Vec2::div(const float x, const float y) const {
    return Vec2(this->x / x, this->y / y);
}

float Vec2::dist(const Vec2 &vec) const {
    return sqrtf(powf(this->x - vec.x, 2) + powf(this->y - vec.y, 2));
}

float Vec2::len() const {
    return sqrtf(this->x * this->x + this->y * this->y);
}

bool Vec2::operator==(const Vec2& o) const {
    return this->x == o.x && this->y == o.y;
}

Vec3::Vec3(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 Vec3::add(const float val) const {
    return Vec3(this->x + val, this->y + val, this->z + val);
}

Vec3 Vec3::add(const Vec3 &vec) const {
    return Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

Vec3 Vec3::add(const float x, const float y, const float z) const {
    return Vec3(this->x + x, this->y + y, this->z + z);
}

Vec3 Vec3::sub(const float val) const {
    return Vec3(this->x - val, this->y - val, this->z - val);
}

Vec3 Vec3::sub(const Vec3 &vec) const {
    return Vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

Vec3 Vec3::sub(const float x, const float y, const float z) const {
    return Vec3(this->x - x, this->y - y, this->z - z);
}

Vec3 Vec3::mul(const float val) const {
    return Vec3(this->x * val, this->y * val, this->z * val);
}

Vec3 Vec3::mul(const Vec3 &vec) const {
    return Vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z);
}

Vec3 Vec3::mul(const float x, const float y, const float z) const {
    return Vec3(this->x * x, this->y * y, this->z * z);
}

Vec3 Vec3::div(const float val) const {
    return Vec3(this->x / val, this->y / val, this->z / val);
}

Vec3 Vec3::div(const Vec3 &vec) const {
    return Vec3(this->x / vec.x, this->y / vec.y, this->z / vec.z);
}

Vec3 Vec3::div(const float x, const float y, const float z) const {
    return Vec3(this->x / x, this->y / y, this->z / z);
}

float Vec3::dist(const Vec3 &vec) const {
    return sqrtf(powf(this->x - vec.x, 2) + powf(this->y - vec.y, 2) + powf(this->z - vec.z, 2));
}

float Vec3::len() const {
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

BlockPos Vec3::toBlockPos() const {
    return BlockPos(static_cast<int>(this->x), static_cast<int>(this->y), static_cast<int>(this->z));
}

bool Vec3::operator==(const Vec3& o) const {
    return this->x == o.x && this->y == o.y && this->z == o.z;
}

BlockPos::BlockPos(const int x, const int y, const int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

BlockPos BlockPos::add(const int val) const {
    return BlockPos(this->x + val, this->y + val, this->z + val);
}

BlockPos BlockPos::add(const BlockPos &pos) const {
    return BlockPos(this->x + pos.x, this->y + pos.y, this->z + pos.z);
}

BlockPos BlockPos::add(const int x, const int y, const int z) const {
    return BlockPos(this->x + x, this->y + y, this->z + z);
}

BlockPos BlockPos::sub(const int val) const {
    return BlockPos(this->x - val, this->y - val, this->z - val);
}

BlockPos BlockPos::sub(const BlockPos &pos) const {
    return BlockPos(this->x - pos.x, this->y - pos.y, this->z - pos.z);
}

BlockPos BlockPos::sub(const int x, const int y, const int z) const {
    return BlockPos(this->x - x, this->y - y, this->z - z);
}

BlockPos BlockPos::mul(const int val) const {
    return BlockPos(this->x * val, this->y * val, this->z * val);
}

BlockPos BlockPos::mul(const BlockPos &pos) const {
    return BlockPos(this->x * pos.x, this->y * pos.y, this->z * pos.z);
}

BlockPos BlockPos::mul(const int x, const int y, const int z) const {
    return BlockPos(this->x * x, this->y * y, this->z * z);
}

BlockPos BlockPos::div(const int val) const {
    return BlockPos(this->x / val, this->y / val, this->z / val);
}

BlockPos BlockPos::div(const BlockPos &pos) const {
    return BlockPos(this->x / pos.x, this->y / pos.y, this->z / pos.z);
}

BlockPos BlockPos::div(const int x, const int y, const int z) const {
    return BlockPos(this->x / x, this->y / y, this->z / z);
}

float BlockPos::dist(const BlockPos &pos) const {
    return sqrtf(powf(static_cast<float>(this->x) - static_cast<float>(pos.x), 2) + powf(static_cast<float>(this->y) - static_cast<float>(pos.y), 2) +
        powf(static_cast<float>(this->z) - static_cast<float>(pos.z), 2));
}

float BlockPos::len() const {
    return sqrtf(static_cast<float>(this->x) * static_cast<float>(this->x) + static_cast<float>(this->y) * static_cast<float>(this->y) +
        static_cast<float>(this->z) * static_cast<float>(this->z));
}

Vec3 BlockPos::toVec3() const {
    return Vec3(static_cast<float>(this->x), static_cast<float>(this->y), static_cast<float>(this->z));
}

bool BlockPos::operator==(const BlockPos& o) const {
    return this->x == o.x && this->y == o.y && this->z == o.z;
}

Vec4::Vec4(const float x, const float y, const float z, const float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vec4 Vec4::add(const float val) const {
    return Vec4(this->x + val, this->y + val, this->z + val, this->w + val);
}

Vec4 Vec4::add(const Vec4 &vec) const {
    return Vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
}

Vec4 Vec4::add(const float x, const float y, const float z, const float w) const {
    return Vec4(this->x + x, this->y + y, this->z + z, this->w + w);
}

Vec4 Vec4::sub(const float val) const {
    return Vec4(this->x - val, this->y - val, this->z - val, this->w - val);
}

Vec4 Vec4::sub(const Vec4 &vec) const {
    return Vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
}

Vec4 Vec4::sub(const float x, const float y, const float z, const float w) const {
    return Vec4(this->x - x, this->y - y, this->z - z, this->w - w);
}

Vec4 Vec4::mul(const float val) const {
    return Vec4(this->x * val, this->y * val, this->z * val, this->w * val);
}

Vec4 Vec4::mul(const Vec4 &vec) const {
    return Vec4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w);
}

Vec4 Vec4::mul(const float x, const float y, const float z, const float w) const {
    return Vec4(this->x * x, this->y * y, this->z * z, this->w * w);
}

Vec4 Vec4::div(const float val) const {
    return Vec4(this->x / val, this->y / val, this->z / val, this->w / val);
}

Vec4 Vec4::div(const Vec4 &vec) const {
    return Vec4(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w);
}

Vec4 Vec4::div(const float x, const float y, const float z, const float w) const {
    return Vec4(this->x / x, this->y / y, this->z / z, this->w / w);
}

float Vec4::dist(const Vec4 &vec) const {
    return sqrtf(powf(this->x - vec.x, 2) + powf(this->y - vec.y, 2) + powf(this->z - vec.z, 2) + powf(this->w - vec.w, 2));
}

float Vec4::len() const {
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

bool Vec4::operator==(const Vec4& o) const {
    return this->x == o.x && this->y == o.y && this->z == o.z && this->w == o.w;
}

AABB::AABB(const AABB& aabb) {
    this->lower = Vec3(aabb.lower);
    this->upper = Vec3(aabb.upper);
}

bool AABB::operator==(const AABB& o) const {
    return this->lower == o.lower && this->upper == o.upper;
}

AABB::AABB(Vec3 lower, const float width, const float height, const float eyeHeight) {
    lower = lower.sub(Vec3(width, eyeHeight * 2, width).div(2));
    this->lower = lower;
    this->upper = {lower.x + width, lower.y + height, lower.z + width};
}

bool AABB::isFullBlock() const {
    const auto diff = this->lower.sub(this->upper);
    return fabsf(diff.y) == 1 && fabsf(diff.x) == 1 && fabsf(diff.z) == 1;
}

AABB AABB::expanded(const float amount) const {
    return AABB(this->lower.sub(amount), this->upper.add(amount));
}

AABB AABB::expandedXZ(const float amount) const {
    return AABB(this->lower.sub(amount, 0.f, amount), this->upper.add(amount, 0.f, amount));
}

Vec3 AABB::centerPoint() const {
    const Vec3 diff = this->upper.sub(this->lower);
    return this->lower.add(diff.mul(0.5f));
}

bool AABB::intersects(const AABB& aabb) const {
    return aabb.upper.x > this->lower.x && this->upper.x > aabb.lower.x &&
           aabb.upper.y > this->lower.y && this->upper.y > aabb.lower.y &&
           aabb.upper.z > this->lower.z && this->upper.z > aabb.lower.z;
}

bool AABB::intersectsXZ(const AABB& aabb) const {
    return aabb.upper.x > this->lower.x && this->upper.x > aabb.lower.x &&
           aabb.upper.z > this->lower.z && this->upper.z > aabb.lower.z;
}
