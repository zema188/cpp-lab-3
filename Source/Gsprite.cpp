#include "gSprite.h"


void gSprite::Draw(sf::Vector2u point, sf::Color gradientColor, RClass &r, int direction)
{
	std::vector<std::vector<float>> UM(600, std::vector<float>(600));
	while (point.x > 0 && point.y > 0 && point.x < _texture.getSize().x && point.y < _texture.getSize().y &&
		   !UM[point.x][point.y])
	{
		float cx = r._cx[point.x][point.y];
		float cy = r._cy[point.x][point.y];

		sf::RectangleShape currentStep({2, 2});
		currentStep.setFillColor(gradientColor);
		currentStep.setPosition(point.x, _texture.getSize().y - point.y);
		_texture.draw(currentStep);
		UM[point.x][point.y] = true;

		point.x += static_cast<unsigned>(10 * cx * direction);
		point.y += static_cast<unsigned>(10 * cy * direction);
	}
}

void gSprite::Create(const sf::Vector2u &size)
{
	_texture.create(size.x, size.y);
	setTexture(_texture.getTexture());
}

void gSprite::Clear() { _texture.clear(sf::Color::Transparent); }