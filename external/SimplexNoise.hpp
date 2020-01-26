/*
 * A speed-improved simplex noise algorithm for 2D, 3D and 4D in Java.
 *
 * Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * Better rank ordering method for 4D by Stefan Gustavson in 2012.
 *
 * This could be speeded up even further, but it's useful as it is.
 *
 * Version 2012-03-09
 *
 * This code was placed in the public domain by its original author,
 * Stefan Gustavson. You may use it as you see fit, but
 * attribution is appreciated.
 *
 * Original link: http://staffwww.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java
 * Adapted to C++ by Unarelith
 *
 */
#ifndef SIMPLEXNOISE_HPP_
#define SIMPLEXNOISE_HPP_

class SimplexNoise {
	public:
		// 2D simplex noise
		static double noise(double xin, double yin);

		// 3D simplex noise
		static double noise(double xin, double yin, double zin);

		// 4D simplex noise
		static double noise(double xin, double yin, double zin, double win);

	private:
		// Inner class to speed upp gradient computations
		// (In Java, array access is a lot slower than member access)
		// FIXME: Is C++ different on that topic?
		struct Grad {
			Grad(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
			Grad(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w) {}

			double x = 0;
			double y = 0;
			double z = 0;
			double w = 0;
		};

		// This method is a *lot* faster than using (int)Math.floor(x)
		static int fastfloor(double x) {
			int xi = (int)x;
			return x<xi ? xi-1 : xi;
		}

		static double dot(Grad g, double x, double y) {
			return g.x*x + g.y*y;
		}

		static double dot(Grad g, double x, double y, double z) {
			return g.x*x + g.y*y + g.z*z;
		}

		static double dot(Grad g, double x, double y, double z, double w) {
			return g.x*x + g.y*y + g.z*z + g.w*w;
		}

		// Initialization function
		static void init();

		static Grad grad3[12];
		static Grad grad4[32];

		static unsigned short p[];

		static short perm[512];
		static short permMod12[512];

		static double F2;
		static double G2;
		static double F3;
		static double G3;
		static double F4;
		static double G4;

		static bool isInitialized;
};

#endif // SIMPLEXNOISE_HPP_
