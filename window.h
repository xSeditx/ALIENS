#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstddef>

#define GetRandom( min, max )     ((rand() % (int)(((max) + 1) - (min))) + (min))  
#define RANDOM(x)                 ((rand() * (1.0 / (1.0 + RAND_MAX))) * (x))
#define RANDOM_RANGE(x)           (RANDOM(x * 2) - (x))

#define RADIANS(angle)            ((angle) * .0174532925199444)

#define LOOP(x)                   for(std::size_t count = 0; count < (x); count++)
#define FOR_LOOP(x,c)             for(std::size_t x = 0; x < (c); x++)


constexpr std::size_t SCREEN_WIDTH = 1280;
constexpr std::size_t SCREEN_HEIGHT = 960;


extern double COS[360], SIN[360];
#define _COS(a) COS[(int)a]
#define _SIN(a) SIN[(int)a]


template <typename T>
static constexpr int color_from_rgb(T r, T g, T b)
{
    return static_cast<unsigned>(b) + (static_cast<unsigned>(g) << 8u) + (static_cast<unsigned>(r) << 16u);
}


class WINDOW
{

public:
    WINDOW() = default;
    ~WINDOW() = default;

    WINDOW(int, int, int, int, char*);


    int X,
        Y,
        WIDTH,
        HEIGHT;

    char *TITLE;


    SDL_Window             *HWND;
    SDL_Texture            *BACK_BUFFER;
    SDL_Renderer           *RENDER;

    Uint32                  *WINDOW_PIXELS;

    Uint32                  WINDOW_FORMAT;
    SDL_Event               EVENT;
    SDL_Surface            *WINDOW_SURFACE;
    SDL_PixelFormat        *MAPPING_FORMAT;
    SDL_Point               MOUSE_POSITION;
    SDL_Point               MOUSE_VELOCITY;

    double MOUSE_ANGLE;
    struct
    {
        bool LEFT;
        bool RIGHT;
    } MOUSE_BUTTON;

    unsigned long DRAW_COLOR;
    int  FPS;
    void SET_DRAW_COLOR(unsigned long Col);

    double TIME;

    int   FRAME_COUNTER;
    int   CYCLE_COUNTER;

    int CyclePerSecond;

    float CYCLE_TIMER;
    float TIMER;

};


int       KEYBOARD_HANDLER(SDL_Keycode sym);

float     NEWX(float x, float dist, float angle);
float     NEWY(float y, float dist, float angle);

void      SYNC();
void      clear_screen();
void      PRINT_SCREEN(char *text);
void      SET_PIXEL(int, int, Uint32);
void      SET_PIXELII(int, int, Uint32);
void      SET_DRAW_COLOR(unsigned long Col);
void      LINE(int x1, int y1, int x2, int y2);
void      LINE2(int x, int y, float Angle, std::size_t Length);
void      CIRCLE(int x, int y, float radius);
void      FILLED_CIRCLE(int x, int y, float radius);
void      BOX(int X1, int Y1, int X2, int Y2);
void      FILLED_BOX(int X1, int Y1, int X2, int Y2);


float    FindAngle(SDL_Point A, SDL_Point B);

bool      Is_CLICK(int X, int Y);
bool      LOOP_GAME();
bool      SET_ACTIVE_WINDOW(WINDOW *active);

extern WINDOW   *SCREEN;
extern int       WINDOW_OFFSET;
extern int       LEFT_BOUNDS;
extern int       TOP_BOUNDS;
extern int       RIGHT_BOUNDS;
extern int       BOTTOM_BOUNDS;
