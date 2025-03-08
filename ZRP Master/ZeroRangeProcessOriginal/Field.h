#pragma once
#include "LuckyWheel.h"
#include "RandomNumberGenerator.h"
#include <string>
#include <utility>
#include <sstream>
#include <iostream>
#include <fstream>

class Field {
public:
  /**
	 * 
	 */
	enum class Initialisation {
		AtZero,
		RandomUniform,
		FromSource,
	};

private:
	int LatticeSize;
	int ParticleCount;
	double Degree;
	double Theta;
	LuckyWheel* _LuckyWheel;
	RandomNumberGenerator* _RandomNumberGenerator;
	int* Lattice;
	double* RateLookupTable;
	double CompletionTime;
	Initialisation _Initialisation;

	double CurrentTime;
	unsigned long long JumpCounter;

public:
	Field(
		double* rateLookupTable,
		int latticeSize,
		int particleCount,
		double degree,
		double theta,
		double completionTime,
		Initialisation initialisation,
		RandomNumberGenerator* randomNumberGenerator);
	~Field();

	auto IsDone() const																					-> bool;
  auto Reload(int* lattice, const int* source = nullptr)			-> void;
	auto Update()																								-> void;
	auto CountOnes()																						-> std::pair<double, int>;
	auto CountClusters()																				-> std::pair<double, int>;
	auto CountAvgClusterSize()																	-> std::pair<double, double>;
	auto Description() const-> std::string;
	auto Save() const-> void;
};