#pragma once
#include "Field.h"
#include <future>
#include <chrono>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

class SimulationParams {
public:
  static int ParticleCount;
  static int LatticeSize;
  static double Theta;
  static double Degree;
  static double Gamma;
  static int Runtime;
  static Field::Initialisation Init;
};

int SimulationParams::ParticleCount = 256;
int SimulationParams::LatticeSize = 256;
double SimulationParams::Theta = 1.0;
double SimulationParams::Degree = 1.0;
double SimulationParams::Gamma = 1.0;
int SimulationParams::Runtime = 10000 * SimulationParams::ParticleCount;
Field::Initialisation SimulationParams::Init = Field::Initialisation::AtZero;

template <
  int LinearSimulationRunsCount,
  int ParallelSimulationRunsCount>
  class Simulation {
  public:

    // returns all lattices: 
    //  [   lattice(lin=1, para=1)|...|lattice(lin=LSRC, para=1)|
    //      lattice(lin=1, para=2)|...|lattice(lin=LSRC, para=2)|
    //      ...
    //      lattice(lin=1, para=PSRC)|...|lattice(lin=LSRC, para=PSRC)   ]
    //static auto Run(double (*rateFunction)(int))                    -> int*;

  private:
    static auto MakeRateLookupTable() -> double*
    {
      auto rateLookupTable = new double[SimulationParams::ParticleCount + 1];
      for (int i = 0; i <= SimulationParams::ParticleCount; i++)
        rateLookupTable[i] = RateGenerator(i);

      return rateLookupTable;
    }

    static auto RunLinear(
      double* rateLookupTable,
      int* lattice,
      int threadNumber,
      const int* source = nullptr) -> void
    {
      auto randomNumberGenerator = new RandomNumberGenerator();
      auto field = Field(
        rateLookupTable,
        SimulationParams::LatticeSize,
        SimulationParams::ParticleCount,
        SimulationParams::Degree,
        SimulationParams::Theta,
        SimulationParams::Gamma,
        (double)(SimulationParams::Runtime),
        SimulationParams::Init,
        randomNumberGenerator);

      auto current = std::vector<int>();

      for (int i = 0; i < LinearSimulationRunsCount; i++)
      {
        field.Reload(lattice + i * SimulationParams::LatticeSize, source + i * SimulationParams::LatticeSize);

        while (!field.IsDone())
          field.Update();

        field.Save();
        current.push_back(0);
        std::cout << "simulation " << threadNumber + i * ParallelSimulationRunsCount << " complete!" << std::endl;
      }
      delete randomNumberGenerator;
    }

  public:
    static auto RateGenerator(int n) -> double
    {
      if (n == 0) return 0;

      if (SimulationParams::Theta == 0.0) // for large n 2^(-n-1) is computationally 0
        return 2;

      //auto w_n = pow(2, -n - 1) + (SimulationParams::Theta / SimulationParams::LatticeSize) * pow(n + 1, SimulationParams::Degree);
      //auto w_n_minus_1 = pow(2, -n) + (SimulationParams::Theta / SimulationParams::LatticeSize) * pow(n, SimulationParams::Degree);
      auto w_n = pow(2, -n - 1) + (SimulationParams::Theta / pow(SimulationParams::LatticeSize, SimulationParams::Gamma)) * pow(n + 1, SimulationParams::Degree);
      auto w_n_minus_1 = pow(2, -n) + (SimulationParams::Theta / pow(SimulationParams::LatticeSize, SimulationParams::Gamma)) * pow(n, SimulationParams::Degree);

      return w_n_minus_1 / w_n;
    }

    static auto printDuration(double time_ms, const std::string& qualifier) -> void {
      std::array<double, 5> factors = { 1.0, 1.0 / 1000, 1.0 / 60000, 1.0 / 3600000, 1.0 / 86400000 };
      std::array<std::string, 5> unit_names = { "ms", "sec", "min", "h", "d" };

      int i = 0;
      for (; i < 5 && time_ms * factors[i] > 1; i++) { }
      i--;
      std::cout
        << "simulation time " << qualifier << " (" << unit_names[i] << "): "
        << static_cast<int>(time_ms * factors[i])
        << "(" << time_ms << "ms)"
        << std::endl;
    }

    static auto Run(const int* source = nullptr) -> void
    {
      auto rateLookupTableNew = MakeRateLookupTable();

      int* lattices = new int[LinearSimulationRunsCount * ParallelSimulationRunsCount * SimulationParams::LatticeSize];

      auto futures = new std::future<void>[ParallelSimulationRunsCount];

      std::cout << "simulation started" << std::endl;
      std::chrono::time_point<std::chrono::system_clock> start, end;
      start = std::chrono::system_clock::now();

      for (int i = 0; i < ParallelSimulationRunsCount; i++)
      {
        int capturedIndex = i;

        futures[i] = std::async(
          RunLinear,
          rateLookupTableNew,
          lattices + capturedIndex * LinearSimulationRunsCount * SimulationParams::LatticeSize,
          capturedIndex,
          source + capturedIndex * LinearSimulationRunsCount * SimulationParams::LatticeSize);
      }

      for (int i = 0; i < ParallelSimulationRunsCount; i++) {
        futures[i].wait();
      }

      end = std::chrono::system_clock::now();

      std::cout << "threads all finished" << std::endl;
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch() - start.time_since_epoch());

      auto avg_time_ms = static_cast<double>(duration.count()) / (LinearSimulationRunsCount * ParallelSimulationRunsCount);

      printDuration(avg_time_ms, "avg");
      printDuration(duration.count(), "tot");

      delete rateLookupTableNew;
      delete lattices;
    }
};