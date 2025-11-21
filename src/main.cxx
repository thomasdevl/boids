// Modified by Thomas Devlamminck on 20/11/2025.
// 1) class (boid.h, charBoid.h, pieBoid,h, randomrange.h)
// 2) heritage avec virtual  (boid.h, charBoid.h, pieBoid,h)
// 3) exception (main.cxx, boid.h, charBoid.h, pieBoid,h, randomrange.h)
// 4) utilisation de la librairie standard (main.cxx, randomrange.h, boids.h, boids.cpp, charBoid.cpp, pieBoid.cppp)
// 5) templates (randomrange.h)
// 6) RTTI (main.cxx)

#ifdef _WIN32
#include <SDL.h>
#define not !
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#endif

#include "boid.h"
#include "charBoid.h"
#include "pieBoid.h"
#include "randomrange.h"

#include <iostream>
#include <vector>
#include <memory>
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

std::vector<std::unique_ptr<Boid>> boidArray;

// mouse x and y
int mx, my;

// buffer for fps count
char fpsText[32] = "FPS: 0";
char boidCount[32] = "Boids : 0";
char charBoidCount[32] = "CharBoids : 0";
char pieBoidCount[32] = "PieBoids : 0";

void do_render() {
    SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(g.renderer);

    for (auto &b: boidArray) {
        b->draw(g.renderer);
    }

    // statistics
    stringRGBA(g.renderer, 10, 10, fpsText, 10, 255, 10, 255);
    stringRGBA(g.renderer, 10, 25, boidCount, 255, 0, 255, 255);
    stringRGBA(g.renderer, 10, 40, charBoidCount, 255, 0, 255, 255);
    stringRGBA(g.renderer, 10, 55, pieBoidCount, 255, 0, 255, 255);

    SDL_RenderPresent(g.renderer);
}

void do_update() {

    float cohesion_weight = 0.5f;
    float separation_weight = 0.04f;
    float align_weight = 0.05f;

    float neighborhoodRadius = 100.0f;
    float minCohesionDistance = 1.0f;

    for (auto &b_ptr: boidArray) {
        Boid &b = *b_ptr;

        float avg_x = 0, avg_y = 0;
        float avg_vx = 0, avg_vy = 0;
        float sep_x = 0, sep_y = 0;
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
            avg_vy /= static_cast<float>(count);

            float cohesion_x = avg_x - b.x;
            float cohesion_y = avg_y - b.y;

            float align_x = avg_vx - b.vx;
            float align_y = avg_vy - b.vy;

            float separation_x = sep_x;
            float separation_y = sep_y;

            float distToAverage = sqrt(cohesion_x*cohesion_x + cohesion_y*cohesion_y);
            if (distToAverage < minCohesionDistance) {
                cohesion_weight = 0.0f;
            }

            float ax = cohesion_x*cohesion_weight
                     + align_x*align_weight
                     + separation_x*separation_weight;
            float ay = cohesion_y*cohesion_weight
                     + align_y*align_weight
                     + separation_y*separation_weight;

            float len = sqrt(ax*ax + ay*ay);
            float maxAccel = 0.05f;
            if (len > maxAccel) {
                ax = ax / len * maxAccel;
                ay = ay / len * maxAccel;
            }

            b.vx += ax;
            b.vy += ay;

            float speed = sqrt(b.vx*b.vx + b.vy*b.vy);
            float maxSpeed = 3.0f;
            if (speed > maxSpeed) {
                b.vx = b.vx / speed * maxSpeed;
                b.vy = b.vy / speed * maxSpeed;
            }
        }

        // RTTI behavior based on the class
        if (auto *cb = dynamic_cast<CharBoid*>(b_ptr.get())) {
            // CharBoids follow the mouse
            float dx = mx - cb->x;
            float dy = my - cb->y;
            cb->vx += dx * 0.0005f;
            cb->vy += dy * 0.0005f;
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

    g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED);
    if (not g.renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Random initialization
    RandomRange<int> rInt(0, 2);
    RandomRange<int> rBool(0, 1);
    RandomRange<int> rColor(0, 255);

    boidArray.reserve(in);

    for (int i = 0; i < in; i++) {
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
                    boidArray.push_back(std::make_unique<PieBoid>(
                        rPos.get(), rPos.get(),
                        rSpeed.get(), rSpeed.get(),
                        color,
                        filled,
                        30));
                    break;

                case 1:
                    boidArray.push_back(std::make_unique<CharBoid>(
                        rPos.get(), rPos.get(),
                        rSpeed.get(), rSpeed.get(),
                        color,
                        filled,
                        "thomas"));
                    break;

                default:
                    boidArray.push_back(std::make_unique<Boid>(
                        rPos.get(), rPos.get(),
                        rSpeed.get(), rSpeed.get(),
                        color,
                        filled));
                    break;
            }
        } catch (BoidException &e) {
            std::cerr << "Failed to create Boid: " << e.what() << std::endl;
            return 1;
        }
    }

    // RTTI used to get the statitics
    int countBoid = 0;
    int countCharBoid = 0;
    int countPieBoid = 0;

    for (auto &b: boidArray) {
        if (dynamic_cast<CharBoid*>(b.get())) countCharBoid++;
        else if (dynamic_cast<PieBoid*>(b.get())) countPieBoid++;
        else countBoid++;
    }

    // char boidCount[32] = "Boids : 0";
    snprintf(boidCount, sizeof(boidCount), "DefaultBoids: %d", countBoid);
    snprintf(charBoidCount, sizeof(charBoidCount), "CharBoids: %d", countCharBoid);
    snprintf(pieBoidCount, sizeof(pieBoidCount), "PieBoids: %d", countPieBoid);

    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;

    bool end = false;
    Uint64 frameStart;

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

        do_update();
        do_render();

        frameCount++;
        if (SDL_GetTicks() - fpsTimer >= 1000) {
            snprintf(fpsText, sizeof(fpsText), "FPS: %d", frameCount);
            frameCount = 0;
            fpsTimer = SDL_GetTicks();
        }

        int frametime = SDL_GetTicks64() - frameStart;
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