#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include "imgui-SFML.h"
#include "imgui.h"


class RClass : public sf::Sprite
{
public:
	void Create(const sf::Vector2u &size, const int selectedNormalIndex);
	void DrawRFunc(const std::function<float(const sf::Vector2f &)> &rfunc, const sf::FloatRect &subSpace);
	void UpdatePalette(const sf::Color &firstColor, const sf::Color &secondColor);
	void SaveImageToFile(const std::string &filename);
	std::vector<std::vector<float>> _cx;
	std::vector<std::vector<float>> _cy;

private:
	sf::Color fColor;
	sf::Color sColor;
	sf::Texture texture;
	sf::Image image;
	int index;
};