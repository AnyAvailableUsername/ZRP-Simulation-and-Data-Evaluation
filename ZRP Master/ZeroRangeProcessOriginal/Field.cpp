#include "Field.h"
#include <iostream>

Field::Field(
  double* rateLookupTable,
  int latticeSize,
  int particleCount,
  double degree,
  double theta,
  double completionTime,
  Initialisation initialisation,
  RandomNumberGenerator *randomNumberGenerator)
{
  CompletionTime = completionTime;
  Lattice = nullptr;
  LatticeSize = latticeSize;
  ParticleCount = particleCount;
  Degree = degree;
  Theta = theta;

  RateLookupTable = rateLookupTable;
  _RandomNumberGenerator = randomNumberGenerator;

  _LuckyWheel = new LuckyWheel(LatticeSize, _RandomNumberGenerator);
  _Initialisation = initialisation;

  JumpCounter = 0;
  CurrentTime = 0;
}

Field::~Field()
{
  delete _LuckyWheel;
}

auto Field::IsDone() const -> bool
{
  return CurrentTime >= CompletionTime;
}


auto Field::Reload(int* lattice, const int* source) -> void
{
  CurrentTime = 0;
  JumpCounter = 0;
  Lattice = lattice;

  for (int i = 0; i < LatticeSize; i++) {
    if (_Initialisation == Initialisation::FromSource)
      Lattice[i] = source[i];
    else 
      Lattice[i] = 0;
  }

  for (int i = 0; i < ParticleCount; i++) {
    if (_Initialisation == Initialisation::AtZero)
      Lattice[0]++;
    else if (_Initialisation == Initialisation::AtZero)
      Lattice[_RandomNumberGenerator->NextIntegerRealisation(LatticeSize)]++;
  }

  _LuckyWheel->Initialize(Lattice, RateLookupTable);
}

auto Field::Update() -> void
{
  double rateSum = _LuckyWheel->RateSum();
  int particleOriginSite = _LuckyWheel->Spin();

  while(Lattice[particleOriginSite] == 0) {
    _LuckyWheel->Initialize(Lattice, RateLookupTable);
    rateSum = _LuckyWheel->RateSum();
    particleOriginSite = _LuckyWheel->Spin();
  }
  CurrentTime += 1 / rateSum;

  JumpCounter += 1;

  int particleTravelDistance = _RandomNumberGenerator->NextIntegerRealisation(1, LatticeSize);
  int particleDestinationSite = (particleOriginSite + particleTravelDistance) % LatticeSize;

  Lattice[particleOriginSite]--;
  Lattice[particleDestinationSite]++;

  _LuckyWheel->Update(Lattice, RateLookupTable, particleOriginSite, particleDestinationSite);
}


auto Field::CountOnes() -> std::pair<double, int> {
  int ones_count = 0;
  for (int i = 0; i < LatticeSize; i++) {
    if (Lattice[i] == 1) {
      ones_count++;
    }
  }
  return std::make_pair(CurrentTime, ones_count);
}

auto Field::CountClusters() -> std::pair<double, int> {
  int clusters_count = 0;
  for (int i = 0; i < LatticeSize; i++) {
    if (Lattice[i] > 1) {
      clusters_count++;
    }
  }
  return std::make_pair(CurrentTime, clusters_count);
}

auto Field::CountAvgClusterSize() -> std::pair<double, double> {
  int cluster_size_sum = 0;
  int clusters_count = 0;
  for (int i = 0; i < LatticeSize; i++) {
    if (Lattice[i] > 1) {
      clusters_count++;
      cluster_size_sum += Lattice[i];
    }
  }
  return std::make_pair(CurrentTime, cluster_size_sum / (double)clusters_count);
}

auto Field::Description() const -> std::string {
  std::stringstream filename;
  filename << "sims/";
  filename << "L" << LatticeSize << " ";
  filename << "N" << ParticleCount << " ";
  filename << "k" << Degree << " ";
  filename << "theta" << Theta << " ";
  filename << "T" << CompletionTime / ParticleCount << "N";
  if (_Initialisation == Initialisation::AtZero)
    filename << "_S0";
  else if(_Initialisation == Initialisation::FromSource)
    filename << "_SSource";
  filename << ".txt";
  return filename.str();
}

auto Field::Save() const -> void {
  std::ofstream fileStream;
  fileStream.open(Description(), std::ios::app);

  for (int i = 0; i < LatticeSize; i++) {
    fileStream << Lattice[i] << ", ";
  }
  fileStream << ";";
  fileStream << JumpCounter;
  fileStream << ";";
  fileStream
    << (_Initialisation == Initialisation::AtZero ? "at zero"
     : (_Initialisation == Initialisation::RandomUniform ? "random uniform"
     : (_Initialisation == Initialisation::FromSource ? "from source" : "ERROR INITIALISATION INVALID")));
  fileStream << std::endl;

  fileStream.close();
}
