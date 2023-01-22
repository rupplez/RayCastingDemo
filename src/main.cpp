#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#define window_width 640
#define window_height 480

#define GETRAD(DIR) DIR*(M_PI/180)

using namespace std;


int FOV = 40;

const int M_W = 20;
const int M_H = 20;
const int B_SIZE = 32;
int map[M_H][M_W]{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

float pl_x = 64;
float pl_y = 288;
float pl_dir = 315.0f;
float spd = 3.0f;

void MoveVec(float _dir, float _spd) { pl_x += cos(GETRAD(_dir))*_spd; pl_y -= sin(GETRAD(_dir))*_spd;}

bool checkWall(float x, float y) {
	if (map[int(y / B_SIZE)][int(x / B_SIZE)])
		return true;
	else
		return false;
}

void RenderMap(SDL_Renderer* rd) {
	SDL_SetRenderDrawColor(rd, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(rd);
	float rayX;
	float rayY;
	float rayDir;
	float wallHeight;
	rayDir = pl_dir + FOV / 2;
	for (int iterX = 0; iterX < window_width; iterX++, rayDir -= (float)FOV / (float)window_width) {
		rayX = pl_x;
		rayY = pl_y;
		while (!checkWall(rayX, rayY)) {
			rayX += cos(GETRAD(rayDir));
			rayY -= sin(GETRAD(rayDir));
		}
		wallHeight = pow(window_height / 2, 2) / sqrt(pow(pl_x - rayX, 2) + pow(pl_y - rayY, 2));
		SDL_SetRenderDrawColor(rd, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(rd, iterX, (window_height - wallHeight) / 2, iterX, (window_height + wallHeight) / 2);
	}
	//std::cout << "Debugging" << '\n';
	SDL_RenderPresent(rd);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

				RenderMap(renderer);

				float t_x = pl_x;
				float t_y = pl_y;

				const Uint8* keystates = SDL_GetKeyboardState(NULL);

                while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							pl_dir += 1.0f*5;
							break;
						case SDLK_RIGHT:
							pl_dir -= 1.0f*5;
							break;
						/*case SDLK_UP:
							FOV += 10;
							break;
						case SDLK_DOWN:
							FOV -= 10;
							break;*/
						default:
							break;
						}
						break;
					case SDL_QUIT:
						done = SDL_TRUE;
						break;
					default:
						break;
					}
                }

				if (keystates[SDL_SCANCODE_W])
					MoveVec(pl_dir, spd);
				else if (keystates[SDL_SCANCODE_A])
					MoveVec(pl_dir + 90.0f, spd);
				else if (keystates[SDL_SCANCODE_S])
					MoveVec(pl_dir - 180.0f, spd);
				else if (keystates[SDL_SCANCODE_D])
					MoveVec(pl_dir - 90.0f, spd);

				if (t_x != pl_x || t_y != pl_y) {
					std::cout << "x : " << pl_x << '\n' << "y : " << pl_y << '\n';
					if ((int)(pl_x / (M_W * B_SIZE) - (pl_x < 0)) || (int)(pl_y / (M_H * B_SIZE)) - (pl_y < 0)) //
						std::cout << "Out of a map!" << '\n';
					std::cout << '\n';
				}
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}