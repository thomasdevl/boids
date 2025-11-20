//
// Created by Thomas Devlamminck on 20/11/2025.
//
#pragma once
#include "boid.h"

class PieBoid : public Boid {
public:
    int radius;

    PieBoid(float x, float y, float vx, float vy, Uint32 color,int filled, int radius)
        : Boid(x, y, vx, vy, color, filled), radius(radius){}

    void printPosition() override;
    void draw(SDL_Renderer *renderer) override;

    // Specific to PieBoid
    [[nodiscard]] float getStartAngle() const;
    [[nodiscard]] float getEndAngle() const;
};