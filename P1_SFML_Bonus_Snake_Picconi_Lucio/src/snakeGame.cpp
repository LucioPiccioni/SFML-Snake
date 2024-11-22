#include "SnakeGame.h"

#include <string>

SnakeGame::SnakeGame(int winLength, int width, int height)
	: cellSize(static_cast<float>(width)), winSnakeLength(winLength), screenWidth(width), screenHeight(height),
	window(sf::VideoMode(width * static_cast<int>(cellSize), height* static_cast<int>(cellSize)), "Snake Game")
{
}

void SnakeGame::play()
{
	std::srand(static_cast<int>(std::time(nullptr)));

	init();

	loadAssets();

	loop();
}

void SnakeGame::init()
{
	isGameOver = false;

	lastValidInput = ' ';

	if (!snakePos.empty())
		snakePos.clear();

	Vector2x2 initialSegment = { {screenWidth / 2, screenHeight / 2}, {screenWidth / 2, screenHeight / 2} };
	snakePos.push_back(initialSegment);

	currentSnakeLength = 1;

	std::string text = "Length: " + std::to_string(currentSnakeLength) + " / " + std::to_string(winSnakeLength);

	textScore.setString(text);
}

void SnakeGame::loadAssets()
{
	loadBackGround.loadFromFile("res/Grass2.png");
	backGround.setTexture(loadBackGround);

	float scaleX = float(screenWidth) * cellSize / loadBackGround.getSize().x;
	float scaleY = float(screenHeight) * cellSize / loadBackGround.getSize().y;

	backGround.setScale(scaleX, scaleY);

	loadAppleTexture.loadFromFile("res/apple.png");
	appleSprite.setTexture(loadAppleTexture);
	float scaleAppleX = cellSize / loadAppleTexture.getSize().x;
	float scaleAppleY = cellSize / loadAppleTexture.getSize().y;
	appleSprite.setScale(scaleAppleX, scaleAppleY);
	appleSprite.setOrigin(appleSprite.getLocalBounds().width / 2, appleSprite.getLocalBounds().height / 2);

	loadbodyTexture.loadFromFile("res/body.png");
	bodySprite.setTexture(loadbodyTexture);
	float scaleBodyX = cellSize / loadbodyTexture.getSize().x;
	float scaleBodyY = cellSize / loadbodyTexture.getSize().y;
	bodySprite.setScale(scaleBodyX, scaleBodyY);
	bodySprite.setOrigin(bodySprite.getLocalBounds().width / 2, bodySprite.getLocalBounds().height / 2);

	loadheadTexture.loadFromFile("res/head.png");
	headSprite.setTexture(loadheadTexture);
	float scaleHeadX = cellSize / loadheadTexture.getSize().x;
	float scaleHeadY = cellSize / loadheadTexture.getSize().y;
	headSprite.setScale(scaleHeadX, scaleHeadY);
	headSprite.setOrigin(headSprite.getLocalBounds().width / 2, headSprite.getLocalBounds().height / 2);

	font.loadFromFile("res/Wake Snake Free Trial.ttf");

	textScore.setFont(font);

	textScore.setCharacterSize(24);
	textScore.setFillColor(sf::Color::Black);
	textScore.setStyle(sf::Text::Bold);
}

void SnakeGame::loop()
{
	sf::Clock clock;
	const float frameTime = 0.15f;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		getInput();

		if (clock.getElapsedTime().asSeconds() > frameTime)
		{
			clock.restart();
			update();
			draw();
			updateSnakePos();
		}

		if (isGameOver)
			init();
	}
}

void SnakeGame::update()
{
	updateMovement();

	if (didSnakeEatFood())
	{
		currentSnakeLength++;

		std::string text = "Length: " + std::to_string(currentSnakeLength) + " / " + std::to_string(winSnakeLength);

		textScore.setString(text);

		isGameOver = didPlayerWin();

		Vector2x2 newSnakePart;
		newSnakePart.actual.x = snakePos[snakePos.size() - 1].past.x;
		newSnakePart.actual.y = snakePos[snakePos.size() - 1].past.y;
		newSnakePart.past.x = newSnakePart.actual.x;
		newSnakePart.past.y = newSnakePart.actual.y;
		snakePos.push_back(newSnakePart);

		isThereFood = false;
	}

	if (!isThereFood)
	{
		spawnFood();
	}

	if (isPlayerDead() || currentSnakeLength == winSnakeLength)
	{
		isGameOver = true;
	}
}

void SnakeGame::draw()
{
	window.clear();
	window.draw(backGround);


	appleSprite.setPosition(food.x * cellSize + 10, food.y * cellSize + 10);
	window.draw(appleSprite);

	headSprite.setPosition(snakePos.begin()->actual.x * cellSize + 10, snakePos.begin()->actual.y * cellSize + 10);
	window.draw(headSprite);

	for (std::vector<Vector2x2>::iterator segment = snakePos.begin() + 1; segment != snakePos.end(); segment++)
	{
		bodySprite.setPosition(segment->actual.x * cellSize + 10, segment->actual.y * cellSize + 10);
		window.draw(bodySprite);
	}

	window.draw(textScore);

	window.display();
}

void SnakeGame::getInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && lastValidInput != 'S'
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		lastValidInput = 'W';
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && lastValidInput != 'W'
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		lastValidInput = 'S';
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && lastValidInput != 'D'
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		lastValidInput = 'A';
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && lastValidInput != 'A'
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		lastValidInput = 'D';
	}
}

void SnakeGame::snakeDirection()
{
	switch (lastValidInput)
	{
	case 'W':
		snakePos[0].actual.y--;
		headSprite.setRotation(180);
		break;

	case 'A':
		snakePos[0].actual.x--;
		headSprite.setRotation(90);
		break;

	case 'S':
		snakePos[0].actual.y++;
		headSprite.setRotation(0);
		break;

	case 'D':
		snakePos[0].actual.x++;
		headSprite.setRotation(-90);
		break;
	}
}

void SnakeGame::updateMovement()
{
	snakeDirection();
}

void SnakeGame::updateSnakePos()
{
	for (int i = 0; i < currentSnakeLength; i++)
	{
		snakePos[i].past.x = snakePos[i].actual.x;
		snakePos[i].past.y = snakePos[i].actual.y;
	}

	for (int i = currentSnakeLength - 1; i > 0; i--)
	{
		snakePos[i].actual.x = snakePos[i - 1].actual.x;
		snakePos[i].actual.y = snakePos[i - 1].actual.y;
	}
}

void SnakeGame::spawnFood()
{
	do
	{
		food.x = std::rand() % screenWidth;
		food.y = std::rand() % screenHeight;

	} while (foodCantSpawn());

	isThereFood = true;
}

bool SnakeGame::foodCantSpawn()
{
	for (std::vector<Vector2x2>::iterator segment = snakePos.begin(); segment != snakePos.end(); segment++)
	{
		if (segment->actual.x == food.x && segment->actual.y == food.y)
			return true;
	}

	return false;
}

bool SnakeGame::isPlayerDead()
{
	if (snakePos[0].actual.x < 0 || snakePos[0].actual.y < 0 ||
		snakePos[0].actual.x >= screenWidth || snakePos[0].actual.y >= screenHeight)
	{
		return true;
	}

	for (int i = 1; i < currentSnakeLength; i++)
	{
		if (snakePos[0].actual == snakePos[i].actual)
		{
			return true;
		}
	}

	return false;
}

bool SnakeGame::didSnakeEatFood()
{
	return snakePos[0].actual == food;
}

bool SnakeGame::didPlayerWin()
{
	return currentSnakeLength == winSnakeLength;
}
