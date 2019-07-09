#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <math.h>
#include <vector>

namespace phy
{
	class vec3
	{
		public:
			
			float x;
			float y;
			float z;

			vec3();
			vec3(float, float, float);

			float calcMag(); //Calculates the magnitude of the vector
			void normalize(); //Makes a unit vector

			//Operator overloading
			vec3 operator +(vec3 const);
			vec3 operator -(vec3 const);
			vec3 operator *(float const); //Scalar product
			vec3 operator /(float const); //Scalar division

			void operator +=(vec3 const);
			void operator -=(vec3 const);
			void operator *=(float const); //Scalar product
			void operator /=(float const); //Scalar division

			float dotProduct(vec3 const);
			vec3 crossProduct(vec3 const);		
	};
	
	class PhysicsObj
	{
		public:
			
			float mass;
			int identity; 
			/*
				1 - Sphere
			*/

			vec3 COM; //Coordinate of the centre of mass
			vec3 vel; //Velocity of the centre of mass

			vector<vec3> forces;
			vector<vec3> impulses;

			PhysicsObj(float, int, vec3, vec3);

			void addForce(vec3);
			void removeForce(vec3);
			void addImpulse(vec3);

			vec3 totalForce(); //Runs every frame
			vec3 totalImpulse(); //Runs every frame

			virtual vec3 isColliding(PhysicsObj*) = 0;
			/*
				Returns impulse
				Implemented in child classes (ie the shapes)
				Runs every frame
			*/

			void updateMembers(float); //Updates the velocity, COM coordinates and clears the impulses vector. Runs every frame.
	};
}

phy::vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

float phy::vec3::calcMag()
{
	float a = (x * x) + (y * y) + (z * z);
	return sqrt(a);
}

phy::vec3 operator +(phy::vec3 const quantity)
{
	phy::vec3 result;
	result.x = x + quantity.x;
	result.y = y + quantity.y;
	result.z = z + quantity.z;
	return result;
}

phy::vec3 operator -(phy::vec3 const quantity)
{
	phy::vec3 result;
	result.x = x - quantity.x;
	result.y = y - quantity.y;
	result.z = z - quantity.z;
	return result;
}

phy::vec3 operator *(float const quantity)
{
	phy::vec3 res;
	res.x = x * quantity;
	res.y = y * quantity;
	res.z = z * quantity;
	return res; 
}

phy::vec3 operator /(float const quantity)
{
	phy::vec3 res;
	res.x = x / quantity;
	res.y = y / quantity;
	res.z = z / quantity;
	return res; 
}

float phy::vec3::dotProduct(phy::vec3 const object)
{
	phy::vec3 res;
	res.x = x * object.x;
	res.y = y * object.y;
	res.z = z * object.z;
	float result = res.x + res.y + res.z;
	return result;
}

phy::vec3 phy::vec3::crossProduct(phy::vec3 const object)
{
	phy::vec3 res;
	res.x = (y * object.z) - (z * object.y);
	res.y = (z * object.x) - (x * object.z);
	res.z = (x * object.y) - (y * object.x);
	return res;
}

void phy::PhysicsObj::addForce(phy::vec3 newForce)
{ 
	forces.push_back(newForce);
}

void phy::PhysicsObj::addImpulse(phy::vec3 newImpulse)
{ 
	impulses.push_back(newImpulse); 			  
}

phy::vec3 phy::PhysicsObj::totalForce()
{
	phy::vec3 ResultantForce;
	for (int i = 0; i < forces.size(); i++)
	{
		ResultantForce = ResultantForce + forces[i];

	}

	return ResultantForce;
}

phy::vec3 phy::PhysicsObj::totalImpulse()
{
	phy::vec3 ResultantImpulse;
	for (int i = 0; i < forces.size(); i++)
	{
		ResultantImpulse = ResultantImpulse + impulses[i];
	}

	return ResultantImpulse;
}

phy::vec3 phy::PhysicsObj::isColliding(phy::PhysicsObj *obj) 
{
	bool collision = false;
	phy::vec3 impulse;
	phy::vec3 vDistance = COM - obj->COM; //Distance vector between the COMs of the 2 objects

	if (obj->identity == 1 && identity == 1)
	{
		if (vDistance.CalcMag() <= (radius + obj->radius))
		{
			collision = true;
		}
	}

	if (collision)
	{	
		vDistance.normalize();
	 	phy::vec3 velDiff = vel - obj->vel;

		float impulseMag = 2 * mass * (obj->mass;)
		impulseMag = impulseMag / (mass + obj->mass);
		impulse = impulseMag * velDiff.dotProduct(vDistance) * vDistance;
	}

	return impulse;

}

void phy::PhysicsObj::updateMembers(float timeInterval)
{
	vec3 acc = TotalForce() / mass;
	vel = vel + acc * timeInterval;

	vel = vel + TotalImpulse() / mass;

	COM = COM + vel * timeInterval;
	
	impulses.clear(); //Empties the impulses vector
}

#endif
