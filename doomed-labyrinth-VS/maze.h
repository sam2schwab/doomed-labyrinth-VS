#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>

class Maze : public sf::Drawable
{
public:
    Maze(sf::RenderWindow &_window);
    ~Maze();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void initialize(int width, int height, sf::Texture _wall, sf::Texture _path, sf::Texture _openChest, sf::Texture _closedChest, sf::Texture _door);
    void update(sf::Time deltaTime);
    void move(int int_dir);
    bool* getMoving();
	bool getVictory();

    static const int UP = 0;
    static const int RIGHT = 1;
    static const int LEFT = 2;
    static const int DOWN = 3;

private:
	static const float SCALE;
	static const float SPEED;
    void init_array();
    void maze_generator(int index, int backtrack_x[], int backtrack_y[], int x, int y, int visited);
    int is_closed(int x, int y);

	bool isVictory;
    bool isMoving;
    sf::Vector2i dir;
    sf::Vector2i currentPos;
	sf::Vector2i doorPos;
    std::vector<std::vector<sf::Sprite*> > spriteArray;
    sf::RenderWindow * window;
    int width;
    int height;
    int CELL;
    sf::Texture pathTexture;
    sf::Texture wallTexture;

	//members for chests
	sf::Texture openChestTexture;
	sf::Texture closedChestTexture;
	sf::Texture doorTexture;
	sf::Sprite chest;
	sf::Vector2i chestPos;
	void initializeChest();
};

#endif // MAZE_H
