/*

	Code by: Travis Stewart
		
	This was originally created as a simple vector class that I could use in my computer graphics course. 
	It contains a Vector class I used in many assignments and didn't want to remake for each one.
	It supports common vector operations.

*/

#include "Global.h"

#ifndef VECTOR_H
#define VECTOR_H

using namespace std;

class Vector {
public:
	float x, y, z;

	Vector() { x = y = z = 0; }
	Vector(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

	~Vector() {}

	//Overloaded Operators
	void operator += (const Vector& v);
	void operator -= (const Vector& v);
	void operator *= (const Vector& v);
	void operator /= (const Vector& v);

	Vector operator + (const Vector& v);
	Vector operator - (const Vector& v);
	Vector operator * (const Vector& v);
	Vector operator / (const Vector& v);

	void operator += (const float& f);
	void operator -= (const float& f);
	void operator *= (const float& f);
	void operator /= (const float& f);

	Vector operator + (const float& f);
	Vector operator - (const float& f);
	Vector operator * (const float& f);
	Vector operator / (const float& f);

	bool operator == (const Vector& v);
	bool operator < (const Vector& v);
	bool operator > (const Vector& v);
	bool operator <= (const Vector& v);
	bool operator >= (const Vector& v);
	bool operator != (const Vector& v);

	bool operator == (const Vector& v) const;
	bool operator <  (const Vector& v) const;
	bool operator >  (const Vector& v) const;
	bool operator <= (const Vector& v) const;
	bool operator >= (const Vector& v) const;
	bool operator != (const Vector& v) const;

	string info();

	Vector unit();
	float getDistance(Vector& pt);
	Vector getDirectionTo(Vector& v);

};

//Custom Hash function for the Vector class
namespace std
{
	template<> struct hash<Vector>
	{
		uint64_t operator()(Vector const& v) const noexcept
		{
			uint64_t const X(std::hash<float>{}(v.x));
			uint64_t const Y(std::hash<float>{}(v.y));
			uint64_t const Z(std::hash<float>{}(v.z));

			return ((X ^ (Y << 1)) >> 1) ^ (Z << 1);
		}
	};
}


//=========================================================================================================================
//-----------------------------------------------General Vector Functions--------------------------------------------------
//=========================================================================================================================

Vector normal(const Vector& v1, const Vector& v2, const Vector& v3);

float dot(const Vector& v1, const Vector& v2);

Vector cross(const Vector& v1, const Vector& v2);


#endif