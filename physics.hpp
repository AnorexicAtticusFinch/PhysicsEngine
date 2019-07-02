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

#endif
