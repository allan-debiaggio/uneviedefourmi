#include "AntColonyBase.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Ant3 : public Ant
{
public:
  Ant3(int id_) : Ant(id_) {}
};

class AntColony3 : public AntColony
{
public:
  std::vector<std::string> path;
  std::vector<int> occupancy; // 0 = free, 1 = occupied (for S1 and S2)

  AntColony3(int n) : AntColony(n)
  {
    path = {"Sv", "S1", "S2", "Sd"};
    occupancy = std::vector<int>(path.size(), 0);
    for (int i = 0; i < n; ++i)
    {
      ants.push_back(std::make_unique<Ant3>(i + 1));
    }
  }

  void simulate() override
  {
    int arrived_count = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 3 (cycle, une seule fourmi par salle S1/S2) :" << std::endl;
    while (arrived_count < num_ants)
    {
      ++step;
      // On part de la fin pour éviter les collisions
      for (int i = num_ants - 1; i >= 0; --i)
      {
        Ant3 *ant = static_cast<Ant3 *>(ants[i].get());
        if (ant->arrived)
          continue;
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
      // Affichage
      std::cout << "\nÉtape " << step << ": ";
      for (int i = 0; i < num_ants; ++i)
      {
        Ant3 *ant = static_cast<Ant3 *>(ants[i].get());
        std::cout << "F" << ant->id << "(" << path[ant->position] << ") ";
      }
      // Compter les fourmis arrivées
      arrived_count = 0;
      for (int i = 0; i < num_ants; ++i)
      {
        if (ants[i]->arrived)
          arrived_count++;
      }
    }
    std::cout << "\n\nToutes les fourmis sont arrivées en " << step << " étapes." << std::endl;
  }
};

int main()
{
  AntColony3 colony(5);
  colony.simulate();
  return 0;
}