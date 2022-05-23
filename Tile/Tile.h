#pragma once
#include <SFML/Graphics.hpp>
struct Vector2f {
	float col, row;
};

struct Vector2i {
	int col, row;
};

class Tile {
public:
	int tileSize;
	int bombCount;
	int markCount;
	bool isBomb;
	bool isMarked;
	bool isPeeking;
	bool isRevealed;
	bool hasClickedOnBomb;
	Vector2i gridSize;
	Vector2i gridPosition;
	Vector2f gridDrawPosition;

public:
	Tile();
	Tile(Vector2i gridPos, Vector2i gridSize, int tileSize, int gridOffset);

	bool mark();
	bool isInTile(sf::Vector2i mousePos);

	void peek();
	void reveal(Tile** grid);
	void getBombCount(Tile** grid);
	void getMarkCount(Tile** grid);
	void floodFill(Tile** grid, int x, int y);
	void peekNearBy(Tile** grid, int x, int y);
	void revealPeaking(Tile** grid, int x, int y);
	void draw(sf::RenderTexture& r_texture, sf::Sprite sprite, int gameState);
	void loopNeighbor(Tile** grid, void(Tile::* function)(Tile** grid, int x, int y));
};
