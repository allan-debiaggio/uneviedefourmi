#include "AntColonyBase.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Ant1 : public Ant
{
public:
  Ant1(int id_) : Ant(id_) {}
};

class AntColony1 : public AntColony
{
public:
  std::vector<std::string> path;
  std::vector<int> occupancy; // 0 = free, 1 = occupied (for S1 and S2)

  AntColony1(int n) : AntColony(n)
  {
    path = {"Sv", "S1", "S2", "Sd"};
    occupancy = std::vector<int>(path.size(), 0);
    for (int i = 0; i < n; ++i)
    {
      ants.push_back(std::make_unique<Ant1>(i + 1));
    }
  }

  void move_ant(int i) override
  {
    Ant1 *ant = static_cast<Ant1 *>(ants[i].get());
    if (ant->arrived)
      return;
    int pos = ant->position;
    if (pos < (int)path.size() - 1)
    {
      // Vérifier si la salle suivante est libre (Sd toujours libre)
      if (path[pos + 1] == "Sd" || occupancy[pos + 1] == 0)
      {
        // Libérer la salle actuelle (sauf Sv)
        if (path[pos] != "Sv")
          occupancy[pos] = 0;
        // Avancer
        ant->position++;
        // Occuper la nouvelle salle (sauf Sd)
        if (path[ant->position] != "Sd")
          occupancy[ant->position] = 1;
        // Marquer arrivée
        if (path[ant->position] == "Sd")
          ant->arrived = true;
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
      Ant1 *ant = static_cast<Ant1 *>(ants[i].get());
      std::cout << "F" << ant->id << "(" << path[ant->position] << ") ";
    }
  }

  void print_summary(int step) override
  {
    std::cout << "\n\nToutes les fourmis sont arrivées en " << step << " étapes." << std::endl;
  }
};

int main()
{
  AntColony1 colony(5);
  colony.simulate();
  return 0;
}