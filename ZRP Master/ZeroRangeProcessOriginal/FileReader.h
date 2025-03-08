#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class FileReader {
public:
  static void ReadLattice(const std::string& filename, int* lattice);
};
