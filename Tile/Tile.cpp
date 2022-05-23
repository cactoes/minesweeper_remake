#include "Tile.h"

Tile::Tile() {
	Tile::bombCount = 0;
	Tile::markCount = 0;
	Tile::tileSize = 32;
	Tile::isBomb = false;
	Tile::isRevealed = false;
	Tile::isMarked = false;
	Tile::isPeeking = false;
	Tile::hasClickedOnBomb = false;
	Tile::gridPosition = { 0, 0 };
	Tile::gridDrawPosition = { 0, 0 };
	Tile::gridSize = { 0, 0 };
};

Tile::Tile(Vector2i gridPos, Vector2i gridSize, int tileSize, int gridOffset) {
	Tile::bombCount = 0;
	Tile::markCount = 0;
	Tile::tileSize = tileSize;
	Tile::isBomb = false;
	Tile::isRevealed = false;
	Tile::isMarked = false;
	Tile::isPeeking = false;
	Tile::hasClickedOnBomb = false;
	Tile::gridPosition = gridPos;
	Tile::gridDrawPosition = { static_cast<float>(gridPos.col * tileSize), static_cast<float>(gridPos.row * tileSize + gridOffset)};
	Tile::gridSize = gridSize;
};

bool Tile::isInTile(sf::Vector2i mousePos) {
	return (mousePos.x > Tile::gridDrawPosition.col && mousePos.x < Tile::gridDrawPosition.col + Tile::tileSize &&
		    mousePos.y > Tile::gridDrawPosition.row && mousePos.y < Tile::gridDrawPosition.row + Tile::tileSize);
};

void Tile::reveal(Tile** grid) {
	Tile::isRevealed = true;
	Tile::isPeeking = false;

	if (Tile::isBomb) {
		Tile::hasClickedOnBomb = true;
	}

	if (Tile::bombCount == Tile::markCount) {
		Tile::loopNeighbor(grid, &Tile::revealPeaking);
	};

	if (Tile::bombCount == 0) {
		Tile::loopNeighbor(grid, &Tile::floodFill);
	};
};

bool Tile::mark() {
	if (!Tile::isRevealed)
		Tile::isMarked = !Tile::isMarked;
	return Tile::isMarked;
};

void Tile::peek() {
	Tile::isPeeking = true;
};

void Tile::loopNeighbor(Tile** grid, void(Tile::* function)(Tile** grid, int x, int y)) {
	for (int i = -1; i <= 1; i++) {
		int xoffsett = Tile::gridPosition.col + i;
		if (xoffsett < 0 || xoffsett >= Tile::gridSize.col)
			continue;

		for (int j = -1; j <= 1; j++) {
			int yoffsett = static_cast<int>(Tile::gridPosition.row + j);
			if (yoffsett < 0 || yoffsett >= Tile::gridSize.row)
				continue;

			(this->*function)(grid, xoffsett, yoffsett);
		};
	};
};

void Tile::floodFill(Tile** grid, int x, int y) {
	Tile neighbor = grid[x][y];
	if (!neighbor.isRevealed) {
		grid[x][y].reveal(grid);
	};
};

void Tile::revealPeaking(Tile** grid, int x, int y) {
	Tile neighbor = grid[x][y];
	if (neighbor.isPeeking) {
		grid[x][y].reveal(grid);
	};
};

void Tile::peekNearBy(Tile** grid, int x, int y) {
	Tile neighbor = grid[x][y];
	if (!neighbor.isRevealed && !neighbor.isMarked) {
		grid[x][y].peek();
	};
};

void Tile::getBombCount(Tile** grid) {
	if (Tile::isBomb) {
		Tile::bombCount = -1;
		return;
	};

	int total = 0;
	for (int i = -1; i <= 1; i++) {
		int xoffsett = static_cast<int>(Tile::gridPosition.col + i);
		if (xoffsett < 0 || xoffsett >= Tile::gridSize.col)
			continue;

		for (int j = -1; j <= 1; j++) {
			int yoffsett = static_cast<int>(Tile::gridPosition.row + j);
			if (yoffsett < 0 || yoffsett >= Tile::gridSize.row)
				continue;

			Tile neighbor = grid[xoffsett][yoffsett];

			if (neighbor.isBomb) {
				total++;
			};
		};
	};

	Tile::bombCount = total;
};

void Tile::getMarkCount(Tile** grid) {
	if (Tile::isBomb) {
		Tile::markCount = -1;
		return;
	};

	int total = 0;
	for (int i = -1; i <= 1; i++) {
		int xoffsett = static_cast<int>(Tile::gridPosition.col + i);
		if (xoffsett < 0 || xoffsett >= Tile::gridSize.col)
			continue;

		for (int j = -1; j <= 1; j++) {
			int yoffsett = static_cast<int>(Tile::gridPosition.row + j);
			if (yoffsett < 0 || yoffsett >= Tile::gridSize.row)
				continue;

			Tile neighbor = grid[xoffsett][yoffsett];
			if (neighbor.isMarked)
				total++;
		};
	};

	Tile::markCount = total;
};

void Tile::draw(sf::RenderTexture& r_texture, sf::Sprite sprite, int gameState) {
	sprite.setPosition({ Tile::gridDrawPosition.col, Tile::gridDrawPosition.row });

	if (Tile::isRevealed) {
		if (Tile::isBomb) {
			if (Tile::hasClickedOnBomb) {
				// isRevealed -> isBomb -> hasClickedOnBomb
				sprite.setTextureRect(sf::IntRect(64, 47, 16, 16));
			}
			else if (Tile::isMarked) {
				if (gameState == 1 && !Tile::isBomb) {
					// isRevealed -> isMarked -> gameIsFinished && !isBomb
					sprite.setTextureRect(sf::IntRect(80, 47, 16, 16));
				}
				else {
					// isRevealed -> isMarked -> isBomb
					sprite.setTextureRect(sf::IntRect(32, 47, 16, 16));
				};
			}
			else {
				// isRevealed -> isBomb
				sprite.setTextureRect(sf::IntRect(48, 47, 16, 16));
			};
		}
		else {
			if (Tile::bombCount > 0) {
				// isRevealed -> !isBomb -> bombCount
				sprite.setTextureRect(sf::IntRect((Tile::bombCount - 1) * 16, 63, 16, 16));
			}
			else {
				// isRevealed -> !isBomb -> emptyTile
				sprite.setTextureRect(sf::IntRect(16, 47, 16, 16));
			};
		};
	}
	else if (Tile::isMarked) {
		if (gameState == 1 && !Tile::isBomb) {
			// isMarked -> gameIsFinished && !isBomb
			sprite.setTextureRect(sf::IntRect(80, 47, 16, 16));
		}
		else {
			// isMarked
			sprite.setTextureRect(sf::IntRect(32, 47, 16, 16));
		};
	}
	else if (Tile::isPeeking) {
		// isPeeking
		sprite.setTextureRect(sf::IntRect(16, 47, 16, 16));
	}
	else {
		// default
		sprite.setTextureRect(sf::IntRect(0, 47, 16, 16));
	};

	r_texture.draw(sprite);
};