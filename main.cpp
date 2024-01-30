#define SDL_MAIN_HANDLED

#include <iostream>
#include "SDL.h"

#include <thread>
#include <random>
#include <cmath>

const int ticksPerSecond = 60;
const int width = 1200;
const int height = 600;



std::random_device rd;
std::mt19937 rng(rd());
const int tics = 1000 / ticksPerSecond;


struct Vector {
    double x;
    double y;
};

struct Point {
    int x;
    int y;
    struct Vector velocity;
};

Point points[10000];

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

bool running = true;

void rotateByAngle(Point& point, double angle) {


    angle *= M_PI / 180;
    point.velocity.x = (point.velocity.x * cos(angle) - point.velocity.y * sin(angle));
    point.velocity.y = (point.velocity.x * sin(angle) + point.velocity.y * cos(angle));

}

void onTick() {
    std::uniform_int_distribution<int> uni(-20, 20);

    for (auto & point : points) {

        if (width < point.x || point.x < 0) {
            rotateByAngle(point, uni(rng));
            point.velocity.x *= -1;

        }
        if (height < point.y || point.y < 0) {
            rotateByAngle(point, uni(rng));
            point.velocity.y *= -1;

        }


        point.x += point.velocity.x;
        point.y += point.velocity.y;
    }
}

void tick()
{
    while(running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tics));
        onTick();
    }
}



#define FPS_INTERVAL 1.0 //seconds.

Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.
Uint32 fps_current; //the current FPS.
Uint32 fps_frames = 0; //frames passed since the last recorded fps.


void draw(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto & point : points) {
        DrawCircle(renderer, point.x, point.y, 4);
    }

    fps_frames++;
    if (fps_lasttime < SDL_GetTicks() - FPS_INTERVAL*1000)
    {
        fps_lasttime = SDL_GetTicks();
        fps_current = fps_frames;
        fps_frames = 0;
    }

    SDL_RenderPresent(renderer);

}

void freeSimulate(SDL_Renderer* renderer) {
    while(running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        draw(renderer);
    }
}

int main() {


    SDL_Window* window = SDL_CreateWindow("TEST", width/2, height/2, width, height, false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    for (auto & point : points) {
        point.x = 600;
        point.y = 300;
        point.velocity.x = 5;
        point.velocity.y = 0;
    }


    std::thread t1(tick);
    freeSimulate(renderer);
    t1.join();




    SDL_Quit();
    return 1;
}





