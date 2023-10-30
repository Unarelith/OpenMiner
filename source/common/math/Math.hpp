/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef MATH_HPP_
#define MATH_HPP_

#include <cmath>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

namespace math {

constexpr double DEG_TO_RAD = M_PI / 180.0;
constexpr float DEG_TO_RADf = (float)DEG_TO_RAD;

// Quick lerp - might fail to return exactly b when t = 1.
// Use only in cases where that doesn't matter.
inline float qlerpf(float a, float b, float t) {
	return a + (b - a) * t;
}

inline double qlerp(double a, double b, double t) {
	return a + (b - a) * t;
}

// Normal lerp - guaranteed to match the endpoints when t = 0 and t = 1
inline float lerpf(float a, float b, float t) {
	return t < 0.5f ? qlerpf(a, b, t) : qlerpf(b, a, (1.f - t));
}

inline double lerp(double a, double b, double t) {
	return t < 0.5 ? qlerp(a, b, t) : qlerp(b, a, (1. - t));
}

// Sin and Cos in degrees
// These versions are more precise when the result is zero or near zero,
// e.g. for straight angles like 90 and 180 they return exact values.
// (By Sterbenz's lemma, 180 - angle and 90 - angle are exact)
inline float sinDegf(float angle) {
	return sinf(((fabsf(angle) > 90.f) ? copysignf(180.f - fabsf(angle), angle) : angle) * DEG_TO_RADf);
}

inline double sinDeg(double angle) {
	return sin(((fabs(angle) > 90.) ? copysign(180. - fabs(angle), angle) : angle) * DEG_TO_RAD);
}

inline float cosDegf(float angle) {
	return (fabsf(angle) > 45.f) ? sinDegf(90.f - fabsf(angle)) : cosf(angle * DEG_TO_RADf);
}

inline double cosDeg(double angle) {
	return (fabs(angle) > 45.) ? sinDeg(90. - fabs(angle)) : cos(angle * DEG_TO_RAD);
}

// Positive integer-only modulo
inline unsigned int pmod(int value, unsigned int m) {
	return (value % m + m) % m;
}

// Positive floating-point modulo
inline float pfmodf(float value, float m) {
	return value < 0 ? std::fmod(value, m) + m : std::fmod(value, m);
}

inline double pfmod(double value, double m) {
	return value < 0 ? std::fmod(value, m) + m : std::fmod(value, m);
}

inline long double pfmodl(long double value, long double m) {
	return value < 0 ? std::fmod(value, m) + m : std::fmod(value, m);
}

inline constexpr float abs(float f) {
	return f < 0.f ? -f : f;
}

inline constexpr double abs(double f) {
	return f < 0. ? -f : f;
}

inline constexpr int abs(int f) {
	return f < 0 ? -f : f;
}

inline constexpr long long abs(long long f) {
	return f < 0LL ? -f : f;
}

} // namespace math

#endif // MATH_HPP_
