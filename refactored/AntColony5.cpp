#include "AntColonyBase.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

class Ant5 : public Ant
{
public:
  int path_index; // 0: Sv->S1->S2->S4->S5->Sd, 1: Sv->S1->S3->S4->S6->Sd
  Ant5(int id_, int path_idx) : Ant(id_), path_index(path_idx) {}
};

class AntColony5 : public AntColony
{
public:
  std::vector<std::vector<std::string>> paths = {
      {"Sv", "S1", "S2", "S4", "S5", "Sd"},
      {"Sv", "S1", "S3", "S4", "S6", "Sd"}};
  std::unordered_map<std::string, int> capacities; // max capacity of each room
  std::unordered_map<std::string, int> occupancy;  // current occupancy of each room

  AntColony5(int n) : AntColony(n)
  {
    // Room capacities
    capacities["S1"] = 2;
    capacities["S4"] = 2;
    capacities["S2"] = 1;
    capacities["S3"] = 1;
    capacities["S5"] = 1;
    capacities["S6"] = 1;
    // The others (Sv, Sd) are unlimited
    // Balanced distribution of ants on both paths
    for (int i = 0; i < n; ++i)
    {
      int path_idx = (i % 2 == 0) ? 0 : 1;
      ants.push_back(std::make_unique<Ant5>(i + 1, path_idx));
    }
    // Initialize occupancy
    for (const auto &room : {"S1", "S2", "S3", "S4", "S5", "S6"})
    {
      occupancy[room] = 0;
    }
  }

  void move_ant(int i) override
  {
    Ant5 *ant = static_cast<Ant5 *>(ants[i].get());
    if (ant->arrived)
      return;
    int pos = ant->position;
    int path_idx = ant->path_index;
    const std::vector<std::string> &path = paths[path_idx];
    if (pos < (int)path.size() - 1)
    {
      std::string next = path[pos + 1];
      // Vérifier capacité de la salle suivante (sauf Sd)
      if (next == "Sd" || occupancy[next] < capacities[next])
      {
        // Libérer la salle actuelle (sauf Sv)
        if (path[pos] != "Sv")
          occupancy[path[pos]]--;
        // Avancer
        ant->position++;
        // Occuper la nouvelle salle (sauf Sd)
        if (path[ant->position] != "Sd")
          occupancy[path[ant->position]]++;
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
      Ant5 *ant = static_cast<Ant5 *>(ants[i].get());
      const std::vector<std::string> &path = paths[ant->path_index];
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
  AntColony5 colony(10);
  colony.simulate();
  return 0;
}