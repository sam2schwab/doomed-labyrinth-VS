#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <CommunicationFPGA.h>
#include "maze.h"
#include "player.h"

class Game
{
public:
    Game();
    ~Game();
    int exec();
private:
	enum States{ Menu, InGame, Victory, Quitting} currentState, nextState;

    //members for ctor
    sf::Vector2i screenDimensions;
    sf::RenderWindow window;
    sf::Clock frameClock;

	//members for menu
	sf::Font menuFont;
	sf::Text menuMessage;

	//members for victory
	sf::Color backgroundColor;
	sf::Font victoryFont;
	sf::Text victoryMessage;

    //members for labyrinth
    sf::Texture wallTexture;
    sf::Texture pathTexture;
    Maze maze;
	//members for chest
	sf::Texture chestOpen;
	sf::Texture chestClosed;
	sf::Texture door;

    //members for player
    sf::Texture playerTexture;
    Player player;

    //members for audio
    sf::Music music;

	//members for FPGA communication
	CommunicationFPGA fpga;

    void initialize();
    void loadContent();
    void draw();
    void update(sf::Time deltaTime);

    void manageEvents();
	void manageFPGAEvents();
	void manageKeysEvents();
	void manageMenuEvents();
};

#endif // GAME_H
