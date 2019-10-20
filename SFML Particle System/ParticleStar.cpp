#include "pch.h"
#include "ParticleStar.h"
#include "stdio.h"

ParticleStar::ParticleStar()
{
	this->particle_sprite.setScale(0.05f, 0.05f);
}

void ParticleStar::draw_particle(sf::RenderWindow* window)
{
	window->draw(this->particle_sprite);
}


ParticleStar::~ParticleStar()
{
}
