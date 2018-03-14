#include<iostream>
#include <string>     // std::string, std::stoi

#include"cell.h"
#include "brain.h"
#include "world.h" 
#include "timer.h"


double COS[360], SIN[360];
constexpr int number_of_creatures = 200;

World WORLD(960, 1280);


int KEYBOARD_HANDLER(SDL_Keycode sym)
{
    return 0;
}


void Draw_Edges(int Xx, int Yy, Cell Parent)
{
    for (Edge &Child : Parent.edges)
    {
        const float
            X = Parent.Offset.X + Xx,
            Y = Parent.Offset.Y + Yy;

        SET_DRAW_COLOR(Parent.Color);                   // SETS DRAW COLOR TO THAT OF THE CELL
        LINE2(X, Y, Child.Angle, Child.Distance / 2);   // DRAWS EACH EDGE
        FILLED_CIRCLE(X, Y, 5);                         // DRAWS EACH NODE
        WORLD.SetSpace(X, Y, Parent.Color);
    }
}

void main()
{
    for (int Angle = 0; Angle < 360; Angle++)
    {
        COS[Angle] = cos(RADIANS(Angle));
        SIN[Angle] = sin(RADIANS(Angle));
    }

    srand(1500);
    WINDOW main(0, 0, 1280, 960, "Multicelled automatons");
    SET_ACTIVE_WINDOW(&main);

    Organism * C[number_of_creatures];

    FOR_LOOP(count, number_of_creatures)
    {
        C[count] = new Organism(10, 100 + (rand() % SCREEN_WIDTH - 100), 100 + (rand() % SCREEN_HEIGHT - 100)); //1 + rand()%90);
    }


    int x = 0,
        y = 100,
        xjump = 100,
        yjump = 100;

    FOR_LOOP(count, number_of_creatures)
    {
        x += xjump;
        if (x > SCREEN_WIDTH - 100)
        {
            x = xjump;
            y += yjump;
        }
        C[count]->Set_Position(x, y);  // rand()%SCREENWIDTH;
        // C[count]->cells[2].Brain.Layers[0].Neurons[1].Value = .5;//RANDOM(1); // <- to Trigger Motion in Dormant cells;
    }


    Cell *Selected = nullptr;
    clear_screen();
    int Epoch = 0;
    int Generation = 0;

    float AverageAvg = 0;
    constexpr int FrameSkip = 3;

    while (LOOP_GAME())
    {
        if ((Epoch++) % 300 == 0)
        {
            Organism Parent;
            int Best = 0,
                Worst = 0;
            float Average = 0;

            FOR_LOOP(count, number_of_creatures - 1)
            {
                Average += C[count]->Distance_moved;

                if (C[count]->Distance_moved < C[Worst]->Distance_moved)
                    Worst = count;

                if (C[count]->Distance_moved > C[Best]->Distance_moved)
                    Best = count;
            }

            Average += C[number_of_creatures - 1]->Distance_moved / number_of_creatures;
            AverageAvg += Average;

            std::cout << '\n';
            std::cout << " FPS: " << SCREEN->FPS << "  CyclesPerSecond:" << SCREEN->CyclePerSecond << " Generation:" << Generation << '\n';
            std::cout << " Best distance:" << C[Best]->Distance_moved << "   Worst distance: " << C[Worst]->Distance_moved << '\n';
            std::cout << " Average:" << Average / number_of_creatures << "  Overall Avg :" << AverageAvg / Epoch << " " << '\n';

            Generation++;

            Parent.Copy(C[Best]);

            FOR_LOOP(count, number_of_creatures)
            {

                C[count]->Mutate(Parent);

                for (Cell &p : C[count]->cells)
                {
                    p.Brain.Layers[0].Neurons[0].Value = .5;
                    p.Brain.Layers[0].Neurons[1].Value = .5;
                    p.Brain.Layers[0].Neurons[2].Value = .5;

                }

                // C[count] = Mutate(*C[number_of_creatures - 1]); //1 + rand()%90);
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        if (Epoch % FrameSkip == 0)
        {
            clear_screen();
        }

        if (!SCREEN->MOUSE_BUTTON.LEFT)
        {
            Selected = nullptr;
        }

        FOR_LOOP(count, number_of_creatures)
        {
            auto const & Creature = C[count];

            if (Selected == nullptr && SCREEN->MOUSE_BUTTON.LEFT == true)
            {
                for (Cell &cell : Creature->cells)
                {
                    if (Is_CLICK(cell.Offset.X + Creature->Potential.X, cell.Offset.Y + Creature->Potential.Y))
                        Selected = &cell;
                }
            }

            if (Selected != nullptr)
            {

                constexpr float MASS = 5;

                Selected->Force.X = MASS * SCREEN->MOUSE_VELOCITY.x; // .03, TIME STEP? 
                Selected->Force.Y = MASS * SCREEN->MOUSE_VELOCITY.y; //(rand()%3-1) * 1;

                SET_DRAW_COLOR(color_from_rgb(255, 255, 255));
                FILLED_CIRCLE(Selected->Offset.X + Selected->Parent->Potential.X,
                    Selected->Offset.Y + Selected->Parent->Potential.Y, 7);
            }

            Creature->Update(10);
            Collision(Creature, &C[0]);

            if (Epoch % FrameSkip == 0)
            {
                Creature->Draw();
            }

        }

        int yy = 0;
        FOR_LOOP(count, number_of_creatures)
        {
            yy += 5;
            LINE2(5, yy, 0, C[count]->Distance_moved);
        }

        if (Epoch%FrameSkip == 0)
        {
            SYNC();
        }
    }

    // CLEAN UP
    // delete *C;
}
