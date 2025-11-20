//
// Created by Thomas Devlamminck on 19/11/2025.
//

#ifndef ANT_WAR_DIRECT_BOID_H
#define ANT_WAR_DIRECT_BOID_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class Boid {
public:
    float x, y;
    float vx, vy;
    int id;
    int filled;

    Uint32 color;

    static int nextID;

    Boid(): x(0), y(0), vx(0.0), vy(0.0), id(nextID++), color(0), filled(0){} // default constructor
    Boid(float x, float y, float vx, float vy, Uint32 color,int filled)
        : x(x), y(y), vx(vx), vy(vy), id(nextID++), color(color), filled(filled) {}

    virtual ~Boid() = default;

    void update(int WIDTH, int HEIGHT);
    void advanceTowards(int target_x, int target_y);

    virtual void draw(SDL_Renderer* renderer) {
        if (filled) {
            filledCircleColor(renderer, this->x, this->y, 10, this->color);
        } else {
            circleColor(renderer, this->x, this->y, 10, this->color);
        }
        //lineColor(renderer, this->x, this->y, this->x+this->vx*25,this->y+this->vy*25,this->color-255);
    }

    virtual void printPosition();
};

#endif //ANT_WAR_DIRECT_BOID_H