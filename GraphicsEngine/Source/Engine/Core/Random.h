#pragma once
#include <ctime>
#include <random>

namespace sr
{
	inline std::mt19937& Generator() {
		static std::random_device r_device;
		static std::mt19937 gen(r_device());
		return gen;
	}

	inline void SetSeed(unsigned int seed = static_cast<unsigned int>(std::time(nullptr))) {
		Generator().seed(seed);
	}

	inline int RandomInt() {
		static std::uniform_int_distribution<> dist;
		return dist(Generator());
	}

	/// <summary>
	/// Get random integer [0, max)
	/// </summary>
	/// <param name="max">exclusive maximum</param>
	/// <returns>Random number between 0 and max - 1</returns>
	inline int RandomInt(int max) {
		std::uniform_int_distribution<> i_dist(0, (max-1));
		return i_dist(Generator());
	}

	/// <summary>
	/// Get random integer [min, max]
	/// </summary>
	/// <param name="min">the minimum value to return</param>
	/// <param name="max">the maximum value to return</param>
	/// <returns>an integer between min and max (inclusive)</returns>
	inline int RandomInt(int min, int max) {
		if (min > max) std::swap(min, max);

		std::uniform_int_distribution<> i_dist(min, (max - 1));
		return i_dist(Generator());
	}

	inline float RandomFloat() {
		static std::uniform_real_distribution f_dist(0.0f, 1.0f);
		return f_dist(Generator());
	}

	inline float RandomFloat(float max) {
		std::uniform_real_distribution f_dist(0.0f, max);
		return f_dist(Generator());
	}

	inline float RandomFloat(float min, float max) {
		if (min > max) std::swap(min, max);

		std::uniform_real_distribution f_dist(min, max);
		return f_dist(Generator());
	}

	inline bool RandomBool() {
		std::bernoulli_distribution b_dist(0.5f);
		return b_dist(Generator());
	}
}