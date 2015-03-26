#include "player.h"

const float Player::SCALE = 0.7;

Player::Player(sf::RenderWindow &_window,sf::Time frameTime)
    : AnimatedSprite(frameTime,true,true),
      window(&_window)
{

}

Player::~Player()
{

}

void Player::changeCharacter(int index)
{
	playerIndex = index;
	move(direction);
}

void Player::move(int int_dir)
{
	direction = int_dir;
    if(!*isMoving)
    {
        switch(int_dir)
        {
        case UP:
            setAnimation(walkingAnimationUp[playerIndex]);
            break;
        case DOWN:
			setAnimation(walkingAnimationDown[playerIndex]);
            break;
        case LEFT:
			setAnimation(walkingAnimationLeft[playerIndex]);
            break;
        case RIGHT:
			setAnimation(walkingAnimationRight[playerIndex]);
            break;
        }
    }
}

void Player::update(sf::Time deltaTime)
{
    *isMoving?play():stop();
    AnimatedSprite::update(deltaTime);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    AnimatedSprite::draw(target,states);
}

void Player::initialize(sf::Texture _spriteSheet, bool* moving, int _playerIndex)
{
	direction = DOWN;
	playerIndex = _playerIndex;
    isMoving = moving;
    spriteSheet = _spriteSheet;

	for (int i = 0; i < NB_CHARACTERS; i++)
	{
		int x = i % 3;
		int y = i < 3 ? 0 : 1;
		walkingAnimationDown[i].setSpriteSheet(spriteSheet);
		walkingAnimationDown[i].addFrame(sf::IntRect(0 + x * 128, 0 + y * 192, 32, 48)); //left, top, width, height
		walkingAnimationDown[i].addFrame(sf::IntRect(32 + x * 128, 0 + y * 192, 32, 48));
		walkingAnimationDown[i].addFrame(sf::IntRect(64 + x * 128, 0 + y * 192, 32, 48));
		walkingAnimationDown[i].addFrame(sf::IntRect(96 + x * 128, 0 + y * 192, 32, 48));

		walkingAnimationLeft[i].setSpriteSheet(spriteSheet);
		walkingAnimationLeft[i].addFrame(sf::IntRect(0 + x * 128, 48 + y * 192, 32, 48));
		walkingAnimationLeft[i].addFrame(sf::IntRect(32 + x * 128, 48 + y * 192, 32, 48));
		walkingAnimationLeft[i].addFrame(sf::IntRect(64 + x * 128, 48 + y * 192, 32, 48));
		walkingAnimationLeft[i].addFrame(sf::IntRect(96 + x * 128, 48 + y * 192, 32, 48));

		walkingAnimationRight[i].setSpriteSheet(spriteSheet);
		walkingAnimationRight[i].addFrame(sf::IntRect(0 + x * 128, 96 + y * 192, 32, 48));
		walkingAnimationRight[i].addFrame(sf::IntRect(32 + x * 128, 96 + y * 192, 32, 48));
		walkingAnimationRight[i].addFrame(sf::IntRect(64 + x * 128, 96 + y * 192, 32, 48));
		walkingAnimationRight[i].addFrame(sf::IntRect(96 + x * 128, 96 + y * 192, 32, 48));

		walkingAnimationUp[i].setSpriteSheet(spriteSheet);
		walkingAnimationUp[i].addFrame(sf::IntRect(0 + x * 128, 144 + y * 192, 32, 48));
		walkingAnimationUp[i].addFrame(sf::IntRect(32 + x * 128, 144 + y * 192, 32, 48));
		walkingAnimationUp[i].addFrame(sf::IntRect(64 + x * 128, 144 + y * 192, 32, 48));
		walkingAnimationUp[i].addFrame(sf::IntRect(96 + x * 128, 144 + y * 192, 32, 48));
	}

	setAnimation(walkingAnimationDown[playerIndex]);

    setPosition(window->getSize().x/2 - 16*SCALE, window->getSize().y/2 - 24*SCALE);
    setScale(SCALE,SCALE);
}
