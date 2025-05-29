#ifndef ANT_COLONY_BASE_H
#define ANT_COLONY_BASE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

// Base class for Ant
class Ant
{
public:
  int id;
  int position;
  bool arrived;
  Ant(int id_) : id(id_), position(0), arrived(false) {}
  virtual ~Ant() = default;
};

// Base class for AntColony
class AntColony
{
public:
  int num_ants;
  std::vector<std::unique_ptr<Ant>> ants;
  AntColony(int n) : num_ants(n) {}
  virtual void simulate() = 0;
  virtual ~AntColony() = default;
};

#endif // ANT_COLONY_BASE_H