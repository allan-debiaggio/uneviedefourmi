#include "AntColonyBase.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Ant4 : public Ant
{
public:
  int path_index; // 0: dead-end, 1: valid
  bool returning; // true if the ant is returning
  Ant4(int id_, int path_idx) : Ant(id_), path_index(path_idx), returning(false) {}
};

class AntColony4 : public AntColony
{
public:
  std::vector<std::vector<std::string>> paths = {
      {"Sv", "S1", "S2", "S3"},
      {"Sv", "S1", "S4", "Sd"}};
  std::vector<int> occupancy; // occupancy of S1, S2, S3, S4 (0 = free, 1 = occupied)

  AntColony4(int n) : AntColony(n)
  {
    // Distribution: 1 ant on the dead-end, the rest on the valid path
    for (int i = 0; i < n; ++i)
    {
      int path_idx = (i == 0) ? 0 : 1;
      ants.push_back(std::make_unique<Ant4>(i + 1, path_idx));
    }
    occupancy = std::vector<int>(4, 0);
  }

  int room_index(const std::string &room)
  {
    if (room == "S1")
      return 0;
    if (room == "S2")
      return 1;
    if (room == "S3")
      return 2;
    if (room == "S4")
      return 3;
    return -1; // Sv or Sd
  }

  void move_ant(int i) override
  {
    Ant4 *ant = static_cast<Ant4 *>(ants[i].get());
    if (ant->arrived)
      return;
    int pos = ant->position;
    int path_idx = ant->path_index;
    std::vector<std::string> &path = paths[path_idx];
    // If the ant is on the dead-end path
    if (path_idx == 0 && !ant->returning)
    {
      if (pos < (int)path.size() - 1)
      {
        std::string next = path[pos + 1];
        int idx_next = room_index(next);
        if (idx_next == -1 || occupancy[idx_next] == 0)
        {
          int idx_cur = room_index(path[pos]);
          if (idx_cur >= 0)
            occupancy[idx_cur] = 0;
          ant->position++;
          if (room_index(path[ant->position]) >= 0)
            occupancy[room_index(path[ant->position])] = 1;
          // If arrived at S3, start returning
          if (path[ant->position] == "S3")
          {
            ant->returning = true;
          }
        }
      }
    }
    else if (path_idx == 0 && ant->returning)
    {
      // Returning to Sv
      if (pos > 0)
      {
        std::string prev = path[pos - 1];
        int idx_prev = room_index(prev);
        if (idx_prev == -1 || occupancy[idx_prev] == 0)
        {
          int idx_cur = room_index(path[pos]);
          if (idx_cur >= 0)
            occupancy[idx_cur] = 0;
          ant->position--;
          if (room_index(path[ant->position]) >= 0)
            occupancy[room_index(path[ant->position])] = 1;
          // If back at Sv, switch to valid path
          if (path[ant->position] == "Sv")
          {
            ant->path_index = 1;
            ant->position = 0;
            ant->returning = false;
          }
        }
      }
    }
    else if (path_idx == 1)
    {
      // Valid path to Sd
      if (pos < (int)path.size() - 1)
      {
        std::string next = path[pos + 1];
        int idx_next = room_index(next);
        if (next == "Sd" || (idx_next >= 0 && occupancy[idx_next] == 0))
        {
          int idx_cur = room_index(path[pos]);
          if (idx_cur >= 0)
            occupancy[idx_cur] = 0;
          ant->position++;
          if (path[ant->position] != "Sd" && room_index(path[ant->position]) >= 0)
            occupancy[room_index(path[ant->position])] = 1;
          if (path[ant->position] == "Sd")
            ant->arrived = true;
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
      Ant4 *ant = static_cast<Ant4 *>(ants[i].get());
      std::vector<std::string> &path = paths[ant->path_index];
      std::string state = ant->arrived ? "(arrivée)" : (ant->returning ? "(retour)" : "");
      std::cout << "F" << ant->id << "(" << path[ant->position] << ")" << state << " ";
    }
  }

  void print_summary(int step) override
  {
    std::cout << "\n\nSimulation terminée : toutes les fourmis sont arrivées à Sd." << std::endl;
  }
};

int main()
{
  AntColony4 colony(5);
  colony.simulate();
  return 0;
}