#pragma once

#include <cstddef>
#include<vector>
#include<iostream>

#include"window.h"
#include"Vertex2D.h"
#include"brain.h"


#define MUTATION_RATE ((rand()%101)-50)

#define MAZE_SIZE          100

#define MAX_AGE            960
#define MAX_HEALTH         2000
#define MAX_CREATURES      1000
#define MAX_CELLS          30

#define WORLD_WIDTH        SCREEN_WIDTH  
#define WORLD_HEIGHT       SCREEN_HEIGHT

#define NUM_NEURONS        5
#define NUM_RESPONSES      3

#define NORTHING           0
#define ENEMY              1
#define MATE               2
#define WALL               3
#define FOOD               4
#define NUMBER_OF_NEURONS  2
#define AMOUNT_FOOD        2200


class Organism; struct Edge; // Forward Declaration

class Cell
{
public:
    Cell() = default;
    ~Cell() = default;
    Cell(Organism *Parent);


    Vector2D Offset,
        Starting,
        Velocity,
        Force;

    std::uint16_t ID;
    float         Friction;

    float Angle,
        Speed,
        Mass;

    unsigned long Color;

    Organism *Parent;
    std::vector<Edge> edges;

    Net Brain;

    void See();
    void Set_Position(int x, int y)
    {
        Offset.X = x;
        Offset.Y = y;
        Starting.X = x;
        Starting.Y = y;
    }

    int Collision();
};

struct Edge
{
    Edge() = default;
    ~Edge() = default;
    Edge(Cell *parent, Cell *other, unsigned char tension);


    Cell *Parent_ptr,
        *Child_ptr;

    int Child_ID;

    Vector2D Displacement,
        RestDistance;

    float Distance,
        Tension,
        Angle;

    float Get_Distance(const Cell &child)
    {
        float ret = sqrtf(Squared(Parent_ptr->Offset.X - child.Offset.X) + Squared(Parent_ptr->Offset.Y - child.Offset.Y));
        return ret;
    }
};


class Organism
{
public:
    Organism::Organism() = default;
    Organism::~Organism() = default;

    Organism(std::size_t id, unsigned char numcells, int x, int y);

    std::size_t ID;

    std::uint16_t Number_of_Cells;

    Vector2D Position,
        Potential,
        Velocity,
        Starting;

    float Distance_moved;

    std::vector<Cell> cells;

public:

    void Update(float Time_Step);
    void Draw() const;
    void Set_Position(float x, float y)
    {
        Starting = Position = Potential = { x, y };
    }


    Organism* Copy(Organism *Parent);
    Organism* Mutate(Organism  Parent);
};

extern int Collision(Organism *parent, Organism *List []);
