#pragma once
#include <unordered_set>
#include "_types.h"
#include <SFML/Graphics.hpp>

class Team {
public:
	Team(IdType team_id, sf::Color team_color) : m_team_id{ team_id }, m_team_color{ team_color } {
		
	}

	void add_player(IdType player_id) {
		players.insert(player_id);
	}

	sf::Color color() {
		return m_team_color;
	}

	bool has_player(IdType player_id) {
		return players.count(player_id) > 0;
	}

	IdType team_id() {
		return m_team_id;
	}
private:
	std::unordered_set<int> players;
	IdType m_team_id;
	sf::Color m_team_color;
};