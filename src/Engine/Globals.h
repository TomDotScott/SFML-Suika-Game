#ifndef GLOBALS_H
#define GLOBALS_H
#include <random>
#include <SFML/System/Vector2.hpp>

class GraphicSettings
{
public:
	void SetScreenSize(const sf::Vector2u& screenSize) { m_screenSize = screenSize; }
	const sf::Vector2u& GetScreenSize() const { return m_screenSize; }
	sf::Vector2u GetScreenSize() { return m_screenSize; }

private:
	sf::Vector2u m_screenSize;
};

class RandomRangeGenerator
{
public:
	RandomRangeGenerator(double min, double max);

	double Next() { return m_dist(m_randomEngine); }

private:
	std::uniform_real_distribution<double> m_dist;
	std::random_device m_randomDevice;
	std::default_random_engine m_randomEngine;
};

extern GraphicSettings GRAPHIC_SETTINGS;

#endif
