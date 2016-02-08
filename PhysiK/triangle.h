#ifndef TRIANGLE_H
#define TRIANGLE_H

namespace PhysiK {

	/**
	 * @brief Contain a set of three vertex offset
	 */

	class Triangle
	{
		private:

			unsigned int VertexOffset[3];

		public:

			Triangle(const unsigned int fst, const unsigned int snd, const unsigned int thr);

			const unsigned int& operator[](const unsigned int offset) const;

			unsigned int& operator[](const unsigned int offset);

	};

}

#endif // TRIANGLE_H
