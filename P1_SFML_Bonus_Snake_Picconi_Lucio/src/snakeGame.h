#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

class SnakeGame
{

public:

	SnakeGame(int winLength, int width, int height);
	void play();

private:

	struct Vector2x2
	{
		sf::Vector2i actual;
		sf::Vector2i past;
	};

	void init();
	void loadSprites();

	void loop();
	void update();
	void draw();

	void updateMovement();
	void getInput();

	void snakeDirection();
	void updateSnakePos();

	void spawnFood();
	bool foodCantSpawn();
	bool isPlayerDead();
	bool didSnakeEatFood();

	bool didPlayerWin();

	int winSnakeLength;
	int screenWidth; 
	int screenHeight;
	int currentSnakeLength = 1;

	float cellSize;

	bool isGameOver = false;
	bool isThereFood = false;

	sf::Vector2i food;
	std::vector<Vector2x2> snakePos;

	char lastValidInput = ' ';

	sf::RenderWindow window;
	sf::RectangleShape snakeBlock;
	sf::RectangleShape foodBlock;

	sf::Texture loadBackGround;
	sf::Sprite backGround;

	sf::Texture loadAppleTexture;
	sf::Sprite appleSprite;

	sf::Texture loadbodyTexture;
	sf::Sprite bodySprite;

	sf::Texture loadheadTexture;
	sf::Sprite headSprite;

};