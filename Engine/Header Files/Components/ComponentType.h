#pragma once

enum class ComponentType : short //?????????
{
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM = 1 << 0,
	COMPONENT_VELOCITY = 1 << 1,
	COMPONENT_BOXCOLLIDER = 1 << 2,
	COMPONENT_GEOMETRY = 1 << 3,
	COMPONENT_TEXTURE = 1 << 4,
	COMPONENT_SHADER = 1 << 5,
	COMPONENT_CAMERA = 1 << 6,
	COMPONENT_LIGHT = 1 << 7,
};

inline ComponentType operator| (const ComponentType& lhs, const ComponentType& rhs) { return static_cast<ComponentType>(static_cast<short>(lhs) | static_cast<short>(rhs)); }

inline ComponentType operator& (const ComponentType& lhs, const ComponentType& rhs) { return static_cast<ComponentType>(static_cast<short>(lhs) & static_cast<short>(rhs)); }

inline ComponentType& operator|= (ComponentType& lhs, const ComponentType& rhs) { return lhs = lhs | rhs; }

inline ComponentType& operator&= (ComponentType& lhs, const ComponentType& rhs) { return lhs = lhs & rhs; }

inline ComponentType operator~(const ComponentType& rhs) { return static_cast<ComponentType>(~static_cast<int>(rhs)); }