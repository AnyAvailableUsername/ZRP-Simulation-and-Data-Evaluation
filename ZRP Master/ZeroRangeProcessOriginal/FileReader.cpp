#include "FileReader.h"


void FileReader::ReadLattice(const std::string& filename, int* lattice) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::string line;
  int index = 0;

  while (std::getline(file, line)) { 
    std::stringstream lineStream(line);

    std::string latticeString;
    std::getline(lineStream, latticeString, ';');
    std::stringstream latticeStream(latticeString);

    std::string valueStr;
    while (std::getline(latticeStream, valueStr, ',')) {
      int value = std::stoi(valueStr); 
      lattice[index++] = value;
    }
  }

  file.close();
}