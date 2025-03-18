#include <array>
#include "Simulation.h"
#include "FileWriter.h"
#include <utility>
#include <math.h>
#include <sstream>
#include "FileReader.h"


void allSims(const int* source = nullptr) {
	constexpr int simCount = 4;

	auto startAt = 0;

	for (int L : {512 * 2}) {
		SimulationParams::LatticeSize = L;
		for (int N : {L * 2, L * 4}) {
			SimulationParams::ParticleCount = N;
			SimulationParams::Runtime = 10000 * N;
			for (double k : {2.5}) {

				SimulationParams::Degree = k;
				for (double theta : { 1 }) {

					if (startAt-- > 0)
						continue;

					SimulationParams::Theta = theta;
					Simulation<simCount, simCount>::Run(source);
				}
			}
		}
	}
}

void runFromSource() {
	std::stringstream filename;
	filename << "D:/OneDrive - student.uni-augsburg.de/Uni/master/simulation output/";
	filename << "L" << SimulationParams::LatticeSize << " ";
	filename << "N" << SimulationParams::ParticleCount << " ";
	filename << "k" << SimulationParams::Degree << " ";
	filename << "theta" << SimulationParams::Theta << " ";
	filename << "gamma" << SimulationParams::Gamma << " ";
	filename << "T10000N";
	filename << ".txt";
	int* source = new int[SimulationParams::LatticeSize * 16];
	FileReader::ReadLattice(filename.str(), source);
	Simulation<4, 4>::Run(source);
}

int main() {
	constexpr int simCount = 2;

	SimulationParams::LatticeSize = 256;
	SimulationParams::ParticleCount = 4 * SimulationParams::LatticeSize;
	SimulationParams::Runtime = 10000 * SimulationParams::ParticleCount;
	SimulationParams::Theta = 1;

	SimulationParams::Degree = -2;
	SimulationParams::Gamma = SimulationParams::Degree + 2;
	//Simulation<simCount, simCount>::Run();

	SimulationParams::Degree = -1;
	SimulationParams::Gamma = SimulationParams::Degree + 2;
	//Simulation<simCount, simCount>::Run();


	SimulationParams::Degree = 0;
	SimulationParams::Gamma = SimulationParams::Degree + 2;
	Simulation<simCount, 8>::Run();


	SimulationParams::Degree = 1;
	SimulationParams::Gamma = SimulationParams::Degree + 2;
	Simulation<simCount, 8>::Run();


	SimulationParams::Degree = -1.5;
	SimulationParams::Gamma = 0;
	Simulation<simCount, 8>::Run();

	SimulationParams::Gamma = SimulationParams::Degree + 2;
	Simulation<simCount, 8>::Run();


	SimulationParams::Gamma = SimulationParams::Degree + 3;
	Simulation<simCount, simCount>::Run();



	//allSims();

  return 0;
}