#pragma once
#define SDL_MAIN_HANDLED
#define _SDL_
#include <SDL.h>

#define GetRandom( min, max )     ((rand() % (int)(((max) + 1) - (min))) + (min))  
#define RANDOM(x)                 ((rand() * (1.0 / (1.0 + RAND_MAX))) * (x))
#define RANDOM_RANGE(x)           (RANDOM(x * 2) - (x))

#ifdef RGB
#undef RGB
#endif

#define RGB(r,g,b)                ((int)b + ((int)g<< 8) + ((int)r << 16))
#define RADIANS(angle)            ((angle) * .0174532925199444)

#define LOOP(x)                   for(int count = 0; count < (x); count++)
#define FOR_LOOP(x,c)             for(int x = 0; x < (c); x++)

#define SCREENWIDTH        1280 //680 
#define SCREENHEIGHT       960 //460 

#define _LOOP_GAME  LOOP_GAME()        // This is being done for future compatibility with various Graphics Libraries
#define _CLS        clear_screen()     //
#define _SYNC       SYNC()        //

#define _COS(a)       cos(RADIANS(a))  //Cos[(int)a]
#define _SIN(a)       sin(RADIANS(a))  //Sin[(int)a]


extern float Cos[360],
Sin[360];


// cos(RADIANS(a))
//sin(RADIANS(a))


class WINDOW
{

public:
	WINDOW(); ~WINDOW();
	WINDOW(int, int, int, int, char*);


	int       X,
		Y,
		WIDTH,
		HEIGHT;

	char     *TITLE;


	SDL_Window             *HWND;
	SDL_Texture            *BACK_BUFFER;
	SDL_Renderer           *RENDER;

	Uint32                  *WINDOW_PIXELS;

	Uint32                  WINDOW_FORMAT;
	SDL_Event               EVENT;
	SDL_Surface            *WINDOW_SURFACE;
	SDL_PixelFormat        *MAPPING_FORMAT;
	SDL_Point               MOUSE_POSITION,
		MOUSE_VELOCITY;

	double                  MOUSE_ANGLE;
	struct
	{
		bool LEFT,
			RIGHT;
	}MOUSE_BUTTON;

	unsigned long           DRAW_COLOR;
	int  FPS;
	void SET_DRAW_COLOR(unsigned long Col);

	double TIME;

	int   FRAME_COUNTER;
	int   CYCLE_COUNTER;

	int CyclePerSecond;

	float CYCLE_TIMER;
	float TIMER;

};


extern   int       KEYBOARD_HANDLER(SDL_Keycode sym);

extern   float     NEWX(float x, float dist, float angle);
extern   float     NEWY(float y, float dist, float angle);

extern   void      SYNC();
extern   void      clear_screen();
extern   void      PRINT_SCREEN(char *text);
extern   void      SET_PIXEL(int, int, Uint32);
extern   void      SET_PIXELII(int, int, Uint32);
extern   void      SET_DRAW_COLOR(unsigned long Col);
extern   void      LINE(int x1, int y1, int x2, int y2);
extern   void      LINE2(int x, int y, float Angle, int Length);
extern   void      CIRCLE(int x, int y, float radius);
extern   void      FILLED_CIRCLE(int x, int y, float radius);
extern   void      BOX(int X1, int Y1, int X2, int Y2);
extern   void      FILLED_BOX(int X1, int Y1, int X2, int Y2);


extern   float    FindAngle(SDL_Point A, SDL_Point B);

extern   bool      Is_CLICK(int X, int Y);
extern   bool      LOOP_GAME();
extern   bool      SET_ACTIVE_WINDOW(WINDOW *active);

extern   WINDOW   *SCREEN;
extern   int       WINDOW_OFFSET;
extern   int       LEFT_BOUNDS;
extern   int       TOP_BOUNDS;
extern   int       RIGHT_BOUNDS;
extern   int       BOTTOM_BOUNDS;




struct stack_array
{
	int ITERATOR;
	int SIZE;
	int *ARRAY;
	int ADD();
	int REMOVE();
	int GETLAST();
};
