//
// Created by Thomas Devlamminck on 20/11/2025.
//
#include "pieBoid.h"
#include <iostream>

float PieBoid::getStartAngle() const {
    float angle = atan2(this->vy, this->vx) + M_PI;
    float delta = 15.0f; // half the wedge in degrees
    float angleDeg = angle * (180.0f / M_PI); // convert radians to degrees

    float startAngle = angleDeg - delta;

    return startAngle;
}

float PieBoid::getEndAngle() const {
    float angle = atan2(this->vy, this->vx) + M_PI;
    float delta = 15.0f; // half the wedge in degrees
    float angleDeg = angle * (180.0f / M_PI); // convert radians to degrees

    float endAngle = angleDeg + delta;

    return endAngle;
}

void PieBoid::printPosition() {
    std::cout << "PieBoid ( x: " << x << ", y: " << y << ", vx: " << vx << ", vy: " << vy << ", radius:" << radius << ", id:" << id << " )\n";
}

void PieBoid::draw(SDL_Renderer *renderer) { // 0xFFFF0000
    if (filled) {
        pieColor(renderer, this->x, this->y, this->radius, this->getStartAngle(), this->getEndAngle(), this->color);
    } else {
        filledPieColor(renderer, this->x, this->y, this->radius, this->getStartAngle(), this->getEndAngle(), this->color);
    }
    //lineColor(renderer, this->x, this->y, this->x+this->vx*25,this->y+this->vy*25,this->color-255);
}