//
// Created by Thomas Devlamminck on 20/11/2025.
//

#ifndef ANT_WAR_DIRECT_BOXBOID_H
#define ANT_WAR_DIRECT_BOXBOID_H
#include "boid.h"


class CharBoid: public Boid {
public:
    const char* word;

    CharBoid(float x, float y, float vx, float vy, Uint32 color, int filled, const char* word)
        : Boid(x, y, vx, vy, color, filled), word(word) {

        if (!word || word[0] == '\0') {
            throw BoidException("CharBoid string cannot be empty");
        }
    }

    void printPosition() override;
    void draw(SDL_Renderer *renderer) override;
};

#endif //ANT_WAR_DIRECT_BOXBOID_H