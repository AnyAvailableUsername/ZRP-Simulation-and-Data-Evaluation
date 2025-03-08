#include "FileWriter.h"

void FileWriter::WriteLatticeToFile(
  int* lattices, 
  int linearSimulationRunsCount, 
  int parallelSimulationRunsCount, 
  int latticeSize, 
  std::string filename)
{
	int latticesCount = linearSimulationRunsCount * parallelSimulationRunsCount;

	std::ofstream myFile_Handler;
	myFile_Handler.open(filename, std::ios::app);

	for (int i = 0; i < latticesCount; i++) {

		for (int k = 0; k < latticeSize - 1; k++) {
			myFile_Handler << lattices[k + i * latticeSize] << ",";
		}
		myFile_Handler << lattices[latticeSize - 1 + i * latticeSize] << std::endl;
	}

	myFile_Handler.close();
}