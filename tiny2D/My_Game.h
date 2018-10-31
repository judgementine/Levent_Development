#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

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

		Grid::Point current_maze_position = {0, 0};

		Grid_Camera::Grid_Camera camera;
		
		unsigned int last_step_time = 0;

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

	}

	void begin_Play(unsigned int current_time)
	{
		Font::set_Screen_Size(&World::text, 16, 16);
		init_Maze(&World::maze);

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	

		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			int done = 0;
			int dir = next_Maze_Step(&World::current_maze_position, &World::maze, done);

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
		int dir = rand() % 4;
		if (dir == 1 && current_pos->y-2>=0)
		{
			if (g->data[(current_pos->y - 2)*current_pos->x]==1)
			{
				g->data[(current_pos->y - 2)*current_pos->x] = 2;
				g->data[(current_pos->y - 1)*current_pos->x] = 2;
				return 1;
			}
		}
		if (dir == 2 && current_pos->y + 2 < g->n_rows)
		{
			if (g->data[(current_pos->y + 2)*current_pos->x] == 1)
			{
				g->data[(current_pos->y + 2)*current_pos->x] = 2;
				g->data[(current_pos->y + 1)*current_pos->x] = 2;
				return 2;
			}
		}
		if (dir == 3 && current_pos->x - 2 >= 0)
		{
			if (g->data[(current_pos->x - 2)*current_pos->y] == 1)
			{
				g->data[(current_pos->x - 2)*current_pos->y] = 2;
				g->data[(current_pos->x - 1)*current_pos->y] = 2;
				return 3;
			}
		}
		if (dir == 4 && current_pos->x + 2 < g->n_cols)
		{
			if (g->data[(current_pos->x + 2)*current_pos->y] == 1)
			{
				g->data[(current_pos->x + 2)*current_pos->y] = 2;
				g->data[(current_pos->x + 1)*current_pos->y] = 2;
				return 4;
			}
		}
		return 0;
	}
}