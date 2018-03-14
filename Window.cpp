//                                                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                                                       
//        SDL FRAMEWORK FOR GRAPHICS OPERATIONS                                                                                                                                                                                                                                                                                                                        
//                                                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                                                       
// TARGET: WINDOW *SCREEN pointer. This is where all functions read from and draw to.   
//
// TODO: Fix the KEYBOARD Handler so that a callback class is made and various functions can be added to the 
// callback Vector 
// Change the structure of the Mouse information in the class so its called MOUSE.Position Angle etc... etc
//=====================================================================================================================                                                                                                                                                                                                                                                                                                                                      


#include"window.h"
#include<windows.h>
#include<iostream>
#include<vector>

#define __DEBUG_

WINDOW *SCREEN = { 0 };
int WINDOW_OFFSET = 1;
int LEFT_BOUNDS = 0 + WINDOW_OFFSET;
int TOP_BOUNDS = 0 + WINDOW_OFFSET;
int RIGHT_BOUNDS = SCREEN_WIDTH - WINDOW_OFFSET;
int BOTTOM_BOUNDS = SCREEN_HEIGHT - WINDOW_OFFSET;

float Cos[360];// = {0}, 
float Sin[360]; // = {0};


// CONSTRUCTS A BASIC WINDOW AND BACKBUFFER ASSOCIATED WITH WINDOW

WINDOW::WINDOW(int x, int y, int width, int height, char *title)
{
    X = x;
    Y = y;
    WIDTH = width;
    HEIGHT = height;
    TITLE = title;

    MOUSE_VELOCITY.x = MOUSE_VELOCITY.y = 0; MOUSE_ANGLE = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)std::cout << "Init Video Error" << std::endl;
    HWND = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0); if (!HWND)                       std::cout << "ERROR: Initializing SDL Window" << std::endl;
    RENDER = SDL_CreateRenderer(HWND, -1, SDL_RENDERER_ACCELERATED);                                                           if (!RENDER)                     std::cout << "ERROR: Initializing SDL Renderer" << std::endl;

    if (SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 255) != 0)std::cout << "Set Color Error" << std::endl;

    BACK_BUFFER = SDL_CreateTexture(RENDER, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    SDL_SetRenderTarget(RENDER, BACK_BUFFER);
    SDL_SetWindowResizable(HWND, SDL_TRUE);

    WINDOW_SURFACE = SDL_GetWindowSurface(HWND);
    WINDOW_FORMAT = SDL_GetWindowPixelFormat(HWND);

    WINDOW_PIXELS = new Uint32[WIDTH * (HEIGHT + 400) + 1];
    memset(WINDOW_PIXELS, 255, WIDTH * HEIGHT * sizeof(Uint32));

    FRAME_COUNTER = 0;
    CYCLE_COUNTER = 0;
    FPS = 0;
    CyclePerSecond = 0;
    LOOP(360)
    {
        Cos[count] = cos(RADIANS(count));
        Sin[count] = sin(RADIANS(count));
    }

    //  for(int Angle =0; Angle > 360; Angle++)Cos[360] = cos(RADIANS(Angle));
    //  for(int Angle =0; Angle > 360; Angle++)Sin[360] = sin(RADIANS(Angle));


}

// MESSAGE HANDLER RETURNS FALSE WHEN APPLICATION IS CLOSED OUT
bool LOOP_GAME()
{

    SCREEN->CYCLE_COUNTER++;
    float delta = SDL_GetTicks() - SCREEN->CYCLE_TIMER;
    SCREEN->CyclePerSecond = 1000 / delta;

    SCREEN->CYCLE_TIMER = SDL_GetTicks();


    bool leftMouseButtonDown = false;
    SDL_PollEvent(&SCREEN->EVENT);
    switch (SCREEN->EVENT.type)
    {
    case SDL_QUIT:
        return 0;
        break;

    case SDL_MOUSEBUTTONUP:
        if (SCREEN->EVENT.button.button == SDL_BUTTON_LEFT)
            leftMouseButtonDown = false;
        SCREEN->MOUSE_BUTTON.LEFT = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (SCREEN->EVENT.button.button == SDL_BUTTON_LEFT)
            leftMouseButtonDown = true;
        SCREEN->MOUSE_BUTTON.LEFT = true;
        break;
    case SDL_MOUSEMOTION:
        //int mouseX = SCREEN->EVENT.motion.x;
        //int mouseY = SCREEN->EVENT.motion.y;

        SDL_Point DELTA;
        DELTA.x = SCREEN->EVENT.motion.x;
        DELTA.y = SCREEN->EVENT.motion.y;

        SCREEN->MOUSE_VELOCITY.x = SCREEN->EVENT.motion.x - SCREEN->MOUSE_POSITION.x;
        SCREEN->MOUSE_VELOCITY.y = SCREEN->EVENT.motion.y - SCREEN->MOUSE_POSITION.y;

        SCREEN->MOUSE_ANGLE = FindAngle(SCREEN->MOUSE_POSITION, DELTA);

        SCREEN->MOUSE_POSITION.x = SCREEN->EVENT.motion.x;
        SCREEN->MOUSE_POSITION.y = SCREEN->EVENT.motion.y;

        break;
    case SDL_KEYDOWN:
        //#ifdef KEYBOARD_HANDLER;
        KEYBOARD_HANDLER(SCREEN->EVENT.key.keysym.sym);
        //#endif
        break;
    }
    return true;
}

// CLEARS AND DISPLAYS THE BACKBUFFER
void clear_screen()
{
    SCREEN->FRAME_COUNTER++;
    //    float fps = 1000 / 60;
    float delta = SDL_GetTicks() - SCREEN->TIMER;
    //   if (delta < fps){SDL_Delay(fps - delta);Print(delta);}
      // if ((delta * 1000) < fps) ; //SDL_Delay(fps - delta); 
       //if(SDL_GetTicks() - SCREEN->TIMER > 1000){
    SCREEN->FPS = 1000 / delta;//SDL_GetTicks() - SCREEN->TIMER/1000 ;////SCREEN->FRAME_COUNTER ;// / (

    SCREEN->TIMER = SDL_GetTicks();

    //SDL_RenderClear(SCREEN->RENDER);
    memset(SCREEN->WINDOW_PIXELS, 0, SCREEN->WIDTH * SCREEN->HEIGHT * sizeof(*SCREEN->WINDOW_PIXELS));

}

void SYNC()
{
    SDL_UpdateTexture(SCREEN->BACK_BUFFER, NULL, SCREEN->WINDOW_PIXELS, SCREEN->WIDTH * sizeof(*SCREEN->WINDOW_PIXELS)); // SLOW
    SDL_RenderCopy(SCREEN->RENDER, SCREEN->BACK_BUFFER, NULL, NULL);
    SDL_RenderPresent(SCREEN->RENDER);
}

// MAKES THE DEFINED WINDOW THE TARGET FOR THE REST OF THE API
bool SET_ACTIVE_WINDOW(WINDOW *active)
{
    SCREEN = active;
    return 1;
}


// LOCKS & UNLOCKS THE SCREEN TEXTURE INTO MEMORY FOR FAST READ AND WRITTING, POSSIBLY UNNEEDED THE WAY I DO THE BUFFERING
void LOCK_PIXELS()
{
    int   PITCH;
    void *PIXELS;
    SDL_LockTexture(SCREEN->BACK_BUFFER, NULL, &PIXELS, &PITCH);
}
void UNLOCK_PIXELS()
{
    SDL_UnlockTexture(SCREEN->BACK_BUFFER);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// VECTOR MANIPULATION /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FINDS THE NEX X/Y POSITION A DISTANCE FROM THE GIVEN X/Y AT A GIVEN ANGLE
float NEWX(float x, float dist, float angle)
{
    return x + dist * cos(RADIANS(angle));
}
float NEWY(float y, float dist, float angle)
{
    return y + dist * sin(RADIANS(angle));
}

void  PRINT_SCREEN(char *text)
{
    std::cout << "ERROR YOU HAVE NOT SETUP THE PRINT FUNCTION FOR SDL YET DUMBASS" << std::endl;
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// PRIMATIVE DRAWING OPERATIONS ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//SETS THE DRAWING COLOR FOR ALL PRIMATIVE DRAWING OPERATIONS
void SET_DRAW_COLOR(unsigned long Col)
{
    SCREEN->DRAW_COLOR = Col;
}

//  SETTING A PIXELS WITHOUT PROTECTION 
//                                                            **NOTE** 
//Currently only overflows in Y direction and much space has been allocated as a buffer to prevent this issue... do not mistake it for being correct value just because it does not overflow
void SET_PIXEL(int x, int y, Uint32 COLOR)
{
    SCREEN->WINDOW_PIXELS[(y * SCREEN->WIDTH) + x] = COLOR;                                                         //((Uint32*)pixels)[ x + ( y * SCREEN_WIDTH ) ] = SDL_MapRGB(fmt, 255, 0, 0);
}

//SETS A PIXEL WITH BOUNDS CHECKING FOR THE SCREEN
void SET_PIXELII(int x, int y, Uint32 COLOR)
{
    x = (SCREEN_WIDTH + (x % SCREEN_WIDTH)) % SCREEN_WIDTH;
    y = (SCREEN_HEIGHT + (y % SCREEN_HEIGHT)) % SCREEN_HEIGHT;

    SCREEN->WINDOW_PIXELS[(y * SCREEN->WIDTH) + x] = COLOR;
}

void FILLED_BOX(int X1, int Y1, int X2, int Y2)
{
    double xt, yt, xb, yb;

    if (X1 < X2)
    {
        xt = X1; xb = X2;
    }
    else
    {
        xt = X2; xb = X1;
    }
    if (Y1 < Y2)
    {
        yt = Y1; yb = Y2;
    }
    else
    {
        yt = Y2; yb = Y1;
    }

    for (int y = yt; y < yb; y++)
    {
        LINE(xt, y, xb, y);
    }
}

void BOX(int X1, int Y1, int X2, int Y2)
{
    LINE(X1, Y1, X1, Y2);
    LINE(X1, Y2, X2, Y2);
    LINE(X2, Y2, X2, Y1);
    LINE(X2, Y1, X1, Y1);
}

void CIRCLE(int x, int y, float radius)
{
    float X1 = x + .5, Y1 = +.5;
    for (float Angle = 0; Angle < 360; Angle++)
    {
        X1 = x + radius * _COS((int) Angle);
        Y1 = y + radius * _SIN((int) Angle);
        SET_PIXEL(X1, Y1, SCREEN->DRAW_COLOR);
    }
}

void FILLED_CIRCLE(int x, int y, float radius)
{
    float X1 = x, Y1 = y;
    unsigned long color = SCREEN->DRAW_COLOR;
    for (float r = 0; r < radius; r++)
    {
        float Theta = (360 / (8 * r));
        for (float Angle = 0; Angle < 360; Angle += Theta)
        {
            X1 = x + r * _COS((int) (Angle));    //X1 = x + r * cos(RADIANS(Angle));
            Y1 = y + r * _SIN((int) (Angle));    //Y1 = y + r * sin(RADIANS(Angle));

            SET_PIXELII(X1, Y1, color);
        }
    }
}

void LINE2(int x, int y, float Angle, int Length)
{
    float Xpos = x, Ypos = y;
    unsigned long color = SCREEN->DRAW_COLOR;
    LOOP(Length)
    {
        Xpos += _COS((int) (Angle)),
            Ypos += _SIN((int) (Angle));
        SET_PIXELII(Xpos, Ypos, color);
    }
}

void LINE(int x1, int y1, int x2, int y2)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;
    dx1 = abs(dx); dy1 = abs(dy); //fabs
    px = 2 * dy1 - dx1; py = 2 * dx1 - dy1;

    long c = SCREEN->DRAW_COLOR; //RGB(155,155,55);

    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1; y = y1;
            xe = x2;
        }
        else
        {
            x = x2; y = y2;
            xe = x1;
        }
        SET_PIXELII(x, y, c);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
            {
                px = px + 2 * dy1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                }
                else
                {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            SET_PIXELII(x, y, c);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        SET_PIXELII(x, y, c);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
            {
                py = py + 2 * dx1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            SET_PIXELII(x, y, c);
        }
    }
}


// RETURNS ANGLE IN DEGREES
float FindAngle(SDL_Point A, SDL_Point B)
{
    return atan2f(B.y - A.y, B.x - A.x) / 3.14159 * 180;
}


// Checks an area 10x10 to see if the mouse is in that area. Fix it in the future to truly return if the mouse is currently clicked
bool Is_CLICK(int X, int Y)
{
    if ((int) (X) <= SCREEN->MOUSE_POSITION.x + 10 &&
        (int) (X) >= SCREEN->MOUSE_POSITION.x - 10
        &&
        (int) (Y) <= SCREEN->MOUSE_POSITION.y + 10 &&
        (int) (Y) >= SCREEN->MOUSE_POSITION.y - 10) return true;
    return false;
}






/// https://gamedev.stackexchange.com/questions/63617/altering-pixel-value-in-an-sdl2-texture
/// Why you are locking the texture? You shoudn't access texture memory directly, in general accessing GPU memory has low performance. 
///    You should work with SDL_Surface all time then use SDL_UpdateTexture(sdlTexture, NULL, pSurface->pixels, pSurface->pitch); 
/// when your drawing is done, and finally call SDL_RenderCopy(renderer, pTexture, NULL, NULL);

//void *pixels;
//int pitch;
//SDL_PixelFormat *fmt;
//Uint32 format = SDL_GetWindowPixelFormat(window);
//fmt = SDL_AllocFormat(format);

//SDL_LockTexture(pTexture, NULL, &pixels, &pitch);
//((Uint32*)pixels)[ x + ( y * SCREEN_WIDTH ) ] = SDL_MapRGB(fmt, 255, 0, 0);
//SDL_UnlockTexture(pTexture);
//SDL_FreeFormat(fmt);

//http://sdl.5483.n7.nabble.com/SDL-UpdateTexture-vs-Locking-td38080.html
