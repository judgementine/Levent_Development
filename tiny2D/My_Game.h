#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"
#include <time.h>
#include "Stack.h"

namespace My_Game
{
	//create this function
	void init_Maze(Grid::Grid *g);
	//create this function
	int next_Maze_Step(Grid::Point *current_pos, Grid::Grid *g, int &done);

	namespace Command
	{

	}

	namespace Assets
	{
		namespace Animation
		{
			
		}
	}

	namespace World
	{
		
		namespace Parameters
		{
			const int maze_n_rows = 80;
			const int maze_n_cols = 128;
			int step_frequency = 5;
			
		}

		Tileset::Tileset tileset;
		Font::Font text;

		Particle::Emitter emitter;

		Grid::Grid maze;

		Grid::Point current_maze_position = {1, 1};

		Grid_Camera::Grid_Camera camera;
		
		unsigned int last_step_time = 0;

		Stacking::Stacking PrevPositions;

	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Tileset::init(&World::tileset, "road_tileset.txt", Engine::renderer);
		Particle::init(&World::emitter, "dirt.txt", 512, Engine::renderer);
	
		Grid::init(&World::maze, World::Parameters::maze_n_rows, World::Parameters::maze_n_cols);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::Parameters::maze_n_cols;
		World::camera.world_coord.h = World::Parameters::maze_n_rows;
		Stacking::init(&World::PrevPositions, World::maze.n_cols*World::maze.n_rows);


	}

	void begin_Play(unsigned int current_time)
	{
		srand(time(NULL));
		Font::set_Screen_Size(&World::text, 16, 16);
		init_Maze(&World::maze);
		World::maze.data[World::current_maze_position.y*World::maze.n_cols+World::current_maze_position.x] = 2;

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	

		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			
			World::last_step_time = current_time;
			int done = 0;
			int dir = next_Maze_Step(&World::current_maze_position, &World::maze, done);
			if (done == 1)
			{
				init_Maze(&World::maze);
				World::maze.data[World::current_maze_position.y*World::maze.n_cols + World::current_maze_position.x] = 2;
			}
			
		}

	}

	void draw(unsigned int current_time)
	{

		SDL_RenderClear(Engine::renderer);

		Grid_Camera::calibrate(&World::camera);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, Engine::renderer);
		Particle::draw(&World::emitter, &World::camera, current_time, Engine::renderer);
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

	void init_Maze(Grid::Grid *g)
	{
		for (int i = 0; i < g->n_cols*g->n_rows; i++) g->data[i] = 1;
	}

	int next_Maze_Step(Grid::Point *current_pos, Grid::Grid *g, int &done)
	{
		int count = 0;
		int *direction= new int[4];
		if (current_pos->y-2>=0 && g->data[(current_pos->y - 2)*g->n_cols+current_pos->x]==1)
		{
			direction[count] = 1;
			count++;

		}
		if (current_pos->y + 2 < g->n_rows && g->data[(current_pos->y + 2)*g->n_cols+current_pos->x] == 1)
		{
			direction[count] = 2;
			count++;
		}
		if (current_pos->x - 2 >= 0 && g->data[(current_pos->x - 2)+(g->n_cols*current_pos->y)] == 1)
		{
			direction[count] = 3;
			count++;
		}
		if (current_pos->x + 2 < g->n_cols && g->data[(current_pos->x + 2)+(g->n_cols*current_pos->y)] == 1)
		{
			direction[count] = 4;
			count++;
			
		}
		if (count == 0)
		{
			if (World::PrevPositions.n_data<=0)
			{
				done = 1;
				return 0;
			}
			int prev = Stacking::Pop(&World::PrevPositions);
			current_pos->x = prev % g->n_cols;
			current_pos->y = (prev - current_pos->x) / g->n_cols;
			 
			return next_Maze_Step(current_pos, g, done);
		}
		int dir = rand() % (count);
		if (direction[dir] == 1)
		{
			World::maze.data[(World::current_maze_position.y - 2)*World::maze.n_cols + World::current_maze_position.x] = 2;
			World::maze.data[(World::current_maze_position.y - 1)*World::maze.n_cols + World::current_maze_position.x] = 2;
			Stacking::Push(&World::PrevPositions, World::current_maze_position.y*World::maze.n_cols + World::current_maze_position.x);
			World::current_maze_position.y = World::current_maze_position.y - 2;
		}
		if (direction[dir] == 2)
		{
			World::maze.data[(World::current_maze_position.y + 2)*World::maze.n_cols + World::current_maze_position.x] = 2;
			World::maze.data[(World::current_maze_position.y + 1)*World::maze.n_cols + World::current_maze_position.x] = 2;
			Stacking::Push(&World::PrevPositions, World::current_maze_position.y*World::maze.n_cols + World::current_maze_position.x);
			World::current_maze_position.y = World::current_maze_position.y + 2;

		}
		if (direction[dir] == 3)
		{
			World::maze.data[World::current_maze_position.y * World::maze.n_cols + World::current_maze_position.x - 2] = 2;
			World::maze.data[World::current_maze_position.y * World::maze.n_cols + World::current_maze_position.x - 1] = 2;
			Stacking::Push(&World::PrevPositions, World::current_maze_position.y * World::maze.n_cols + World::current_maze_position.x);
			World::current_maze_position.x = World::current_maze_position.x - 2;
		}
		if (direction[dir] == 4)
		{
			World::maze.data[World::current_maze_position.y * World::maze.n_cols + World::current_maze_position.x + 2] = 2;
			World::maze.data[World::current_maze_position.y * World::maze.n_cols + World::current_maze_position.x + 1] = 2;
			Stacking::Push(&World::PrevPositions, World::current_maze_position.y * World::maze.n_cols + World::current_maze_position.x);
			World::current_maze_position.x = World::current_maze_position.x + 2;
		}
		return direction[dir];
	}
}