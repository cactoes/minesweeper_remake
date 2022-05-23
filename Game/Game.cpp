#include <vector>
#include "Game.h"

Game::Game(sf::RenderWindow& window, sf::RenderTexture& r_texture, int collums, int rows, int bombCount, int tileSize) {
	this->gridOffset = 100;
	this->tileSize = tileSize;
	this->faceState = 0;
	this->totalMarkCount = 0;

	// --- setup window
	window.create(sf::VideoMode(static_cast<unsigned int>(collums * tileSize), static_cast<unsigned int>(rows * tileSize + this->gridOffset)), "minesweeper", sf::Style::Close);
	r_texture.create(static_cast<unsigned int>(collums * tileSize), static_cast<unsigned int>(rows * tileSize + this->gridOffset));

	window.setFramerateLimit(30);

	// --- setup grid
	this->grid = new Tile * [static_cast<size_t>(collums)];

	for (int i = 0; i < collums; i++) {
		this->grid[i] = new Tile[static_cast<size_t>(rows)];
	};

	for (int i = 0; i < collums; i++) {
		for (int j = 0; j < rows; j++) {
			this->grid[i][j] = Tile({ i, j }, { collums, rows }, tileSize, this->gridOffset);
		};
	};

	this->size = { collums, rows };

	// --- setup bombs
	this->bombCount = bombCount;

	std::vector<std::vector<int>> allBombSpots;

	for (int i = 0; i < collums; i++) {
		for (int j = 0; j < rows; j++) {
			allBombSpots.push_back({ i, j });
		};
	};

	for (int n = 0; n < bombCount; n++) {
		int index = static_cast<int>(floor(rand() % allBombSpots.size()));
		int i = allBombSpots[index][0];
		int j = allBombSpots[index][1];
		allBombSpots.erase(allBombSpots.begin() + index);
		this->grid[i][j].isBomb = true;
	};


	// --- initialize each tile
	for (int i = 0; i < collums; i++) {
		for (int j = 0; j < rows; j++) {
			this->grid[i][j].getBombCount(this->grid);
		};
	};

	// --- setup game stage
		// 0 - playing, 1 - paused
	this->gameState = 0;
};

Game::~Game() {
	for (int i = 0; i < this->size.col; i++) {
		delete[] this->grid[i];
	}
	delete[] this->grid;
};

void Game::reset(sf::RenderWindow& window, sf::RenderTexture& r_texture, int collums, int rows, int bombCount, int tileSize) {
	this->~Game();
	this->faceState = 0;
	this->gameState = 1;

	// --- setup window
	if (this->size.col != collums || this->size.row != rows) {
		window.create(sf::VideoMode(static_cast<unsigned int>(collums * tileSize), static_cast<unsigned int>(rows * tileSize + this->gridOffset)), "minesweeper", sf::Style::Close);
		r_texture.create(static_cast<unsigned int>(collums * tileSize), static_cast<unsigned int>(rows * tileSize + this->gridOffset));
		window.setFramerateLimit(30);
	};

	// --- setup grid
	this->grid = new Tile * [static_cast<size_t>(collums)];

	for (int i = 0; i < collums; i++) {
		this->grid[i] = new Tile[static_cast<size_t>(rows)];
	};

	for (int i = 0; i < collums; i++) {
		for (int j = 0; j < rows; j++) {
			this->grid[i][j] = Tile({ i, j }, { collums, rows }, tileSize, this->gridOffset);
		};
	};

	this->size = { collums, rows };

	// --- setup bombs
	this->bombCount = bombCount;

	std::vector<std::vector<int>> allBombSpots;

	for (int i = 0; i < collums; i++) {
		for (int j = 0; j < rows; j++) {
			allBombSpots.push_back({ i, j });
		};
	};

	for (int n = 0; n < bombCount; n++) {
		int index = static_cast<int>(floor(rand() % allBombSpots.size()));
		int i = allBombSpots[index][0];
		int j = allBombSpots[index][1];
		allBombSpots.erase(allBombSpots.begin() + index);
		this->grid[i][j].isBomb = true;
	};


	// --- initialize each tile
	for (int i = 0; i < collums; i++) {
		for (int j = 0; j < rows; j++) {
			this->grid[i][j].getBombCount(this->grid);
		};
	};

	// --- setup game stage
		// 0 - playing, 1 - paused
	this->gameState = 0;
};

void Game::draw(sf::RenderTexture& r_texture, sf::Sprite sprite) {
	// --- draw face button
	sprite.setPosition({ static_cast<float>(this->size.col * this->tileSize / 2 - 24), static_cast<float>((this->size.row + this->gridOffset) / 2 - 24)});
	sprite.setTextureRect(sf::IntRect(this->faceState * 24, 23, 24, 24));
	r_texture.draw(sprite);

	for (int i = 0; i < this->size.col; i++) {
		for (int j = 0; j < this->size.row; j++) {
			this->grid[i][j].draw(r_texture, sprite, this->gameState);
		};
	};
};

void Game::gameOver() {
	this->gameState = 1;
	for (int i = 0; i < this->size.col; i++) {
		for (int j = 0; j < this->size.row; j++) {
			if (this->grid[i][j].isBomb) {
				this->grid[i][j].isRevealed = true;
			};
		};
	};
};