#pragma once
#include "CollisionData.h"

class ICollideable {
public:
	virtual ~ICollideable() = default;
	virtual void on_collision(const CollisionData& collision_data) = 0;
};