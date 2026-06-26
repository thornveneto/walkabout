#pragma once
#include "MovingEntity.h"
#include "../math/IJ.h"
#include "../physics/ICollideable.h"
#include "../_types.h"
#include <memory>
#include <SFML/Graphics.hpp>

class GameWorld;
class WorldRenderer;
struct CollisionData;
class Weapon;

class Unit : public MovingEntity, public ICollideable {
public:
	Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, IdType id, sf::Color color);

	virtual ~Unit();

	/*
		Weapon management
	*/

	void equip_main_weapon(bool is_melee, int action_points);//TODO: add weapon props
	void equip_aux_weapon(bool is_melee, int action_points); //TODO: add weapon props

	void activate_main_weapon();
	void activate_aux_weapon();
	Weapon* active_weapon();

	bool is_main_weapon_active();
	bool is_aux_weapon_active();

	void de_activate_main_weapon();
	void de_activate_aux_weapon();

	/*
		Selection Management
	*/
	void select();

	void deselect();

	/*
		Misc
	*/
	void draw(WorldRenderer& world_renderer) override;

	void attack_at(IJ target_cell, WorldRenderer& world_renderer);

	IdType id() const;

	void on_collision(const CollisionData& collision_data);

	void apply_damage(int damage_level);

	bool is_alive() const;

	int health() const;
	int max_health() const;

	int action_points() const;
	int max_action_points() const;
	void reduce_action_points(int new_value);
	void reset_action_points();
	bool enough_action_points(int needed_action_points);

	virtual void update(sf::Time& deltaTime, WorldRenderer& world_renderer) override;

	bool any_more_updates();
private:
	bool m_is_selected{ false };
	IdType m_id{ -1 };

	const int MAX_HEALTH{ 100 };
	int m_health{ MAX_HEALTH };

	const int MAX_ACTION_POINTS{ 5 };
	int m_action_points{ MAX_ACTION_POINTS };

	std::unique_ptr<Weapon> m_main_weapon;
	std::unique_ptr<Weapon> m_aux_weapon;
	Weapon* m_active_weapon{};

	sf::Color color;

	std::optional<sf::Texture> m_texture;
	std::optional<sf::Sprite> m_sprite;
	sf::Image m_image;
};