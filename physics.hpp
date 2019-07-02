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
			float operator *(vec3 const quantity)
			{
				vec3 res;
				res.x = x * quantity.x;
				res.y = y * quantity.y;
				res.z = z * quantity.z;
				float final = res.x + res.y + res.z;
				return final;
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
	};
	
	class PhysicsObj
	{
		public:
			
			float mass;
			vec3 COM; //Coordinate of the centre of mass
			vec3 vel; //Velocity of the centre of mass
			vector<vec3> forces;
			
			//Functions for calculating total force, giving an impulse to this body, updating COM coordinate based on velocity and time
	};
}
float phy::vec3::calcMag()
{
	float a = (x*x)+(y*y)+(z*z);
	return sqrt(a);
}

#endif
