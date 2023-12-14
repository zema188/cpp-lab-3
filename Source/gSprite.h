#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "RClass.h"

class gSprite : public sf::Sprite
{
public:
	void Create(const sf::Vector2u &size);
	void Clear();
	void Draw(sf::Vector2u point, sf::Color gradientColor, RClass &r, int direction = 1);

private:
	sf::RenderTexture _texture;
};