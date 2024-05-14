#pragma once
#include <entt/entt.hpp>

class EntityId;

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type  = uint32_t;
    using version_type = uint16_t;

    static constexpr entity_type entity_mask  = 0x3FFFF;
    static constexpr entity_type version_mask = 0x3FFF;
};

template <>
class entt::entt_traits<EntityId> : public entt::basic_entt_traits<EntityIdTraits> {
public:
    static constexpr entity_type page_size = 2048;
};

class EntityId : public entt::entt_traits<EntityId> {
public:
    entity_type mRawId{};

    template <std::integral T>
        requires(!std::is_same_v<std::remove_cvref_t<T>, bool>)
    [[nodiscard]] constexpr EntityId(T rawId) : mRawId(static_cast<entity_type>(rawId)) {} // NOLINT

    [[nodiscard]] constexpr bool isNull() const { return *this == entt::null; }

    [[nodiscard]] constexpr operator entity_type() const { return mRawId; }

    constexpr bool operator==(const EntityId& other) const {
        return mRawId == other.mRawId;
    }
};

class EntityRegistry : public std::enable_shared_from_this<EntityRegistry> {
public:
    std::string name;
    entt::basic_registry<EntityId> ownedRegistry;
    uint32_t id;
};

class EntityContext {
public:
    EntityRegistry& registry;
    entt::basic_registry<EntityId>& enttRegistry;
    EntityId entity;
};