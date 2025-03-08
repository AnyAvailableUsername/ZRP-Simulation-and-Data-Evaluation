#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class FileWriter {

public:

	static void WriteLatticeToFile(
		int* lattices,
		int linearSimulationRunsCount,
		int parallelSimulationRunsCount,
		int latticeSize,
		std::string filename);

	template<typename T>
	static void WriteVectorToFile(
		std::vector<T>& vec, 
		std::string filename);
};

template<typename T>
inline void FileWriter::WriteVectorToFile(
	std::vector<T>& vec, 
	std::string filename)
{
	std::ofstream myFile_Handler;
	myFile_Handler.open(filename, std::ios::app);

	for (auto var : vec)
	{
		myFile_Handler << var << ", ";
	}
	myFile_Handler << std::endl;

	myFile_Handler.close();
}
