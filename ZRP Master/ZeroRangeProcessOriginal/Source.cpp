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
	filename << "D:/OneDrive - student.uni-augsburg.de/Uni/master/simulation output/random_start/";
	filename << "L" << SimulationParams::LatticeSize << " ";
	filename << "N" << SimulationParams::ParticleCount << " ";
	filename << "k" << SimulationParams::Degree << " ";
	filename << "theta" << SimulationParams::Theta << " ";
	filename << "T10000N";
	filename << ".txt";
	int* source = new int[SimulationParams::LatticeSize * 16];
	FileReader::ReadLattice(filename.str(), source);
	Simulation<4, 4>::Run(source);
}

int main() {

	allSims();

  return 0;
}