#ifndef OBJTYPES_HPP
#define OBJTYPES_HPP

#include "physics.hpp"
#include<math.h>
#include<vector>

namespace Obj
{
    class Sphere:phy::PhysicsObj
        {
            public:
                Sphere()
                {
                    identity = 1;
                }
                float radius;
                float CalcVolume();
        };
    class Cube:phy::PhysicsObj
        {
            public:
                Cube()
                {
                    identity = 2;
                }
                float side;
                float CalcVolume();
        };
}


#endif