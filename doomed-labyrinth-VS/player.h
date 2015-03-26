#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "Animation.h"

class Player : public AnimatedSprite
{
public:
    Player(sf::RenderWindow &_window, sf::Time frameTime = sf::seconds(0.2f));
    ~Player();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void initialize(sf::Texture _spriteSheet, bool *moving, int _playerIndex = 0);
    void update(sf::Time deltaTime);
    void move(int int_dir);
	void changeCharacter(int index);

    static const int UP = 0;
    static const int RIGHT = 1;
    static const int LEFT = 2;
    static const int DOWN = 3;
private:
    static const float SCALE;
	static const int NB_CHARACTERS = 6;
    sf::RenderWindow * window;
    const bool * isMoving;
	int playerIndex;
	int direction;
	sf::Texture spriteSheet;
	Animation walkingAnimationDown[NB_CHARACTERS];
	Animation walkingAnimationLeft[NB_CHARACTERS];
	Animation walkingAnimationRight[NB_CHARACTERS];
	Animation walkingAnimationUp[NB_CHARACTERS];
};

#endif // PLAYER_H
