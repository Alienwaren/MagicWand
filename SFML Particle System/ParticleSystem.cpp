#include "pch.h"
#include "ParticleSystem.h"
#include <sstream>

ParticleSystem::ParticleSystem(int width, int height)
{
	m_transparent = sf::Color(0, 0, 0, 0);
	m_image.create(width, height, m_transparent);
	m_texture.loadFromImage(m_image);
	m_sprite = sf::Sprite(m_texture);
	m_position.x = 0.5f * width;
	m_position.y = 0.5f * height;
	m_particleSpeed = 20.0f;
	m_dissolve = false;
	m_dissolutionRate = 4;
	m_shape = Shape::CIRCLE;
	if (!this->star_texture.loadFromFile("Textures/star.png"))
	{
		printf_s("Failed to load particle texture!\n");
	}
}

ParticleSystem::~ParticleSystem()
{
	for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		delete *it;
	}
}

void ParticleSystem::fuel(int particles)
{
	if(enabled)
	{
		float angle;
		for (int i = 0; i < particles; i++)
		{
			Particle* particle = new Particle();
			particle->pos.x = m_position.x;
			particle->pos.y = m_position.y;

			switch (m_shape)
			{
			case Shape::CIRCLE:
				angle = m_randomizer.rnd(0.0f, 6.2832f);
				particle->vel.x = m_randomizer.rnd(0.0f, cos(angle));
				particle->vel.y = m_randomizer.rnd(0.0f, sin(angle));
				break;
			case Shape::SQUARE:
				particle->vel.x = m_randomizer.rnd(-1.0f, 1.0f);
				particle->vel.y = m_randomizer.rnd(-1.0f, 1.0f);
				break;
			default:
				particle->vel.x = 0.5f; // Easily detected
				particle->vel.y = 0.5f; // Easily detected
			}
			particle->particle_star.particle_sprite.setTexture(this->star_texture);
			if (particle->vel.x == 0.0f && particle->vel.y == 0.0f)
			{
				delete particle;
				continue;
			}
			m_particles.push_back(particle);
		}
	}
}

void ParticleSystem::update()
{
	float time = m_clock.restart().asSeconds();

	for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		(*it)->vel.x += m_gravity.x * time;
		(*it)->vel.y += m_gravity.y * time;

		(*it)->pos.x += (*it)->vel.x * time * m_particleSpeed;
		(*it)->pos.y += (*it)->vel.y * time * m_particleSpeed;

		(*it)->particle_star.particle_sprite.setPosition((*it)->pos.x, (*it)->pos.y);

		if ((*it)->pos.x > m_image.getSize().x || (*it)->pos.x < 0 || (*it)->pos.y > m_image.getSize().y || (*it)->pos.y
			< 0)
		{
			delete (*it);
			it = m_particles.erase(it);
			if (it == m_particles.end()) return;
		}
	}
}

void ParticleSystem::render(sf::RenderWindow* window)
{
	for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		(*it)->particle_star.draw_particle(window);
	}
}

std::string ParticleSystem::getNumberOfParticlesString()
{
	std::ostringstream oss;
	oss << m_particles.size();
	return oss.str();
}
