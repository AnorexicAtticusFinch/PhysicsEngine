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
			vec3()
			{
				x=0;
				y=0;
				z=0;
			}

			float calcMag(); //Calculates the magnitude
			
			//Operator overloading for +, -, * (Scalar multiplication), / (Same as scalar multiplication)
			vec3 operator +(vec3 const quantity)
			{
				vec3 result;
				result.x = x + quantity.x;
				result.y = y + quantity.y;
				result.z = z + quantity.z;
				return result;
			}	
			vec3 operator *(float const quantity)//Scalar Product
			{
				vec3 res;
				res.x = x * quantity;
				res.y = y * quantity;
				res.z = z * quantity;
				return res; 
			}		
			vec3 operator -(vec3 const quantity)
			{
				vec3 result;
				result.x = x - quantity.x;
				result.y = y - quantity.y;
				result.z = z - quantity.z;
				return result;
			}	
			//Functions for cross and dot product
			float DotProduct(vec3 const object);
			vec3 CrossProduct(vec3 const object);			
	};
	
	class PhysicsObj
	{
		public:
			
			float mass;
			vec3 COM; //Coordinate of the centre of mass
			vec3 vel; //Velocity of the centre of mass
			vector<vec3> forces;
			
			//Functions for calculating total force, giving an impulse to this body, updating COM coordinate based on velocity and time
			vec3 TotalForce();
	};
}
float phy::vec3::calcMag()
{
	float a = (x*x)+(y*y)+(z*z);
	return sqrt(a);
}
float phy::vec3::DotProduct(vec3 const object)
{
	vec3 res;
	res.x = x * object.x;
	res.y = y * object.y;
	res.z = z * object.z;
	float result = res.x + res.y + res.z;
	return result;
}
vec3 phy::vec3::CrossProduct(vec3 const object)
{
	vec3 res;
	res.x = (y*object.z) - (z*object.y);
	res.y = (z*object.x) - (x*object.z);
	res.z = (x*object.y) - (y*object.x);
	return res;
}
vec3 phy::PhysicsObj::TotalForce()
{
	vec3 ResultantForce;
	for(int i=0;i<forces.size();i++)
	{
		ResultantForce.x += forces[i].x;
		ResultantForce.y += forces[i].y;
		ResultantForce.z += forces[i].z;

	}
	return ResultantForce;
}

#endif
