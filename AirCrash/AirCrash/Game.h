/// <summary>
/// author Jake Dowd Gilligan 2025
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

const int HEIGHT = 800;//screen height 
const int WIDTH = 1000;//screen width

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void setupSky();
	void setupPlanes();

	void movePlanes();

	sf::Texture m_skyTexture;
	sf::Sprite m_skySprite;

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	
	sf::Texture m_logoTexture; // texture used for sfml logo

	sf::Texture m_planesTexture;//all planes texture 
	sf::Sprite m_bigPlaneSprite;//big plane sprite 
	sf::Vector2f m_bigPlaneVelocity{ 1.0f,-1.0f };//big plane velocity
	sf::Vector2f m_bigPlaneLocation{ 200.0f,200.0f };//big plane location 

	sf::Sprite m_smallPlaneSprite;//small plane sprite 
	sf::Vector2f m_smallPlaneVelocity{ -2.0f,2.0f };//small plane velocity
	sf::Vector2f m_smallPlaneLocation{ 600.0f,0.0f };//small plane location 


	float m_bigHeading{ 45.0f };//heading of big plane 
	float m_smallHeading{ 225.0f };//heading of small plane

	
	
	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

