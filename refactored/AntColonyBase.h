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
  virtual ~AntColony() = default;

  // Main simulation loop (template method)
  virtual void simulate()
  {
    int step = 0;
    while (!is_finished())
    {
      ++step;
      before_step();
      for (int i = num_ants - 1; i >= 0; --i)
      {
        move_ant(i);
      }
      print_state(step);
      after_step();
    }
    print_summary(step);
  }

  // Hooks for derived classes
  virtual void move_ant(int i) = 0;
  virtual bool is_finished() = 0;
  virtual void print_state(int step) {}
  virtual void before_step() {}
  virtual void after_step() {}
  virtual void print_summary(int step)
  {
    std::cout << "\nSimulation finished in " << step << " steps.\n";
  }
};

#endif // ANT_COLONY_BASE_H