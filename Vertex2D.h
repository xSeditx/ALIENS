#pragma once

#include<iostream>


// HAZURL  Rule of Const reference:=   sizeof(T) <= 8 ? T : const T&

class Vector2D
{

public:
	Vector2D();
	Vector2D(const float& x, const float& y);
	float   X,
		Y;

	Vector2D& add(const Vector2D& other);
	Vector2D& subtract(const Vector2D& other);
	Vector2D& divide(const Vector2D& other);
	Vector2D& multiply(const Vector2D& other);

	friend Vector2D operator +(Vector2D left, const Vector2D& right);
	friend Vector2D operator -(Vector2D left, const Vector2D& right);
	friend Vector2D operator /(Vector2D left, const Vector2D& right);
	friend Vector2D operator *(Vector2D left, const Vector2D& right);

	Vector2D& operator +=(const Vector2D& other);
	Vector2D& operator -=(const Vector2D& other);
	Vector2D& operator *=(const Vector2D& other);
	Vector2D& operator /=(const Vector2D& other);

	bool operator ==(const Vector2D& other);
	bool operator !=(const Vector2D& other);

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vector);


	double Dot(Vector2D A, Vector2D B);
	double Cross(Vector2D A, Vector2D B);



	//_____________________________________________________________________________________________________________________________________
	//_____________________________________________________________________________________________________________________________________

	Vector2D& add(double other); // USED FOR THE MANIPULATION OF BOTH ELEMENTS OF THE VECTOR WITH A SINGLE NUMBER
	Vector2D& subtract(double other); // 
	Vector2D& divide(double other); // USAGE:    VECTOR2D   = float VALUE
	Vector2D& multiply(double other); // RESULT:   VECTOR2D.X = float VALUE
	Vector2D& assign(double other); //           VECTOR2D.Y = float VALUE

	Vector2D& operator =(double right);
	Vector2D& operator +=(double other);
	Vector2D& operator -=(double other);
	Vector2D& operator *=(double other);
	Vector2D& operator /=(double other);


	friend Vector2D operator +(Vector2D left, double right);  //
	friend Vector2D operator -(Vector2D left, double right);  //
	friend Vector2D operator /(Vector2D left, double right);  //
	friend Vector2D operator *(Vector2D left, double right);  //
//_____________________________________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________

};


extern inline double GetAngle(Vector2D A, Vector2D B);

template <typename T>
static inline T Squared(T x) { return x * x; }

extern inline Vector2D Get_Displacement(Vector2D A, Vector2D B);
