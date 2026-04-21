#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
class ResourceManager {
public:
	ResourceManager() {
		std::cout << "Loading Main resources:" << std::endl;

		std::cout << "Loading arial.ttf.....";
		if (_font.openFromFile("arial.ttf")) {
			std::cout << "SUCCESS" << std::endl;
		}
		else {
			std::cout << "FAILURE" << std::endl;
		}
	}

	sf::Font& main_font() {
		return _font;
	};
private:
	sf::Font _font;
};