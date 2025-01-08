#include <iostream>
#include <mutex>
#include <optional>
#include <SFML/Graphics.hpp>

#include "Engine/Globals.h"
#include "Engine/Timer.h"

GraphicSettings GRAPHIC_SETTINGS{};

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

	sf::CircleShape shape(100.f);

	RandomRangeGenerator rng(0.0, 1.0);
	sf::Vector2f dir{ static_cast<float>(rng.Next()), static_cast<float>(rng.Next()) };
	shape.setFillColor(sf::Color::Green);

	shape.setOrigin({ shape.getGlobalBounds().size.x / 2.f, shape.getGlobalBounds().size.y / 2.f });
	shape.setPosition(static_cast<sf::Vector2f>(GRAPHIC_SETTINGS.GetScreenSize()) / 2.f);


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

		window.clear();
		Timer::Get().Update();

		constexpr float speed = 500.f;

		shape.setPosition(shape.getPosition() + dir.normalized() * speed * Timer::Get().DeltaTime());

		const auto& newPosition = shape.getPosition();

		if (newPosition.x - shape.getRadius() < 0)
		{
			shape.setPosition({ shape.getRadius(), shape.getPosition().y });
			dir.x = -dir.x;
		}

		if (newPosition.x + shape.getRadius() > static_cast<float>(GRAPHIC_SETTINGS.GetScreenSize().x))
		{
			shape.setPosition({ static_cast<float>(GRAPHIC_SETTINGS.GetScreenSize().x) - shape.getRadius(), shape.getPosition().y });
			dir.x = -dir.x;
		}

		if (newPosition.y - shape.getRadius() < 0)
		{
			shape.setPosition({ shape.getPosition().x, shape.getRadius() });
			dir.y = -dir.y;
		}
		if (newPosition.y + shape.getRadius() > static_cast<float>(GRAPHIC_SETTINGS.GetScreenSize().y))
		{
			shape.setPosition({ shape.getPosition().x, static_cast<float>(GRAPHIC_SETTINGS.GetScreenSize().y) - shape.getRadius() });
			dir.y = -dir.y;
		}

		window.draw(shape);

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

