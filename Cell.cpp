﻿//
//
//   Cell class for building and manipulating organisms
//
//     _____      ______       __           __
//    /  _  \    |  ____}     |  |         |  |
//   | |  \ _|   | |___       |  |         |  |
//   | |         |  ___}      |  |         |  |
//   | |    _    | |          |  |         |  |
//   | | _/  |   | |____      |  |______   |  |______
//    \_____/    |______}     |_________|  |_________|
//
//
// TODO: Impliment better friction so that cells lose Velocity proportional to their mass and friction Coefficient
//==================================================================================================================
#include"cell.h"
#include"window.h"
#include"Vertex2D.h"
#include"world.h"

#include <cstddef>


//==================================================================================================================
//                                                         CELL CLASS                                               
//==================================================================================================================

Cell::Cell(Organism *parent)
    : Offset(RANDOM(300), RANDOM(300))
    , Starting(Offset)
    , Velocity()
    , Force()

    , ID(parent->Number_of_Cells++)
    , Friction(RANDOM(1))

    , Angle()
    , Speed()
    , Mass(10)

    , Color(color_from_rgb(
        55 + RANDOM(200),
        55 + RANDOM(200),
        55 + RANDOM(200)
    ))

    , Parent(parent)
{ }

void Cell::See()
{
    const float
        X = Offset.X + Parent->Potential.X,
        Y = Offset.Y + Parent->Potential.Y;

    for (int dist = 0; dist < 8; dist++)
    {
        SET_PIXELII(
            X + dist * _COS(Angle),
            Y + dist * _SIN(Angle),
            0xffffff
        );
    }
}


//==================================================================================================================
//                                                 ORGANISM CLASS                                                   
//==================================================================================================================


Organism::Organism(std::size_t id, unsigned char numcells, int x, int y)
    : ID(id)
    , Number_of_Cells(0)
    , Distance_moved(0.0)
{
    Position.X = Starting.X = Potential.X = x;
    Position.Y = Starting.Y = Potential.Y = y;
    Velocity.X = Velocity.Y = 0;

    FOR_LOOP(count, numcells)
    {
        cells.emplace_back(this);
    }

    float
        angle = 0,
        Theta = 360.f / this->cells.size(),
        dist = 15;

    for (Cell &c : cells)
    {
        angle += Theta;
        const auto Xx = Position.X + dist * _COS(angle);
        const auto Yy = Position.Y + dist * _SIN(angle);
        c.Offset.X = Xx - Position.X;
        c.Offset.Y = Yy - Position.Y;                        // rand()%(int)dist;//   // rand()%(int)dist;//
        c.Starting = c.Offset;
    }

    FOR_LOOP(cellcount, numcells)
    {
        FOR_LOOP(edgecount, rand() % numcells)
        {
            // MAKE EDGES CONNECTING THE OTHER CELLS
            if (edgecount != cellcount)
            {
                int cnum = rand() % numcells;
                int cnum2 = rand() % numcells;

                cells[cnum].edges.emplace_back(&cells[cnum], &cells[cnum2], RANDOM(1));
                cells[cnum2].edges.emplace_back(&cells[cnum2], &cells[cnum], RANDOM(1));
            }
        }
    }

    for (Cell &C : cells)
    {
        C.Brain = Net(
            3, /* inputs */
            5, /* hidden */
            2  /* output */
        );
    }
}

Organism* Organism::Copy(Organism *Parent)
{
    *this = *Parent;
    return this;
}

Organism* Organism::Mutate(Organism Parent)
{
    this->Number_of_Cells = Parent.Number_of_Cells;
    this->cells = Parent.cells;

    this->Velocity = 0;
    this->Distance_moved = 0;

    FOR_LOOP(cellcount, this->Number_of_Cells)
    {
        this->cells[cellcount].Offset = this->cells[cellcount].Starting;
        this->cells[cellcount].Velocity = 0;
        this->cells[cellcount].Speed = 0;
        this->cells[cellcount].Force = 0;
        this->cells[cellcount].Angle = 0;
        this->cells[cellcount].Parent = this;

        this->cells[cellcount].Brain.Layers[0].Neurons[0].Value = 0;
        this->cells[cellcount].Brain.Layers[0].Neurons[1].Value = 0;
        this->cells[cellcount].Brain.Layers[0].Neurons[2].Value = 0;

        this->cells[cellcount].Brain.Layers[1].Neurons[0].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[1].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[2].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[3].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[4].Value = 0;

        FOR_LOOP(HNeuron, this->cells[cellcount].Brain.Layers[1].Neurons.size())
        {
            FOR_LOOP(HSynapses, this->cells[cellcount].Brain.Layers[0].Neurons.size())
            {
                this->cells[cellcount].Brain.Layers[1].Neurons[HNeuron].Synapses[HSynapses].Weight += ((RANDOM(2) - 1) / 10);
            }
        }

        this->cells[cellcount].Brain.Layers[2].Neurons[0].Value = 0;
        this->cells[cellcount].Brain.Layers[2].Neurons[1].Value = 0;

        FOR_LOOP(ONeuron, this->cells[cellcount].Brain.Layers[2].Neurons.size())
        {
            FOR_LOOP(OSynapses, this->cells[cellcount].Brain.Layers[1].Neurons.size())
            {
                this->cells[cellcount].Brain.Layers[2].Neurons[ONeuron].Synapses[OSynapses].Weight += ((RANDOM(2) - 1) / 10);
            }
        }
    }

    return this;
}

void Organism::Update(float Time_Step)
{
    const float DELTA_TIME = (SDL_GetTicks() - SCREEN->TIME) / 10.;
    SCREEN->TIME = SDL_GetTicks();

    float Xmove = 0, Ymove = 0;

    for (Cell &Parent : cells)
    {
        Parent.Velocity += Parent.Force / Parent.Mass;  // Change in Velocity equals Acceleration    
        Parent.Offset += Parent.Velocity;               // Change in Position over time equals Velocity   

        Parent.Force.X = 0; Parent.Force.Y = 0;

        Xmove += Parent.Offset.X;
        Ymove += Parent.Offset.Y;
    }

    Xmove /= this->Number_of_Cells;
    Ymove /= this->Number_of_Cells;
    Position.X = Xmove + Potential.X;
    Position.Y = Ymove + Potential.Y;

    Potential.X = Xmove + Starting.X;
    Potential.Y = Ymove + Starting.Y;
    Distance_moved = sqrt(Squared(Position.X - Starting.X) + Squared(Position.Y - Starting.Y));


    for (Cell &Parent : cells)
    {
        Parent.Velocity *= .5f; // APPLY A CRUDE "FRICTION" SO THAT VELOCITY IS LOST OVER TIME

        Parent.See();
        Parent.Brain.Think();

        for (Edge &Child : Parent.edges)
        {
            // For Every Cell Get and Set Information about all Connecting Cells.
            int off = Child.Child_ID;

            Child.Angle = GetAngle(Parent.Offset, cells[off].Offset);

            Child.Displacement = Get_Displacement(cells[off].Offset, Parent.Offset) - Child.RestDistance;
            Child.Distance = Child.Get_Distance(cells[off]);

            constexpr float K = .1f;
            cells[off].Force.X += -K * (Child.Displacement.X);
            cells[off].Force.Y += -K * (Child.Displacement.Y);

            // acceleration = (−SpringStiffness ⁄ mass) * Position 

            cells[off].Brain.Layers[0].Neurons[1].Value = cells[off].Force.X; //Parent.Brain.Layers[2].Neurons[1].Value;
            cells[off].Brain.Layers[0].Neurons[0].Value = cells[off].Force.Y; //.Brain.Layers[2].Neurons[0].Value;
        }

        while (Parent.Angle < 0) Parent.Angle += 360;
        while (Parent.Angle >= 360) Parent.Angle -= 360;
    }

    for (Cell &C : cells)
    {
        C.Speed = C.Brain.Layers[2].Neurons[0].Value * 30;
        C.Angle += (C.Brain.Layers[2].Neurons[1].Value * 5); // rand()%180;//
        while (C.Angle < 0) C.Angle += 360;
        while (C.Angle >= 360) C.Angle -= 360;

        C.Force.X += C.Speed * _COS(C.Angle);
        C.Force.Y += C.Speed * _SIN(C.Angle);
    }
}

void Organism::Draw() const
{
    for(auto & cell : cells)
    {
        for(auto & edge : cell.edges)
        {
            SET_DRAW_COLOR(cell.Color);
            LINE(
                cell.Offset.X + Potential.X,
                cell.Offset.Y + Potential.Y,
                (cell.Offset.X + edge.Child_ptr->Offset.X) / 2 + Potential.X,
                (cell.Offset.Y + edge.Child_ptr->Offset.Y) / 2 + Potential.Y
            );
        }
    }
}

int Collision(Organism *parent, Organism *List [])
{
    return 1;

    //FOR_LOOP(OrganismCount, 10)
    //{
    //    if (List[OrganismCount] != parent)
    //    {
    //        if (sqrt(Squared(List[OrganismCount]->Y - parent->Y) + Squared(List[OrganismCount]->X - parent->X)) < 30)
    //        {
    //            //  Print("Collision"); 
    //            //  Print (OrganismCount);
    //        }
    //    }
    //}

    //return 1;
}






//=============================================================================================================================================================
//                                                      EDGE CLASS                                                                                          
//=============================================================================================================================================================



Edge::Edge(Cell *parent, Cell *other, unsigned char tension)
    : Parent_ptr(parent)
    , Child_ptr(other)
    , Child_ID(other->ID)

    , Displacement()
    , RestDistance(Get_Displacement(other->Offset, parent->Offset))

    , Distance(Get_Distance(*other))
    , Tension(tension)
    , Angle(0)
{ }
