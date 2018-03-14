//                                                                                                                                                                                                                                                                                                                                       
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
//=============================================================================================================================================================                                                                                                                                                                                                                                                                                  

#include"cell.h"
#include"window.h"
#include"Vertex2D.h"
#include"world.h"

using namespace std;

//=============================================================================================================================================================
//                                                         CELL CLASS                                                                                
//=============================================================================================================================================================

Cell::Cell() : Number_of_edges(0), Angle(0), Speed(0)
{ }

Cell::~Cell()
{ }

Cell::Cell(Organism *parent)
{
	ID = parent->cells.size();
	Offset.X = Potential.X = RANDOM(300);
	Offset.Y = Potential.Y = RANDOM(300);
	Starting.X = Offset.X;
	Starting.Y = Offset.Y;

	Velocity = 0;
	Force = 0;

	Speed = 0;
	Angle = RANDOM(0);
	Friction = RANDOM(1);

	Color = color_from_rgb((55 + RANDOM(200)),
		(55 + RANDOM(200)),
		(55 + RANDOM(200)));

	Mass = 10; // + (rand()%3);
	Parent = parent;
}

void Cell::See()
{
	const float
		X = Offset.X + Parent->Potential.X,
		Y = Offset.Y + Parent->Potential.Y;

	for (float dist = 0; dist < 8; dist++)
	{
		SET_PIXELII(
			X + dist * _COS(Angle),
			Y + dist * _SIN(Angle),
			color_from_rgb(255, 255, 255)
		);
	}
}


//=============================================================================================================================================================
//                                                         ORGANISM CLASS                                                                              
//=============================================================================================================================================================


Organism::Organism(unsigned char numcells, int x, int y)
	: Distance_moved(0.0)
{
	Position.X = Starting.X = Potential.X = x;
	Position.Y = Starting.Y = Potential.Y = y;
	Velocity.X = Velocity.Y = 0;

	FOR_LOOP(count, numcells)
	{
		cells.emplace_back(this);
	}

	float Xx = X,
		Yy = Y,
		angle = 0,
		Theta = 360.f / this->cells.size(),
		dist = 15;

	for (Cell &c : cells)
	{
		angle += Theta;
		Xx = Position.X + dist * cos(RADIANS(angle));
		Yy = Position.Y + dist * sin(RADIANS(angle));
		c.Offset.X = Xx - Position.X;
		c.Offset.Y = Yy - Position.Y;                        // rand()%(int)dist;//   // rand()%(int)dist;//
		c.Starting = c.Offset;
		c.Number_of_edges = 0;
	}

	FOR_LOOP(cellcount, numcells)
	{   // FOR EACH CELL.....
		FOR_LOOP(edgecount, rand() % numcells)
		{  // MAKE EDGES CONNECTING THE OTHER CELLS
			if (edgecount != cellcount)
			{
				int cnum = rand() % numcells;
				int cnum2 = rand() % numcells;

				cells[cnum].edges.push_back(Edge(&cells[cnum], &cells[cnum2], RANDOM(1)));
				cells[cnum2].edges.push_back(Edge(&cells[cnum2], &cells[cnum], RANDOM(1)));
				cells[cnum].Number_of_edges++;
				cells[cnum2].Number_of_edges++;
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
	this->cells = Parent.cells;

	this->Velocity.X = 0;
	this->Velocity.Y = 0;
	this->Distance_moved = 0;

	FOR_LOOP(cellcount, this->cells.size())
	{
		this->cells[cellcount].Offset = this->cells[cellcount].Starting;
		this->cells[cellcount].Velocity.X = 0;
		this->cells[cellcount].Velocity.Y = 0;
		this->cells[cellcount].Speed = 0;
		this->cells[cellcount].Force.X = 0;
		this->cells[cellcount].Force.Y = 0;
		this->cells[cellcount].Acceleration.X = 0;
		this->cells[cellcount].Angle = 0;
		this->cells[cellcount].Mass = this->cells[cellcount].Mass;
		this->cells[cellcount].Acceleration.Y = 0;
		this->cells[cellcount].Parent = this;

		this->cells[cellcount].Brain.Layers[0] = this->cells[cellcount].Brain.Layers[0];

		this->cells[cellcount].Brain.Layers[0].Neurons[0].Value = 0;
		this->cells[cellcount].Brain.Layers[0].Neurons[1].Value = 0;
		this->cells[cellcount].Brain.Layers[0].Neurons[2].Value = 0;

		this->cells[cellcount].Brain.Layers[1] = this->cells[cellcount].Brain.Layers[1];
		this->cells[cellcount].Brain.Layers[1].Neurons[0].Value = 0;
		this->cells[cellcount].Brain.Layers[1].Neurons[1].Value = 0;
		this->cells[cellcount].Brain.Layers[1].Neurons[2].Value = 0;
		this->cells[cellcount].Brain.Layers[1].Neurons[3].Value = 0;
		this->cells[cellcount].Brain.Layers[1].Neurons[4].Value = 0;

		FOR_LOOP(HNeuron, this->cells[cellcount].Brain.Layers[1].Number_of_Neurons)
		{

			FOR_LOOP(HSynapses, this->cells[cellcount].Brain.Layers[0].Number_of_Neurons)
			{
				this->cells[cellcount].Brain.Layers[1].Neurons[HNeuron].Synapses[HSynapses].Weight += ((RANDOM(2) - 1) / 10);
			}
		}

		this->cells[cellcount].Brain.Layers[2] = this->cells[cellcount].Brain.Layers[2];
		this->cells[cellcount].Brain.Layers[2].Neurons[0].Value = 0;
		this->cells[cellcount].Brain.Layers[2].Neurons[1].Value = 0;

		FOR_LOOP(ONeuron, this->cells[cellcount].Brain.Layers[2].Number_of_Neurons)
		{
			FOR_LOOP(OSynapses, this->cells[cellcount].Brain.Layers[1].Number_of_Neurons)
			{
				this->cells[cellcount].Brain.Layers[2].Neurons[ONeuron].Synapses[OSynapses].Weight += ((RANDOM(2) - 1) / 10);
			}
		}
	}
	return this;
}

void Organism::Update(float Time_Step)
{

	float
		DELTA_TIME = 0,
		DELTA_VELOCITY = 0;

	DELTA_TIME = (SDL_GetTicks() - SCREEN->TIME) / 10;
	SCREEN->TIME = SDL_GetTicks();

	float Xmove = 0, Ymove = 0;

	//Print(DELTA_TIME);
	for (Cell &Parent : cells)
	{ // Cycle Every Cell

		Parent.Acceleration = ((Parent.Force) / Parent.Mass);
		Parent.Velocity += (Parent.Acceleration); // Change in Velocity equals Acceleration    
		Parent.Offset += Parent.Velocity;     // Change in Position over time equals Velocity   

		Parent.Force.X = 0; Parent.Force.Y = 0;


		Xmove += Parent.Offset.X;
		Ymove += Parent.Offset.Y;
	}

	Xmove = Xmove / (this->cells.size());
	Ymove = Ymove / (this->cells.size());
	X = Xmove + Potential.X;
	Y = Ymove + Potential.Y;
	// FILLED_CIRCLE( X,Y, 14);

	Potential.X = Xmove + Starting.X;
	Potential.Y = Ymove + Starting.Y;
	Distance_moved = sqrt(Squared(X - Starting.X) + Squared(Y - Starting.Y));

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------

	for (Cell &Parent : cells)
	{ // Cycle Every Cell
		Parent.Velocity *= .5; // APPLY A CRUDE "FRICTION" SO THAT VELOCITY IS LOST OVER TIME

		Parent.See();
		Parent.Brain.Think();

		for (Edge &Child : Parent.edges)
		{ // For Every Cell Get and Set Information about all Connecting Cells.

			int off = Child.Child_ID;

			Child.Angle = GetAngle(Parent.Offset, cells[off].Offset);

			Child.Displacement = Get_Displacement(cells[off].Offset, Parent.Offset) - Child.RestDistance;
			Child.Distance = Child.Get_Distance(cells[off]);

			float K = .1;
			cells[off].Force.X += -K * (Child.Displacement.X);
			cells[off].Force.Y += -K * (Child.Displacement.Y);


			// acceleration = (−SpringStiffness ⁄ mass) * Position 


			cells[off].Brain.Layers[0].Neurons[1].Value = cells[off].Force.X; //Parent.Brain.Layers[2].Neurons[1].Value;
			cells[off].Brain.Layers[0].Neurons[0].Value = cells[off].Force.Y; //.Brain.Layers[2].Neurons[0].Value;
		}


		if (Parent.Angle < 0) Parent.Angle += 360;
		if (Parent.Angle > 360) Parent.Angle -= 0;

	}

	for (Cell &C : cells)
	{
		C.Speed = C.Brain.Layers[2].Neurons[0].Value * 30;
		C.Angle += (C.Brain.Layers[2].Neurons[1].Value * 5); // rand()%180;//
		C.Force.X += C.Speed * cos(RADIANS(C.Angle));
		C.Force.Y += C.Speed * sin(RADIANS(C.Angle));
	}
}

void Organism::Draw()
{             /// WAIT WHY IS THIS NOT BEING CALLED AT ALL... DID I SHUT IT OFF????

	FOR_LOOP(cellcount, this->cells.size())
	{
		FOR_LOOP(edgecount, cells[cellcount].Number_of_edges)
		{
			int Parent = cells[cellcount].edges[edgecount].Parent_ID;
			int Child = cells[cellcount].edges[edgecount].Child_ID;

			float x1 = cells[cellcount].Offset.X + Potential.X,
				y1 = cells[cellcount].Offset.Y + Potential.Y,

				x2 = (x1 + cells[Child].Offset.X + Potential.X) / 2,
				y2 = (y1 + cells[Child].Offset.Y + Potential.Y) / 2;


			SET_DRAW_COLOR(cells[cellcount].Color);
			// CIRCLE(X, Y,20);
			 //FILLED_CIRCLE(x1,y1,10);   
			LINE(x1, y1, x2, y2);
			// float angle =  GetAngle(cells[cellcount].Offset, cells[Child].Offset / 2), 
			//       dist = cells[cellcount].edges[edgecount].Distance;
			// LINE2(x1,y1,angle, dist  );
		}
	}
}


//void f2(const vector<int> & v)

int Collision(Organism *parent, Organism *List [])
{
	float X = 0,
		Y = 0;
	X = parent->X,
		Y = parent->Y;
	FOR_LOOP(OrganismCount, 10)
	{
		if (List[OrganismCount] != parent)
		{

			// Print ("  ");
			if (sqrt(Squared(List[OrganismCount]->Y - Y) + Squared(List[OrganismCount]->X - parent->X)) < 30)
			{
				//  Print("Collision"); 
				//  Print (OrganismCount);

			}
		}
	}
	return 1;
}






//=============================================================================================================================================================
//                                                      EDGE CLASS                                                                                          
//=============================================================================================================================================================



Edge::Edge(Cell *parent, Cell *other, unsigned char tension)
	: Parent_ID(parent->ID), Child_ID(other->ID), Tension(tension), Angle(0), Parent_ptr(parent), Child_ptr(other)
{
	RestDistance = Get_Displacement(other->Offset, parent->Offset);
	Distance = Get_Distance(*other);
	Displacement = 0;
}
Edge::~Edge()
{ }
Edge::Edge()
{ }
