#ifndef TRIANGLE_H
#define TRIANGLE_H

namespace PhysiK {

	/**
	 * @brief Contain a set of three vertex offset
	 */

	class Triangle
	{
		private:

            unsigned int vertexOffset[3];

		public:

			Triangle(const unsigned int fst = 0 , const unsigned int snd = 0, const unsigned int thr = 0);

			const unsigned int& operator[](const unsigned int offset) const;

			unsigned int& operator[](const unsigned int offset);

	};

}

#endif // TRIANGLE_H
