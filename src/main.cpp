#include <iostream>
#include <mutex>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Engine/Globals.h"
#include "Engine/Timer.h"
#include "Game/Game.h"

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

	window.setFramerateLimit(60);

	Game game{};

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			if (event->is<sf::Event::Resized>())
			{
				GRAPHIC_SETTINGS.SetScreenSize(window.getSize());
			}
		}

		Timer::Get().Update();

		game.Update();

		window.clear();

		game.Render(window);

		window.display();
	}
}

