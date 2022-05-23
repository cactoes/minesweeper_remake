#pragma once
#include <SFML/Graphics.hpp>
#include "../Tile/Tile.h"

class Game {
public:
	int gameState;
	int bombCount;
	int gridOffset;
	int tileSize;
	int faceState;
	int totalMarkCount;
	Vector2i size;
	Tile** grid;

public:
	Game(sf::RenderWindow& window, sf::RenderTexture& r_texture, int collums, int rows, int bombCount, int tileSize);
	~Game();

	void gameOver();
	void draw(sf::RenderTexture& r_texture, sf::Sprite sprite);
	void reset(sf::RenderWindow& window, sf::RenderTexture& r_texture, int collums, int rows, int bombCount, int tileSize);
};