#include <SFML/Graphics.hpp>
#include <iostream>
#include "RClass.h"
#include "gSprite.h"
#include "imgui-SFML.h"
#include "imgui.h"

#include <array>
#include <functional>
#include <vector>

float calculateAnd(float weight1, float weight2)
{
	return weight1 + weight2 + std::sqrt((weight1 * weight1 + weight2 * weight2) - 2 * weight1 * weight2);
}

float calculateOr(float weight1, float weight2)
{
	return weight1 + weight2 - std::sqrt((weight1 * weight1 + weight2 * weight2) - 2 * weight1 * weight2);
}

int currentSpriteIndex = 0;
std::vector<RClass *> spriteObjects;

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "cpp-lab-3");
	window.setFramerateLimit(60);
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "ImGui initialization failed";
		return -1;
	}

	auto spriteSize = sf::Vector2u{window.getSize().x, window.getSize().y};

	RClass rClassNX;
	rClassNX.Create(spriteSize, 0);
	spriteObjects.push_back(&rClassNX);

	RClass rClassNY;
	rClassNY.Create(spriteSize, 1);
	spriteObjects.push_back(&rClassNY);

	RClass rClassNZ;
	rClassNZ.Create(spriteSize, 2);
	spriteObjects.push_back(&rClassNZ);

	RClass rClassNW;
	rClassNW.Create(spriteSize, 3);
	spriteObjects.push_back(&rClassNW);

	gSprite graphicSprite;
	graphicSprite.Create(sf::Vector2u(600, 600));

	std::function<float(const sf::Vector2f &)> rFunction[5];

	rFunction[0] = [](const sf::Vector2f &point) -> float { return std::sin(point.x) + std::cos(point.y); };
	rFunction[1] = [](const sf::Vector2f &point) -> float { return std::cos(point.x) * std::sin(point.y); };
	rFunction[2] = [](const sf::Vector2f &point) -> float { return std::cos(point.x + point.y); };
	rFunction[3] = [](const sf::Vector2f &point) -> float { return point.x * point.x + point.y * point.y - 200; };
	rFunction[4] = [](const sf::Vector2f &point) -> float { return std::sin(point.x) * std::cos(point.y); };

	std::function<float(const sf::Vector2f &)> complexFunction = [&rFunction](const sf::Vector2f &point) -> float
	{
		return calculateAnd(
			calculateAnd(calculateOr(calculateAnd(rFunction[0](point), rFunction[1](point)), rFunction[2](point)),
						 rFunction[3](point)),
			calculateOr(rFunction[4](point), rFunction[0](point)));
	};

	sf::FloatRect subSpace(-10.f, -10.f, 20.f, 20.f);

	static ImVec4 firstColor(0, 0, 0, 1);
	static ImVec4 secondColor(1, 1, 1, 1);

	for (int i = 0; i < spriteObjects.size(); i++)
	{
		spriteObjects[i]->DrawRFunc(complexFunction, subSpace);
	}

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					graphicSprite.Draw(sf::Vector2u(static_cast<unsigned>(event.mouseButton.x),
													static_cast<unsigned>(event.mouseButton.y)),
									   sf::Color::Yellow, *spriteObjects[0], 1);
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					graphicSprite.Draw(sf::Vector2u(static_cast<unsigned>(event.mouseButton.x),
													static_cast<unsigned>(event.mouseButton.y)),
									   sf::Color::Magenta, *spriteObjects[0], -1);
				}
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		static bool mChanged = false;
		ImGui::Begin("Controls");
		if (ImGui::Button("OX"))
		{
			currentSpriteIndex = 0;
			mChanged = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("OY"))
		{
			currentSpriteIndex = 1;
			mChanged = true;
		}

		if (ImGui::Button("OZ"))
		{
			currentSpriteIndex = 2;
			mChanged = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("OW"))
		{
			currentSpriteIndex = 3;
			mChanged = true;
		}

		if (ImGui::ColorEdit3("First color", &firstColor.x))
		{
		}
		if (ImGui::ColorEdit3("Second color", &secondColor.x))
		{
		}

		if (ImGui::Button("Update"))
		{
			auto sfFirstColor =
				sf::Color(static_cast<sf::Uint8>(firstColor.x * 255), static_cast<sf::Uint8>(firstColor.y * 255),
						  static_cast<sf::Uint8>(firstColor.z * 255), static_cast<sf::Uint8>(firstColor.w * 255));

			auto sfSecondColor =
				sf::Color(static_cast<sf::Uint8>(secondColor.x * 255), static_cast<sf::Uint8>(secondColor.y * 255),
						  static_cast<sf::Uint8>(secondColor.z * 255), static_cast<sf::Uint8>(secondColor.w * 255));

			for (RClass *sprite : spriteObjects)
			{
				sprite->UpdatePalette(sfFirstColor, sfSecondColor);
			}
		}
		if (ImGui::Button("Clear gradient"))
		{
			graphicSprite.Clear();
		}

		ImGui::Text("Press 'A' to save nx,ny,nz,nw images");

		if (event.key.code == sf::Keyboard::S)
		{
			spriteObjects[0]->SaveImageToFile("images/nx.png");
			spriteObjects[1]->SaveImageToFile("images/ny.png");
			spriteObjects[2]->SaveImageToFile("images/nz.png");
			spriteObjects[3]->SaveImageToFile("images/nw.png");
		}

		ImGui::End();

		window.clear();
		if (mChanged)
		{
			graphicSprite.Clear();
			mChanged = false;
		}
		if (currentSpriteIndex == 0)
		{
			window.draw(rClassNX);
		}
		else if (currentSpriteIndex == 1)
		{
			window.draw(rClassNY);
		}
		else if (currentSpriteIndex == 2)
		{
			window.draw(rClassNZ);
		}
		else if (currentSpriteIndex == 3)
		{
			window.draw(rClassNW);
		}
		window.draw(graphicSprite);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}
