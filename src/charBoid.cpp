//
// Created by Thomas Devlamminck on 20/11/2025.
//

#include "charBoid.h"

#include <iostream>

void CharBoid::draw(SDL_Renderer *renderer) {
    stringColor(renderer, this->x, this->y, this->word, this->color);
    //lineColor(renderer, this->x, this->y, this->x+this->vx*25,this->y+this->vy*25,this->color-255);
}

void CharBoid::printPosition() {
    std::cout << "CharBoid ( x: " << x << ", y: " << y << ", vx: " << vx << ", vy: " << vy << ", word:" << word << ", id:" << id << " )" << std::endl;
}