#include <stdlib.h>
#include <time.h>
#include "Game/Game.h"
#include "Tile/Tile.h"

#include "Shaders/header/shaders.h"
#include "img.h"

int main() {
	// --- create window obj
	sf::RenderWindow window;
	sf::RenderTexture r_texture;

	// --- randomize seed with time
	srand(static_cast<unsigned int>(time(NULL)));

	// --- load sprite
	sf::Texture texture;
	//texture.loadFromFile("minesweeper_sprites.png");

	sf::Image img;
	img.create(gimp_image.width, gimp_image.height, gimp_image.pixel_data);
	texture.loadFromImage(img);

	sf::Sprite sprite(texture);
	sprite.setScale(2, 2);

	float clock = 0;
	sf::Clock sf_clock;

	sf::Shader shader;
	// --- enable ONE for shaders
	//shader.loadFromMemory(bloom_shader_frag_string, sf::Shader::Fragment);
	//shader.loadFromMemory(chromatic_abberation_shader_frag_string, sf::Shader::Fragment);
	//shader.loadFromMemory(crt_shader_frag_string, sf::Shader::Fragment);
	//shader.loadFromMemory(empty_shader_frag_string, sf::Shader::Fragment);
	//shader.loadFromMemory(film_grain_shader_frag_string, sf::Shader::Fragment);
	//shader.loadFromMemory(vignetting_shader_frag_string, sf::Shader::Fragment);

	// --- game Dificulty
		// 0 - easy, 1 - intermediate, 2 - expert 
	int game_difficulty = 0;
	int game_size[3][2] = { { 9, 9 }, { 16, 16 }, { 30, 16 } };
	int game_bombs[3] = { 10, 40, 99 };

	// --- tileSize
	int tileSize = 32;

	// --- init game
	Game game(window, r_texture, game_size[game_difficulty][0], game_size[game_difficulty][1], game_bombs[game_difficulty], tileSize);

	while (window.isOpen()) {
		// --- event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			// -- window
			if (event.type == sf::Event::Closed)
				window.close();

			if (!window.hasFocus())
				continue;

			// --- game
			int i = static_cast<int>(floor(sf::Mouse::getPosition(window).x / tileSize));
			int j = static_cast<int>(floor((sf::Mouse::getPosition(window).y - game.gridOffset) / tileSize));
			sf::Vector2i mouse = sf::Mouse::getPosition(window);
			int xTop = game.size.col * game.tileSize / 2 - 24;
			int yLeft = (game.size.row + game.gridOffset) / 2 - 24;

			// --- reveal clicked Tile && reset peeking status
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && game.gameState == 0 && j >= 0) {
				if (game.grid[i][j].isInTile(sf::Mouse::getPosition(window))) {
					if (!game.grid[i][j].isMarked) {
						game.grid[i][j].reveal(game.grid);
					};
				};

				// --- prevent peek ghosting / glitching
				for (int i = 0; i < game_size[game_difficulty][0]; i++) {
					for (int j = 0; j < game_size[game_difficulty][1]; j++) {
						if (game.grid[i][j].isPeeking) {
							game.grid[i][j].isPeeking = false;
						};
					};
				};
			};

			// --- marking / flagging a tile
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && game.gameState == 0 && j >= 0) {
				if (game.grid[i][j].isInTile(sf::Mouse::getPosition(window))) {
					game.totalMarkCount = game.grid[i][j].mark() ? game.totalMarkCount + 1 : game.totalMarkCount - 1;
				};
			};

			// --- tile peeking
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && game.gameState == 0 && j >= 0) {
				// --- prevent peek ghosting / glitching
				for (int i = 0; i < game_size[game_difficulty][0]; i++) {
					for (int j = 0; j < game_size[game_difficulty][1]; j++) {
						if (game.grid[i][j].isPeeking) {
							game.grid[i][j].isPeeking = false;
						};
					};
				};

				if (game.grid[i][j].isInTile(sf::Mouse::getPosition(window))) {
					if (game.grid[i][j].isRevealed) {
						game.grid[i][j].loopNeighbor(game.grid, &Tile::peekNearBy);
					}
					else {
						game.grid[i][j].peek();
					};
				};
			};

			// --- menu
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				if (mouse.x > xTop && mouse.x < xTop + 24 * 2 && mouse.y > yLeft && mouse.y < yLeft + 24 * 2) {
					game.reset(window, r_texture, game_size[game_difficulty][0], game_size[game_difficulty][1], game_bombs[game_difficulty], tileSize);
				};
			};

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (mouse.x > xTop && mouse.x < xTop + 24*2 && mouse.y > yLeft && mouse.y < yLeft + 24*2) {
					game.faceState = 1;
				};
			};

			if (event.type == event.KeyReleased && (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Num3) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				game_difficulty = event.key.code - 27;
				game.reset(window, r_texture, game_size[game_difficulty][0], game_size[game_difficulty][1], game_bombs[game_difficulty], tileSize);
			};

		};

		int totalMarked = 0;
		int totalUnrevealed = 0;

		for (int i = 0; i < game_size[game_difficulty][0]; i++) {
			for (int j = 0; j < game_size[game_difficulty][1]; j++) {
				// --- update markcount / flag count for each tile
				game.grid[i][j].getMarkCount(game.grid);

				// --- check if game over
				if (game.grid[i][j].hasClickedOnBomb) {
					game.faceState = 3;
					game.gameOver();
				};
				if (!game.grid[i][j].isRevealed) {
					totalUnrevealed++;
				};
				if (game.grid[i][j].isBomb && game.grid[i][j].isMarked) {
					totalMarked++;
				};
			};
		};

		if (totalUnrevealed == game_bombs[game_difficulty] || (totalMarked == game_bombs[game_difficulty] && totalUnrevealed == game_bombs[game_difficulty])) {
			game.faceState = 2;
			game.gameOver();
		};


		
		sf::Sprite framebuffer(r_texture.getTexture());

		r_texture.clear(sf::Color(192, 192, 192));
		game.draw(r_texture, sprite);
		r_texture.display();

		// --- enable for shaders
		//clock = sf_clock.getElapsedTime().asSeconds();
		//shader.setUniform("u_time", clock);
		//shader.setUniform("texture", r_texture.getTexture());
		//shader.setUniform("u_resolution", sf::Vector2f{ (float)game_size[game_difficulty][0] * tileSize, (float)game_size[game_difficulty][1] * tileSize + game.gridOffset });

		window.clear();
		window.draw(framebuffer, &shader);
		window.display();
	};

    return 0;
};
