#include "game.h"
#include <bitset>
#include <iostream>

Game::Game()
	: screenDimensions(1920, 1080),
	window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Doomed Labyrinth of Doom", sf::Style::Fullscreen),
	maze(window),
	player(window, sf::seconds(0.2)),
	backgroundColor(sf::Color::Black)
{
    window.setFramerateLimit(60);
}

Game::~Game()
{

}

int Game::exec()
{
    loadContent();
    initialize();

    while (window.isOpen())
    {
        manageEvents();

        //restart the clock and keep the elapsed time stored in frameTime variable
        sf::Time frameTime = frameClock.restart();

        // update game components
        update(frameTime);

        // draw game components
        draw();
    }

    return 0;
}

void Game::loadContent()
{
    //Load ressources here

    //ressources for labyrinth
    wallTexture.loadFromFile("wall.png");
    wallTexture.setSmooth(true);
    pathTexture.loadFromFile("path.png");
    pathTexture.setSmooth(true);

    //ressources for player
    playerTexture.loadFromFile("player.png");
    playerTexture.setSmooth(true);

    //ressources for audio
    music.openFromFile("menu.ogg");

	//ressources for victory
	victoryFont.loadFromFile("showg.ttf");

	//ressources for menu
	menuFont.loadFromFile("arial.ttf");
}

void Game::initialize()
{
    //Create game components here

    //initializing labyrinth
    maze.initialize(10,5 ,wallTexture,pathTexture);

    //initializing player
    player.initialize(playerTexture, maze.getMoving());

    //initializing audio
    music.play();

	//initializing victory
	victoryMessage.setString("Victoire!");
	victoryMessage.setFont(victoryFont);
	victoryMessage.setColor(sf::Color::Black);
	victoryMessage.setCharacterSize(100);
	victoryMessage.setPosition(window.getSize().x / 2 - victoryMessage.getLocalBounds().width / 2,
		window.getSize().y / 3 - victoryMessage.getLocalBounds().height / 2);

	//initializing menu
	menuMessage.setString("Appuyez sur une touche\n"
		                  "      pour commencer");
	menuMessage.setFont(menuFont);
	menuMessage.setCharacterSize(50);
	menuMessage.setPosition(window.getSize().x / 2 - menuMessage.getLocalBounds().width / 2,
		window.getSize().y / 3 - menuMessage.getLocalBounds().height / 2);
}

void Game::update(sf::Time deltaTime)
{
    //Update game components here
	States oldState = currentState;
	currentState = nextState;

	switch (currentState)
	{
	case Game::Menu:
		break;
	case Game::InGame:
		maze.update(deltaTime);
		player.update(deltaTime);
		if (maze.getVictory())
			nextState = Game::Victory;
		break;
	case Game::Victory:
		if (oldState != Game::Victory)
			backgroundColor = sf::Color::White;
		break;
	case Game::Quitting:
		window.close();
		break;
	default:
		break;
	}
}

void Game::draw()
{
	window.clear(backgroundColor);
	//Draw game components here
	//example: window.draw(componentName);

	switch (currentState)
	{
	case Game::Menu:
		window.draw(menuMessage);
		break;
	case Game::InGame:
		window.draw(maze);
		window.draw(player);
		break;
	case Game::Victory:
		window.draw(victoryMessage);
		break;
	default:
		break;
	}
    window.display();
}

void Game::manageEvents()
{
	switch (currentState)
	{
	case Game::Menu:
		manageMenuEvents();
		break;
	case Game::InGame:
		manageFPGAEvents();
		manageKeysEvents();
		break;
	case Game::Victory:
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
				nextState = Game::Quitting;
			if (event.type == sf::Event::Closed)
				window.close();
		}
		break;
	default:
		break;
	}
	
}

void Game::manageFPGAEvents()
{
	int temp;
	fpga.lireRegistre(BTN, temp);
	std::bitset<4> btn{ (unsigned)temp };
	if (btn[0]) //right
	{
		player.move(Player::RIGHT);
		maze.move(Maze::RIGHT);
	}
	if (btn[1]) //down
	{
		player.move(Player::DOWN);
		maze.move(Maze::DOWN);
	}
	if (btn[2]) //up
	{
		player.move(Player::UP);
		maze.move(Maze::UP);
	}
	if (btn[3]) //left
	{
		player.move(Player::LEFT);
		maze.move(Maze::LEFT);
	}

}

void Game::manageKeysEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			window.close();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
		{
			player.move(Player::UP);
			maze.move(Maze::UP);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
		{
			player.move(Player::RIGHT);
			maze.move(Maze::RIGHT);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		{
			player.move(Player::DOWN);
			maze.move(Maze::DOWN);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		{
			player.move(Player::LEFT);
			maze.move(Maze::LEFT);
		}
	}
}

void Game::manageMenuEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
			nextState = Game::InGame;
		if (event.type == sf::Event::Closed)
			window.close();
	}
}
