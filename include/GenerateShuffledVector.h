#pragma once
#include <vector>
#include <algorithm>
#include <random>

template <class T>
auto GenerateShuffledVector(const T& MAX_VALUE, const T& MIN_VALUE)
{
	std::mt19937 rng = std::mt19937();
	std::vector<T> arr;

	for (T i = MIN_VALUE; i < MAX_VALUE; i++)
	{
		arr.push_back(i);
	}

	std::shuffle(arr.begin(), arr.end(), rng);

	return arr;
}