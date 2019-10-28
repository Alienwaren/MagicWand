// SFML Particle System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "ParticleSystem.h"
#include "ParticleStar.h"
#include <opencv2/opencv.hpp>
#include "ColorDetector.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Helpers.h"
int main()
{
	cv::setUseOptimized(true);


	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(cv::CAP_PROP_FPS, 30);
	int low_h = 19;
	int high_h = 94;

	int low_s = 72;
	int high_s = 255;

	int low_v = 97;
	int high_v = 255;

	namedWindow("Control", cv::WINDOW_AUTOSIZE); //create a window called "Control"

	cv::createTrackbar("LowH", "Control", &low_h, 179); //Hue (0 - 179)
	cv::createTrackbar("HighH", "Control", &high_h, 179);

	cv::createTrackbar("LowS", "Control", &low_s, 255); //Saturation (0 - 255)
	cv::createTrackbar("HighS", "Control", &high_s, 255);

	cv::createTrackbar("LowV", "Control", &low_v, 255); //Value (0 - 255)
	cv::createTrackbar("HighV", "Control", &high_v, 255);

	ColorDetector detector(19, 72, 97, 25, 255, 255);

	if (!cap.isOpened())
		return -1;

	const int width = 1280;
	const int height = 720;

	sf::RenderWindow window(sf::VideoMode(width, height, 32), "Magic");
	window.setVerticalSyncEnabled(true);
	sf::Event events;
	sf::Font font;
	if (!font.loadFromFile("CONSOLA.TTF")) return 1;
	sf::Text text("", font, 10);
	text.setPosition(10.0f, 10.0f);

	ParticleSystem particleSystem(width, height);
	particleSystem.setGravity(0, 1);
	particleSystem.setParticleSpeed(200.0f);
	particleSystem.setDissolve(true);
	particleSystem.setDissolutionRate(1);
	particleSystem.setShape(Shape::SQUARE);
	sf::Image img;
	cv::Mat frame, RGBA;
	sf::Sprite cap_sprite;
	sf::Texture capture_texture;
	std::tuple<int, int> old_pos = std::tuple<int, int>(0, 0);
	while (window.isOpen())
	{
		cap >> frame;
		if (frame.empty())
		{
			continue;
		}
		cvtColor(frame, RGBA, cv::COLOR_BGR2RGBA);
		unsigned char* pixels = RGBA.ptr(0);
		img.create(RGBA.cols, RGBA.rows, pixels);
		if (!capture_texture.loadFromImage(img))
		{
			return -2;
		}
		cap_sprite.setTexture(capture_texture);
		cv::Mat detected_colors = detector.detect_color(frame);
		detector.set_hsv(high_h, high_s, high_v, low_h, low_s, low_v);

		std::tuple<int, int> coords = detector.make_image_moments(detected_colors);
		if (is_outside_tolerance(coords, old_pos, 10))
		{
			particleSystem.setPosition(std::get<0>(old_pos), std::get<1>(old_pos));
			particleSystem.fuel(10);
		}

		if(coords != old_pos)
		{
			old_pos = coords;
		}

		while (window.pollEvent(events))
		{
			switch (events.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (events.key.code == sf::Keyboard::Escape)
					window.close();
				if (events.key.code == sf::Keyboard::S)
				{
					particleSystem.enabled = !particleSystem.enabled;
				}
				break;
			}
		}
		particleSystem.update();
		text.setString(particleSystem.getNumberOfParticlesString());
		window.clear(sf::Color::Black);
		window.draw(cap_sprite);
		window.draw(text);
		//window.draw(detection_center);
		particleSystem.render(&window);
		window.display();
	}

	return 0;
}
