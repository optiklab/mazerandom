#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <ctime>

using namespace sf;
using namespace std;

const int VERTICAL_CELLS = 20;
const int HORIZONTAL_CELLS = 20;
const int SQUARE_TILE_SIZE = 26;
const int SQUARE_WALL_SIZE = 2;
const int SQUARE_CELL_SIZE = 28;

const int CELL_PATH_N = 0x01;
const int CELL_PATH_E = 0x02;
const int CELL_PATH_S = 0x04;
const int CELL_PATH_W = 0x08;
const int CELL_VISITED = 0x10;

void drawMaze(RenderWindow& window, vector<vector<int>> &maze_cells)
{
	Color rgba = Color::Black;
	rgba.r = 235;
	rgba.g = 235;
	rgba.b = 235;

	sf::RectangleShape verticalSpace(sf::Vector2f(SQUARE_WALL_SIZE, SQUARE_TILE_SIZE + 2));
	verticalSpace.setFillColor(rgba);

	sf::RectangleShape horizontalSpace(sf::Vector2f(SQUARE_TILE_SIZE + 2, SQUARE_WALL_SIZE));
	horizontalSpace.setFillColor(rgba);

	sf::RectangleShape verticalWall(sf::Vector2f(SQUARE_WALL_SIZE, SQUARE_TILE_SIZE));
	verticalWall.setFillColor(Color::Black);

	sf::RectangleShape horizontalWall(sf::Vector2f(SQUARE_TILE_SIZE, SQUARE_WALL_SIZE));
	horizontalWall.setFillColor(Color::Black);

	sf::RectangleShape tile;// (sf::Vector2f(SQUARE_TILE_SIZE, SQUARE_TILE_SIZE));
	tile.setSize(sf::Vector2f(SQUARE_TILE_SIZE, SQUARE_TILE_SIZE));
	tile.setPosition(sf::Vector2f(SQUARE_WALL_SIZE, SQUARE_WALL_SIZE));
	tile.setFillColor(Color::White);

	int cur = 0;
	for (int i = 0; i < HORIZONTAL_CELLS - 1; i++)
	{
		for (int j = 0; j < VERTICAL_CELLS - 1; j++)
		{
			if (maze_cells[i][j] & CELL_VISITED)
			{
				tile.setPosition(sf::Vector2f(SQUARE_WALL_SIZE + i * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE),
					SQUARE_WALL_SIZE + j * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE)));

				window.draw(tile);
			}

			if ((maze_cells[i][j] & CELL_PATH_S) == 0)
			{
				horizontalWall.setPosition(sf::Vector2f(i * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE), j * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE)));
				window.draw(horizontalWall);
			}
			else
			{
				horizontalSpace.setPosition(sf::Vector2f(i * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE), j * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE)));
				window.draw(horizontalSpace);
			}

			if ((maze_cells[i][j] & CELL_PATH_E) == 0)
			{
				verticalWall.setPosition(sf::Vector2f(i * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE), j * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE)));
				window.draw(verticalWall);
			}
			else
			{
				verticalSpace.setPosition(sf::Vector2f(i * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE), j * (SQUARE_WALL_SIZE + SQUARE_TILE_SIZE)));
				window.draw(verticalSpace);
			}
		}
	}
}

struct Coord
{
	int x;
	int y;
};

int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(HORIZONTAL_CELLS * SQUARE_CELL_SIZE, VERTICAL_CELLS * SQUARE_CELL_SIZE), "Maze!");

	vector<vector<int>> maze_cells;
	maze_cells.assign(HORIZONTAL_CELLS * SQUARE_CELL_SIZE, vector<int>(VERTICAL_CELLS * SQUARE_CELL_SIZE, 0));
	maze_cells[0][0] = CELL_VISITED;

	stack<Coord> my_stack;
	my_stack.push(Coord(0, 0));

	int visitedCells = 1;

	// Little lambda function to calculate index in a readable way
	auto offset_x = [&](int n)
	{
		return my_stack.top().x + n;
	};
	auto offset_y = [&](int n)
	{
		return my_stack.top().y + n;
	};

	int cur = 0;

	while (window.isOpen())
	{
		if (visitedCells < HORIZONTAL_CELLS * VERTICAL_CELLS)
		{
			vector<int> neighbours;
			// Step 1: create set of unvisited neighbours
			if (my_stack.top().y > 0 && // Do not work out of bounds
				(maze_cells[offset_x(0)][offset_y(-1)] & CELL_VISITED) == 0) // North
			{
				neighbours.push_back(0);
			}
			if (my_stack.top().x < HORIZONTAL_CELLS - 1 && // Do not work out of bounds
				(maze_cells[offset_x(1)][offset_y(0)] & CELL_VISITED) == 0) // East
			{
				neighbours.push_back(1);
			}
			if (my_stack.top().y < VERTICAL_CELLS - 1 &&  // Do not work out of bounds
				(maze_cells[offset_x(0)][offset_y(1)] & CELL_VISITED) == 0) // South
			{
				neighbours.push_back(2);
			}
			if (my_stack.top().x > 0 && // Do not work out of bounds
				(maze_cells[offset_x(-1)][offset_y(0)] & CELL_VISITED) == 0) // West
			{
				neighbours.push_back(3);
			}

			if (!neighbours.empty())
			{
				// Choose random neighbor to make it available
				int next_cell_dir = neighbours[cur % neighbours.size()]; //rand() % neighbours.size()];
				cur = (cur + 1) % neighbours.size();

				// Create a path between the neighbour and the current cell
				switch (next_cell_dir)
				{
				case 0: // North
					maze_cells[offset_x(0)][offset_y(-1)] |= CELL_VISITED | CELL_PATH_S;
					maze_cells[offset_x(0)][offset_y(0)] |= CELL_PATH_N;
					my_stack.push(Coord(offset_x(0), offset_y(-1)));
					break;

				case 1: // East
					maze_cells[offset_x(1)][offset_y(0)] |= CELL_VISITED | CELL_PATH_W;
					maze_cells[offset_x(0)][offset_y(0)] |= CELL_PATH_E;
					my_stack.push(Coord(offset_x(1), offset_y(0)));
					break;

				case 2: // South
					maze_cells[offset_x(0)][offset_y(1)] |= CELL_VISITED | CELL_PATH_N;
					maze_cells[offset_x(0)][offset_y(0)] |= CELL_PATH_S;
					my_stack.push(Coord(offset_x(0), offset_y(1)));
					break;

				case 3: // West
					maze_cells[offset_x(-1)][offset_y(0)] |= CELL_VISITED | CELL_PATH_E;
					maze_cells[offset_x(0)][offset_y(0)] |= CELL_PATH_W;
					my_stack.push(Coord(offset_x(-1), offset_y(0)));
					break;

				}

				visitedCells++;
			}
			else
			{
				my_stack.pop();
			}
		}
		else
		{
			// Do nothing - draw only.
		}
		
		// Redraw.
		window.clear(Color::Blue);

		drawMaze(window, maze_cells);

		window.display();
	}

	return 0;
}