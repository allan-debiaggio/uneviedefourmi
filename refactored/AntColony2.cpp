#include "AntColonyBase.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Ant2 : public Ant
{
public:
  int path_index; // 0 for Sv->S1->Sd, 1 for Sv->S2->Sd
  Ant2(int id_, int path_idx) : Ant(id_), path_index(path_idx) {}
};

class AntColony2 : public AntColony
{
public:
  std::vector<std::vector<std::string>> paths = {
      {"Sv", "S1", "Sd"},
      {"Sv", "S2", "Sd"}};
  int occupancy[2] = {0, 0}; // occupancy[0] for S1, occupancy[1] for S2

  AntColony2(int n) : AntColony(n)
  {
    // Balanced distribution of ants on both paths
    for (int i = 0; i < n; ++i)
    {
      int path_idx = (i % 2 == 0) ? 0 : 1;
      ants.push_back(std::make_unique<Ant2>(i + 1, path_idx));
    }
  }

  void move_ant(int i) override
  {
    Ant2 *ant = static_cast<Ant2 *>(ants[i].get());
    if (ant->arrived)
      return;
    int pos = ant->position;
    int path_idx = ant->path_index;
    // Si pas encore à Sd
    if (pos < 2)
    {
      // Vérifier si la salle suivante est libre (Sd toujours libre)
      if (paths[path_idx][pos + 1] == "Sd" || occupancy[path_idx] == 0)
      {
        ant->position++;
        if (paths[path_idx][ant->position] == "Sd")
        {
          ant->arrived = true;
        }
        else
        {
          occupancy[path_idx] = 1; // occuper S1 ou S2
        }
      }
    }
  }

  bool is_finished() override
  {
    for (int i = 0; i < num_ants; ++i)
    {
      if (!ants[i]->arrived)
        return false;
    }
    return true;
  }

  void print_state(int step) override
  {
    std::cout << "\nÉtape " << step << ": ";
    for (int i = 0; i < num_ants; ++i)
    {
      Ant2 *ant = static_cast<Ant2 *>(ants[i].get());
      std::cout << "F" << ant->id << "(" << paths[ant->path_index][ant->position] << ") ";
    }
  }

  void print_summary(int step) override
  {
    std::cout << "\n\nToutes les fourmis sont arrivées en " << step << " étapes." << std::endl;
  }
};

int main()
{
  AntColony2 colony(2);
  colony.simulate();
  return 0;
}