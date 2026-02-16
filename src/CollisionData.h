#pragma once
#include "_types.h"
struct CollisionData {
	//TODO: enrich with more meaningfull information
	IdType collided_entity_id{};
	int damage_power{ 0 };
};