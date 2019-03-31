#pragma once

enum class ComponentType : int
{
	COMPONENT_NONE = 0,
	COMPONENT_AI = 1 << 1,
	COMPONENT_AUDIO = 1 << 2,
	COMPONENT_BOXCOLLIDER = 1 << 3,
	COMPONENT_CAMERA = 1 << 4,
	COMPONENT_COLOUR = 1 << 5,
	COMPONENT_GEOMETRY = 1 << 6,
	COMPONENT_GRAVITY = 1 << 7,
	COMPONENT_LIGHT = 1 << 8,
	COMPONENT_RAY = 1 << 9,
	COMPONENT_SHADER = 1 << 10,
	COMPONENT_SPHERECOLLIDER = 1 << 11,
	COMPONENT_TEXTURE = 1 << 12,
	COMPONENT_TRANSFORM = 1 << 13,
	COMPONENT_VELOCITY = 1 << 14,
};

inline ComponentType operator| (const ComponentType& lhs, const ComponentType& rhs) { return static_cast<ComponentType>(static_cast<int>(lhs) | static_cast<int>(rhs)); }

inline ComponentType operator& (const ComponentType& lhs, const ComponentType& rhs) { return static_cast<ComponentType>(static_cast<int>(lhs) & static_cast<int>(rhs)); }

inline ComponentType& operator|= (ComponentType& lhs, const ComponentType& rhs) { return lhs = lhs | rhs; }

inline ComponentType& operator&= (ComponentType& lhs, const ComponentType& rhs) { return lhs = lhs & rhs; }

inline ComponentType operator~(const ComponentType& rhs) { return static_cast<ComponentType>(~static_cast<int>(rhs)); }