#include "charBoid.h"
#include "pieBoid.h"
#include "randomrange.h"
#ifdef _WIN32
#include <SDL.h>

#define not !

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#endif

#include "boid.h"

#include <iostream>
#include <random>

int const WIDTH = 1280;
int const HEIGHT = 720;

struct global_t {
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	// random
	std::random_device rd;
	std::default_random_engine eng;
	std::uniform_real_distribution<float> rand;

	global_t(float min, float max) : eng(rd()), rand(min,max) {}
};

global_t g(0,1);

RandomRange<int> rPos(0,WIDTH);
RandomRange<float> rSpeed(-0.2,0.2);

// boid array
std::array<std::unique_ptr<Boid>, 200> boidArray;

// mouse x and y
int mx, my;

// buffer for fps count
char fpsText[32] = "FPS: 0";

void do_render() {
	SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(g.renderer);

	for (auto &b: boidArray) {
		b->draw(g.renderer);
	}

	// Draw FPS in top-left corner (red text)
	stringRGBA(g.renderer, 10, 10, fpsText, 255, 0, 0, 255);

	SDL_RenderPresent(g.renderer);
}

void do_update() {

	// auto current_time = SDL_GetTicks64();

	float cohesion_weight = 0.5f;
	float separation_weight = 0.04f;
	float align_weight = 0.05f;

	float neighborhoodRadius = 100.0f;
	float minCohesionDistance = 1.0f;

	// Applying boid rules
	for (auto &b_ptr: boidArray) {
		Boid &b = *b_ptr;

		float avg_x = 0, avg_y = 0;        // cohesion
		float avg_vx = 0, avg_vy = 0;      // alignment
		float sep_x = 0, sep_y = 0;        // separation
		int count = 0;

		for (const auto &c : boidArray) {
			if (b.id == c->id) continue;

			float dx = c->x - b.x;
			float dy = c->y - b.y;
			float dist2 = dx*dx + dy*dy;

			if (dist2 < neighborhoodRadius * neighborhoodRadius) {

				avg_x += c->x;
				avg_y += c->y;

				avg_vx += c->vx;
				avg_vy += c->vy;

				sep_x += b.x - c->x;
				sep_y += b.y - c->y;

				count++;
			}
		}

		if (count > 0) {
			avg_x /= static_cast<float>(count);
			avg_y /= static_cast<float>(count);
			avg_vx /= static_cast<float>(count);
			avg_vy /=  static_cast<float>(count);

			// Compute forces
			float cohesion_x = avg_x - b.x;
			float cohesion_y = avg_y - b.y;

			float align_x = avg_vx - b.vx;
			float align_y = avg_vy - b.vy;

			float separation_x = sep_x;
			float separation_y = sep_y;

			// if too close don't use cohesion
			float distToAverage = sqrt(cohesion_x*cohesion_x + cohesion_y*cohesion_y);
			if (distToAverage < minCohesionDistance) {
				cohesion_weight = 0.0f;
			}

			// Combine with weights
			float ax = cohesion_x*cohesion_weight
						+ align_x*align_weight
						+ separation_x*separation_weight;
			float ay = cohesion_y*cohesion_weight
						+ align_y*align_weight
						+ separation_y*separation_weight;

			// Limit acceleration
			float len = sqrt(ax*ax + ay*ay);
			float maxAccel = 0.05f;
			if (len > maxAccel) {
				ax = ax / len * maxAccel;
				ay = ay / len * maxAccel;
			}

			// Update velocity
			b.vx += ax;
			b.vy += ay;

			// Limit speed
			float speed = sqrt(b.vx*b.vx + b.vy*b.vy);
			float maxSpeed = 3.0f;
			if (speed > maxSpeed) {
				b.vx = b.vx / speed * maxSpeed;
				b.vy = b.vy / speed * maxSpeed;
			}
		}

		b.update(WIDTH, HEIGHT);
	}
}

int main(int argc, char ** argv){

	int in = 100;

	if (argc >= 2) {
		try {
			in = std::stoi(argv[1]);
		} catch (const std::exception& e) {
			std::cerr << "Error in conversion: " << e.what() << std::endl;
		}
	}

	std::cout << "Number of boids: " << in << std::endl;


	SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
	SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");

	int status;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	g.window = SDL_CreateWindow("Boids",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (not g.window) {
		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	// get the default renderer
	g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED);
	if (not g.renderer) {
		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}


	// Adding random boid to array
	RandomRange<int> rInt(0, 2);
	RandomRange<int> rBool(0, 1);
	RandomRange<int> rColor(0, 255);

	for (auto &boi: boidArray) {

		int randomInt = rInt.get();

		Uint8 a = 255;
		Uint8 r = rColor.get();
		Uint8 g = rColor.get();
		Uint8 b = rColor.get();

		Uint32 color = (a << 24) | (r << 16) | (g << 8) | b;

		int filled = rBool.get();
		try {
			switch (randomInt) {
				case 0:
					boi = std::make_unique<PieBoid>(
						rPos.get(), rPos.get(),
						rSpeed.get(), rSpeed.get(),
						color,
						filled,
						30);
					break;
				case 1:
					boi = std::make_unique<CharBoid>(
						rPos.get(), rPos.get(),
						rSpeed.get(), rSpeed.get(),
						color,
						filled,
						"thomas");
					break;
				default:
					boi = std::make_unique<Boid>(
						rPos.get(), rPos.get(),
						rSpeed.get(), rSpeed.get(),
						color,
						filled);
					break;
			}
		} catch (BoidException &e) {
			std::cerr << "Failed to create Boid: " << e.what() << std::endl;
			return 1;
		}
	}

	const int TARGET_FPS = 60;
	const int FRAME_DELAY = 1000 / TARGET_FPS; // ms per frame

	bool end = false;
	Uint64 frameStart;
	int frametime;

	// FPS tracking
	Uint32 fpsTimer = SDL_GetTicks();
	int frameCount = 0;

	while (not end) {
		frameStart = SDL_GetTicks64();

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) end = true;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) end = true;
		}

		SDL_GetMouseState(&mx, &my);
		//std::cout << "mouse position: " << mx << ", " << my << std::endl;

		do_update();
		do_render();

		// FPS calculation
		frameCount++;
		if (SDL_GetTicks() - fpsTimer >= 1000) {
			snprintf(fpsText, sizeof(fpsText), "FPS: %d", frameCount);
			frameCount = 0;
			fpsTimer = SDL_GetTicks();
		}

		// Frame rate limitation
		frametime = SDL_GetTicks64() - frameStart;
		if (frametime < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY - frametime);
		}
	}

	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_CloseAudio();
	SDL_Quit();
	return 0;
}