//
// Created by Thomas Devlamminck on 19/11/2025.
//
#include "boid.h"
#include <iostream>

int Boid::nextID = 0;

void ::Boid::update(int WIDTH, int HEIGHT) {
    this->x += this->vx;
    this->y += this->vy;

    int margin = 50;

    if (this->x < -margin) this->x += WIDTH + 2*margin;
    if (this->x > WIDTH + margin) this->x -= WIDTH + 2*margin;

    if (this->y < -margin) this->y += HEIGHT + 2*margin;
    if (this->y > HEIGHT + margin) this->y -= HEIGHT + 2*margin;
}


void Boid::advanceTowards(int target_x, int target_y) {
    // update vx and vy to get towards a target with speed 1

    float speed = 0.1f;

    float dx = target_x - this->x;
    float dy = target_y - this->y;
    float length = std::sqrt((dx* dx) + (dy * dy));

    if (length == 0) return;

    this->vx += dx / length * speed;
    this->vy += dy / length * speed;
}

void Boid::printPosition() {
    std::cout << "Boid ( x: " << x << ", y: " << y << ", vx: " << vx << ", vy: " << vy << ", id:" << id << " )\n";
}