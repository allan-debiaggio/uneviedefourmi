#include "AntColonyBase.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Ant6 : public Ant
{
public:
  int path_index; // index of the chosen path
  Ant6(int id_, int path_idx) : Ant(id_), path_index(path_idx) {}
};

class AntColony6 : public AntColony
{
public:
  std::unordered_map<std::string, int> capacities = {
      {"S1", 8}, {"S2", 4}, {"S3", 2}, {"S4", 4}, {"S5", 2}, {"S6", 4}, {"S7", 2}, {"S8", 5}, {"S9", 1}, {"S10", 1}, {"S11", 1}, {"S12", 1}, {"S13", 4}, {"S14", 2}};
  std::unordered_map<std::string, std::vector<std::string>> adjacency = {
      {"Sv", {"S1"}},
      {"S1", {"S2", "S6"}},
      {"S2", {"S3", "S5"}},
      {"S3", {"S4"}},
      {"S4", {"Sd"}},
      {"S5", {"S4"}},
      {"S6", {"S7", "S8"}},
      {"S7", {"S9", "S10"}},
      {"S8", {"S12", "S11"}},
      {"S9", {"S14"}},
      {"S10", {"S14"}},
      {"S11", {"S13"}},
      {"S12", {"S13"}},
      {"S13", {"Sd"}},
      {"S14", {"Sd"}}};
  std::vector<std::vector<std::string>> paths;
  std::unordered_map<std::string, int> occupancy;

  AntColony6(int n) : AntColony(n)
  {
    // Initialize occupancy
    for (const auto &kv : capacities)
      occupancy[kv.first] = 0;
    // Find all simple paths Sv->Sd
    std::vector<std::string> path;
    std::unordered_set<std::string> visited;
    dfs("Sv", path, visited);
    // Sort paths by increasing length
    std::sort(paths.begin(), paths.end(), [](const auto &a, const auto &b)
              { return a.size() < b.size(); });
    // Distribute ants on paths (equal distribution)
    for (int i = 0; i < n; ++i)
    {
      int idx = i % paths.size();
      ants.push_back(std::make_unique<Ant6>(i + 1, idx));
    }
  }

  void dfs(const std::string &room, std::vector<std::string> &path, std::unordered_set<std::string> &visited)
  {
    path.push_back(room);
    visited.insert(room);
    if (room == "Sd")
    {
      paths.push_back(path);
    }
    else if (adjacency.count(room))
    {
      for (const auto &next : adjacency[room])
      {
        if (!visited.count(next))
        {
          dfs(next, path, visited);
        }
      }
    }
    path.pop_back();
    visited.erase(room);
  }

  void simulate() override
  {
    int arrived_count = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 6 (optimisation, capacités, anti-cycles) :" << std::endl;
    while (arrived_count < num_ants)
    {
      ++step;
      // Process from the end to avoid deadlocks
      for (int i = num_ants - 1; i >= 0; --i)
      {
        Ant6 *ant = static_cast<Ant6 *>(ants[i].get());
        if (ant->arrived)
          continue;
        const auto &path = paths[ant->path_index];
        int pos = ant->position;
        if (pos < (int)path.size() - 1)
        {
          std::string next = path[pos + 1];
          // Check capacity of the next room (except Sd)
          if (next == "Sd" || occupancy[next] < capacities[next])
          {
            // Free the current room (except Sv)
            if (path[pos] != "Sv" && capacities.count(path[pos]))
              occupancy[path[pos]]--;
            // Move forward
            ant->position++;
            // Occupy the new room (except Sd)
            if (path[ant->position] != "Sd" && capacities.count(path[ant->position]))
              occupancy[path[ant->position]]++;
            // Mark as arrived
            if (path[ant->position] == "Sd")
              ant->arrived = true;
          }
        }
      }
      // Display
      std::cout << "\nÉtape " << step << ": ";
      for (int i = 0; i < num_ants; ++i)
      {
        Ant6 *ant = static_cast<Ant6 *>(ants[i].get());
        const auto &path = paths[ant->path_index];
        std::cout << "F" << ant->id << "(" << path[ant->position] << ") ";
      }
      // Count arrived ants
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
  AntColony6 colony(50);
  colony.simulate();
  return 0;
}