#ifndef BODY_H
#define BODY_H
#include "physicobject.h"

namespace PhysiK {

	class Triangle;

	class Body : PhysicObject
	{
		private:

			unsigned int nbTriangle;

			Triangle * triangles;

		public:

			//the constructor miss

			const Triangle * getTriangles();

	};

}

#endif // BODY_H
