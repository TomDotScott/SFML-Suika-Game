#include <iostream>
#include <mutex>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Engine/Globals.h"
#include "Engine/Timer.h"
#include "Game/Fruit.h"

GraphicSettings GRAPHIC_SETTINGS{};
RandomRangeGenerator RNG = RandomRangeGenerator(0.0, 1.0);

int main(int argc, char** argv)
{
	sf::RenderWindow window(
#if BUILD_DEBUG
		sf::VideoMode({ 1280, 720 }),
#else
		sf::VideoMode::getDesktopMode(),
#endif
		"Suika Game Clone",
#if BUILD_DEBUG
		sf::State::Windowed
#else
		sf::State::Fullscreen
#endif
	);

	GRAPHIC_SETTINGS.SetScreenSize(window.getSize());

	window.setVerticalSyncEnabled(true);

	std::vector<Fruit> fruitCollection;

	constexpr int amountOfFruit = 100;

	fruitCollection.reserve(amountOfFruit);

	for (int i = 0; i < amountOfFruit; ++i)
	{
		fruitCollection.emplace_back(static_cast<Fruit::eFruitType>(i), static_cast<sf::Vector2f>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre));
	}


#if !MASTER_BUILD
	const sf::Font defaultFont{ "fonts/FiraCode-Regular.ttf" };
	sf::Text fpsText(defaultFont, "", 10);
	fpsText.setPosition({ 0, 0 });
	fpsText.setFillColor(sf::Color::White);
	uint16_t frameCount = 0;
#endif

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		Timer::Get().Update();

		for (auto& fruit : fruitCollection)
		{
			fruit.Update();
		}

		window.clear();

		for (const auto& fruit : fruitCollection)
		{
			fruit.Render(window);
		}

#if !BUILD_MASTER
		if (++frameCount % 2 == 0)
		{
			fpsText.setString(std::to_string(static_cast<int>(Timer::Get().Fps())) + "fps");
		}

		window.draw(fpsText);
#endif
		window.display();
	}
}

