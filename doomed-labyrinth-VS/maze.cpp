#include "maze.h"
#include <iostream>
const float Maze::SCALE = 0.17;
const float Maze::SPEED = 15;

Maze::Maze(sf::RenderWindow &_window)
	: window(&_window),
	isMoving(false),
	isVictory(false)
{

}

Maze::~Maze()
{
	for each (auto tab in spriteArray)
	{
		for each (auto sprite in tab)
		{
			delete sprite;
		}
	}
}

bool Maze::getVictory()
{
	return isVictory;
}

bool *Maze::getMoving()
{
    return &isMoving;
}

void Maze::move(int int_dir)
{
    if(!isMoving)
    {
        switch(int_dir)
        {
        case UP:
            dir = sf::Vector2i(0,-1);
			std::cout << "trying to move up" << std::endl;
            break;
        case DOWN:
            dir = sf::Vector2i(0,1);
			std::cout << "trying to move down" << std::endl;
            break;
        case LEFT:
            dir = sf::Vector2i(-1,0);
			std::cout << "trying to move left" << std::endl;
            break;
        case RIGHT:
            dir = sf::Vector2i(1,0);
			std::cout << "trying to move right" << std::endl;
            break;
        }
        sf::Vector2i temp = currentPos + dir;
        sf::Vector2i test90;
        sf::Vector2i test270;
		while (!(temp.x < 0 || temp.y < 0 || temp.x >= width * 2 + 1 || temp.y >= height * 2 + 1 ||
			spriteArray[temp.x][temp.y]->getTexture() != &pathTexture))
        {
			std::cout << "can move in that direction!" << std::endl;
            isMoving = true;
            currentPos = temp;
            temp = currentPos + dir;
            test90 = currentPos + sf::Vector2i(dir.y,dir.x);
            test270 = currentPos + sf::Vector2i(-dir.y,-dir.x);
            if(spriteArray[test90.x][test90.y]->getTexture() != &wallTexture ||
               spriteArray[test270.x][test270.y]->getTexture() != &wallTexture ||
			   (currentPos == chestPos && chest.getTexture() == &closedChestTexture))
                break;
        }
    }
    else
		std::cout << "cannot move, already moving" << std::endl;
}

void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for each (auto tab in spriteArray)
    {
		for each (auto sprite in tab)
        {
            target.draw(*sprite, states);
        }
    }
	target.draw(chest);
}

void Maze::initialize(int _width,int _height, sf::Texture _wall, sf::Texture _path, sf::Texture _openChest, sf::Texture _closedChest, sf::Texture _door)
{
    width = _width;
    height = _height;
	doorPos = sf::Vector2i(rand()%(width)*2+1,height * 2);
    wallTexture = _wall;
    pathTexture = _path;
	openChestTexture = _openChest;
	closedChestTexture = _closedChest;
	doorTexture = _door;
    CELL = width*height;

    srand((unsigned)time(NULL));
    int index = 0;
    int* backtrack_x = new int[CELL];
    int* backtrack_y = new int[CELL];
    init_array();
    backtrack_x[index] = 1;
    backtrack_y[index] = 1;
    maze_generator(index, backtrack_x, backtrack_y, 1, 1, 1);

	initializeChest();

    currentPos = sf::Vector2i(1,0);

	delete[] backtrack_x;
	delete[] backtrack_y;
}

void Maze::initializeChest()
{
	do {
		chestPos.x = rand() % (width * 2);
		chestPos.y = rand() % (height * 2);
	} while (!(spriteArray[chestPos.x][chestPos.y]->getTexture() == &pathTexture && 
		(int(spriteArray[chestPos.x + 1][chestPos.y]->getTexture() == &wallTexture) +
		 int(spriteArray[chestPos.x - 1][chestPos.y]->getTexture() == &wallTexture) +
		 int(spriteArray[chestPos.x][chestPos.y + 1]->getTexture() == &wallTexture) +
		 int(spriteArray[chestPos.x][chestPos.y - 1]->getTexture() == &wallTexture)) == 3));
	chest.setTexture(closedChestTexture);
	chest.setScale(SCALE, SCALE);
	chest.setPosition(spriteArray[chestPos.x][chestPos.y]->getPosition().x + pathTexture.getSize().x / 2 * SCALE - closedChestTexture.getSize().x / 2 * SCALE,
		spriteArray[chestPos.x][chestPos.y]->getPosition().y + pathTexture.getSize().y / 2 * SCALE - closedChestTexture.getSize().y / 2 * SCALE);
}

void Maze::init_array()
{
    int x_offset = window->getSize().x/2 - wallTexture.getSize().x*SCALE*1.5;
    int y_offset = window->getSize().y/2 - wallTexture.getSize().y*SCALE*0.5;

    spriteArray = std::vector<std::vector<sf::Sprite*> >(width*2+1);

    for(int colonne = 0; colonne < width*2+1; colonne++)
    {
        spriteArray[colonne] = std::vector<sf::Sprite*>(height*2+1);
        for(int ligne = 0; ligne < height*2+1; ligne++)
        {
            spriteArray[colonne][ligne] = new sf::Sprite();
            spriteArray[colonne][ligne]->setScale(SCALE,SCALE);
            spriteArray[colonne][ligne]->setPosition(x_offset+(wallTexture.getSize().x*SCALE*colonne),y_offset+(wallTexture.getSize().y*SCALE*ligne));
            if(colonne % 2 == 0 || ligne % 2 == 0)
                spriteArray[colonne][ligne]->setTexture(wallTexture);
            else
                spriteArray[colonne][ligne]->setTexture(pathTexture);
        }
    }
}

void Maze::maze_generator(int index, int backtrack_x[], int backtrack_y[], int x, int y, int visited)
{
    if(visited < width * height)
    {
        int neighbour_valid = -1;
        int neighbour_x[4];
        int neighbour_y[4];
        int step[4];

        int x_next;
        int y_next;

        if(x - 2 > 0 && is_closed(x - 2, y))  // upside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x - 2;;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=1;
        }

        if(y - 2 > 0 && is_closed(x, y - 2))  // leftside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y - 2;
            step[neighbour_valid]=2;
        }

        if(y + 2 < height * 2 + 1 && is_closed(x, y + 2))  // rightside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y + 2;
            step[neighbour_valid]=3;

        }

        if(x + 2 < width * 2 + 1 && is_closed(x + 2, y))  // downside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x+2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=4;
        }

        if(neighbour_valid == -1)
        {
            // backtrack
            x_next = backtrack_x[index];
            y_next = backtrack_y[index];
            index--;
        }

        if(neighbour_valid!=-1)
        {
            int randomization = neighbour_valid + 1;
            int random = rand()%randomization;
            x_next = neighbour_x[random];
            y_next = neighbour_y[random];
            index++;
            backtrack_x[index] = x_next;
            backtrack_y[index] = y_next;

            int rstep = step[random];

            if(rstep == 1)
                spriteArray[x_next+1][y_next]->setTexture(pathTexture);
            else if(rstep == 2)
                spriteArray[x_next][y_next + 1]->setTexture(pathTexture);
            else if(rstep == 3)
                spriteArray[x_next][y_next - 1]->setTexture(pathTexture);
            else if(rstep == 4)
                spriteArray[x_next - 1][y_next]->setTexture(pathTexture);
            visited++;
        }

        maze_generator(index, backtrack_x, backtrack_y, x_next, y_next, visited);
    }
    //generer entree et sortie -- temporairement hardcode
    spriteArray[1][0]->setTexture(pathTexture);
    spriteArray[doorPos.x][doorPos.y]->setTexture(doorTexture);
}

int Maze::is_closed(int x, int y)
{
    if(spriteArray[x - 1][y]->getTexture() != &pathTexture
       && spriteArray[x][y - 1]->getTexture() != &pathTexture
       && spriteArray[x][y + 1]->getTexture() != &pathTexture
       && spriteArray[x + 1][y]->getTexture() != &pathTexture
    )
        return 1;

    return 0;
}

void Maze::update(sf::Time deltaTime)
{
    if(isMoving)
    {
        if(abs((spriteArray[currentPos.x][currentPos.y]->getPosition().x + wallTexture.getSize().x*SCALE*0.5) - window->getSize().x/2) > SPEED*SCALE ||
           abs((spriteArray[currentPos.x][currentPos.y]->getPosition().y + wallTexture.getSize().y*SCALE*0.5) - window->getSize().y/2) > SPEED*SCALE)
        {
            for(int colonne = 0; colonne < width*2+1; colonne++)
            {
                for(int ligne = 0; ligne < height*2+1; ligne++)
                {
                    spriteArray[colonne][ligne]->setPosition(spriteArray[colonne][ligne]->getPosition().x - SPEED*SCALE*dir.x,
                                                             spriteArray[colonne][ligne]->getPosition().y - SPEED*SCALE*dir.y);
                }
            }
			chest.setPosition(chest.getPosition().x - SPEED*SCALE*dir.x,
				chest.getPosition().y - SPEED*SCALE*dir.y);
        }
        else if (int((spriteArray[currentPos.x][currentPos.y]->getPosition().x + wallTexture.getSize().x*SCALE*0.5) - window->getSize().x/2) == 0 &&
                 int((spriteArray[currentPos.x][currentPos.y]->getPosition().y + wallTexture.getSize().y*SCALE*0.5) - window->getSize().y/2) == 0)
        {
			std::cout << "stopped moving" << std::endl;
            isMoving = false;
			if (currentPos.y + 1 >= height*2+1)
				isVictory = true;
			if (currentPos == chestPos && chest.getTexture() == &closedChestTexture)
				chest.setTexture(openChestTexture);
			if (currentPos + sf::Vector2i(0, 1) == doorPos && chest.getTexture() == &openChestTexture)
				spriteArray[doorPos.x][doorPos.y]->setTexture(pathTexture);

        }
        else
        {
            int x_offset = (spriteArray[currentPos.x][currentPos.y]->getPosition().x + wallTexture.getSize().x*SCALE*0.5) - window->getSize().x/2;
            int y_offset = (spriteArray[currentPos.x][currentPos.y]->getPosition().y + wallTexture.getSize().y*SCALE*0.5) - window->getSize().y/2;
            for(int colonne = 0; colonne < width*2+1; colonne++)
            {
                for(int ligne = 0; ligne < height*2+1; ligne++)
                {
                    spriteArray[colonne][ligne]->setPosition(spriteArray[colonne][ligne]->getPosition().x - x_offset,
                                                             spriteArray[colonne][ligne]->getPosition().y - y_offset);
                }
            }
			chest.setPosition(chest.getPosition().x - x_offset,
				chest.getPosition().y - y_offset);
        }
    }
}
