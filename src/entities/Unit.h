#pragma once
#include "MovingEntity.h"
#include "../math/IJ.h"
#include "../physics/ICollideable.h"
#include "../_types.h"
#include <memory>

class GameWorld;
class WorldRenderer;
struct CollisionData;
class Weapon;

class Unit : public MovingEntity, public ICollideable {
public:
	Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, IdType id);

	virtual ~Unit();

	void equip_main_weapon(bool is_melee);//TODO: add weapon props
	void equip_aux_weapon(bool is_melee); //TODO: add weapon props

	void activate_main_weapon();
	void activate_aux_weapon();
	Weapon* active_weapon();

	void select();

	void deselect();

	void draw(WorldRenderer& world_renderer) override;

	void attack_at(IJ target_cell, WorldRenderer& world_renderer);

	IdType id() const;

	void on_collision(const CollisionData& collision_data);

	void apply_damage(int damage_level);

	bool is_alive() const;

	int health() const;

	int max_health() const;

	virtual void update(sf::Time& deltaTime, WorldRenderer& world_renderer) override;

	bool any_more_updates();
private:
	bool _is_selected{ false };
	IdType _id{ -1 };

	const int _MAX_HEALTH{ 100 };
	int _health{ _MAX_HEALTH };

	std::unique_ptr<Weapon> _main_weapon;
	std::unique_ptr<Weapon> _aux_weapon;
	Weapon* _active_weapon{};
};