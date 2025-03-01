/// <summary>
/// author Jake Dowd Gilligan 2025 
/// you need to change the above line or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ WIDTH, HEIGHT, 32U }, "AirCrash by Jake" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupPlanes(); //load planes 
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
#ifdef _DEBUG
			render(); // want to debug drawing while stepping through code
#endif // _DEBUG

		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processMouseDown(newEvent);
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouseUp(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}

	if (sf::Keyboard::F1 == t_event.key.code)
	{
		m_debugging = !m_debugging;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (t_deltaTime.asMilliseconds() != 16)
	{
		std::cout << "time warp" << std::endl; // expecting 60 fps of action
	}
	if (m_exitGame)
	{
		m_window.close();
	}
	movePlanes();
	keepOnScreen(m_smallPlaneLocation);
	keepOnScreen(m_bigPlaneLocation);
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_skySprite);
	m_window.draw(m_bigPlaneSprite);
	m_window.draw(m_smallPlaneSprite);
	m_window.display();
	if (m_debugging)
	{
		drawPlane(m_bigPlaneSprite);
		drawPlane(m_smallPlaneSprite);
	}
	
}

void Game::processMouseDown(sf::Event t_event)
{
	m_mouseDown.x = static_cast<float> (t_event.mouseButton.x);
	m_mouseDown.y = static_cast<float> (t_event.mouseButton.y);
}

void Game::processMouseUp(sf::Event t_event)
{
	sf::Vector2f mouseUp;
	sf::Vector2f displacement;
	float heading;
	float headingD;
	float headingR;
	
	mouseUp.x = static_cast<float> (t_event.mouseButton.x);
	mouseUp.y = static_cast<float> (t_event.mouseButton.y);

	displacement = mouseUp - m_mouseDown;
	headingR = std::atan2f(displacement.y, displacement.x);
	headingD = headingR * 180.0f / M_PI;
	headingD = headingD + 90.0f;

	if (sf::Mouse::Left == t_event.mouseButton.button)
	{
		m_bigHeading = headingD;
		m_bigPlaneVelocity = displacement / 100.0f;
		m_bigPlaneSprite.setRotation(headingD);
	}
	if (sf::Mouse::Right == t_event.mouseButton.button)
	{
		m_smallHeading = headingD;
		m_smallPlaneVelocity = displacement / 50.0f;
		m_smallPlaneSprite.setRotation(headingD);
	}


}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	setupSky();

}

void Game::setupSky()
{
	if (!m_skyTexture.loadFromFile("ASSETS\\IMAGES\\sky.jpg"))
	{
		std::cout << "could not load file for background" << std::endl;
	}
	m_skyTexture.setRepeated(true);
	m_skySprite.setTexture(m_skyTexture);
	m_skySprite.setTextureRect(sf::IntRect{ 0,0,WIDTH,HEIGHT });
}

void Game::setupPlanes()
{
	sf::IntRect bigRectangle{ 3,11,104,93 }; //area of image for big plane 
	sf::IntRect smallRectangle{ 362,115,87,69 }; //area of image of small plane 

	if (!m_planesTexture.loadFromFile("ASSETS\\IMAGES\\planes.png"))
	{
		std::cout << "could not load file for planes" << std::endl;
	}

	m_bigPlaneSprite.setTexture(m_planesTexture);
	m_bigPlaneSprite.setTextureRect(bigRectangle);
	m_bigPlaneSprite.setOrigin(bigRectangle.width / 2.0f, bigRectangle.height / 2.0f);
	m_bigPlaneSprite.setPosition(m_bigPlaneLocation);
	m_bigPlaneSprite.setRotation(m_bigHeading);

	m_smallPlaneSprite.setTexture(m_planesTexture);
	m_smallPlaneSprite.setTextureRect(smallRectangle);
	m_smallPlaneSprite.setOrigin(smallRectangle.width / 2.0f, smallRectangle.height / 2.0f);
	m_smallPlaneSprite.setPosition(m_smallPlaneLocation);
	m_smallPlaneSprite.setRotation(m_smallHeading);
}

void Game::movePlanes()
{
	m_bigPlaneLocation += m_bigPlaneVelocity;
	m_bigPlaneSprite.setPosition(m_bigPlaneLocation);
	m_smallPlaneLocation += m_smallPlaneVelocity;
	m_smallPlaneSprite.setPosition(m_smallPlaneLocation);
}


///check for boundaries 
void Game::keepOnScreen(sf::Vector2f& t_location)
{
	float screenWidth = static_cast<float>(WIDTH);
	float screenHeight = static_cast<float>(HEIGHT);

	if (t_location.x < 0.0f)
	{
		t_location.x = 0.0f;
	}
	if (t_location.x > screenWidth)
	{
		t_location.x = screenWidth;
	}
	if (t_location.y < 0.0f)
	{
		t_location.y = 0.0f;
	}
	if (t_location.y > screenHeight)
	{
		t_location.y = screenHeight;
	}
}

void Game::drawPlane(sf::Sprite& t_plane)
{
	sf::CircleShape dot{ 4.0f };
	dot.setFillColor(sf::Color::Yellow);
	dot.setOrigin(4.0f, 4.0f);
	dot.setPosition(t_plane.getPosition());
	m_window.draw(dot);
}
