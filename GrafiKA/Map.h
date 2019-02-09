#include <SFML\Graphics.hpp>

using namespace sf;

const int height_map = 25;
const int width_map = 40;

String TileMap[height_map] =
{
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0    f                0     0          0",
	"0   000            0 00f    f0f0     0f0",
	"000000000s0s00000000s0000s00000000000000",
	"0000000000000000000000000000000000000000",
};

void randomMapGenerate() {

	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	int countStone = 2;

	while (countStone > 0) {
		randomElementX = 1 + rand() % (width_map - 1);
		randomElementY = 1 + rand() % (height_map - 1);

		if (TileMap[randomElementY][randomElementX] == ' ') {
			TileMap[randomElementY][randomElementX] = 's';
			countStone--;
		}
	}
}