#ifndef PARTICLESTAR_H
#define PARTICLESTAR_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class ParticleStar
{
public:
	ParticleStar();
	~ParticleStar();
	void draw_particle(sf::RenderWindow* window);
	sf::Sprite particle_sprite;
};

#endif // PARTICLESTAR_H
