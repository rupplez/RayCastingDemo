#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#define window_width 640
#define window_height 480

#define DEGTORAD(DIR) DIR*(M_PI/180)

using namespace std;


int FOV = 40;

const int M_W = 20;
const int M_H = 20;
const int B_SIZE = 32;
int map[M_H][M_W]{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
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

void MoveVec(float _dir, float _spd) { pl_x += cos(DEGTORAD(_dir))*_spd; pl_y -= sin(DEGTORAD(_dir))*_spd;}

int checkWall(float x, float y) {
    return map[int(y / B_SIZE)][int(x / B_SIZE)];
}

void RenderMap(SDL_Renderer* rd) {
	SDL_SetRenderDrawColor(rd, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(rd);
	float rayX;
	float rayY;
	float rayDir;
    float dist;
	float wallHeight;
    int obj=0; //
	rayDir = pl_dir + FOV / 2;
	for (int iterX = 0; iterX < window_width; iterX++, rayDir -= (float)FOV / (float)window_width) {
		rayX = pl_x;
		rayY = pl_y;
		while (!(obj = checkWall(rayX, rayY))) {
			rayX += cos(DEGTORAD(rayDir));
			rayY -= sin(DEGTORAD(rayDir));
		}

        dist = sqrt(pow(pl_x - rayX, 2) + pow(pl_y - rayY, 2)) * cos(DEGTORAD((rayDir - pl_dir)));
		wallHeight = pow(window_height / 2, 2) / dist;

        SDL_SetRenderDrawColor(rd, 82, 255, 252, SDL_ALPHA_OPAQUE); //ceil
        SDL_RenderDrawLine(rd, iterX, 0, iterX, (window_height - wallHeight) / 2);

        if(obj==1) {
            SDL_SetRenderDrawColor(rd, 140, 140, 140, SDL_ALPHA_OPAQUE); //wall
		    SDL_RenderDrawLine(rd, iterX, (window_height - wallHeight) / 2, iterX, (window_height + wallHeight) / 2);
        } else if(obj==2) {
            SDL_SetRenderDrawColor(rd, 161, 80, 48, SDL_ALPHA_OPAQUE); //block
		    SDL_RenderDrawLine(rd, iterX, (window_height - wallHeight) / 2, iterX, (window_height + wallHeight) / 2);
        }

        SDL_SetRenderDrawColor(rd, 88, 204, 86, SDL_ALPHA_OPAQUE); //bottom
		SDL_RenderDrawLine(rd, iterX, (window_height + wallHeight) / 2, iterX, window_height);
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