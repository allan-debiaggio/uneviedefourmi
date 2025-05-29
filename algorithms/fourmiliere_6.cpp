#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

class Fourmi
{
public:
  int id;
  int chemin;   // index du chemin choisi
  int position; // position sur le chemin
  bool arrivee;
  Fourmi(int id_, int chemin_) : id(id_), chemin(chemin_), position(0), arrivee(false) {}
};

class Fourmiliere6
{
public:
  std::unordered_map<std::string, int> capacites = {
      {"S1", 8}, {"S2", 4}, {"S3", 2}, {"S4", 4}, {"S5", 2}, {"S6", 4}, {"S7", 2}, {"S8", 5}, {"S9", 1}, {"S10", 1}, {"S11", 1}, {"S12", 1}, {"S13", 4}, {"S14", 2}};
  std::unordered_map<std::string, std::vector<std::string>> adj = {
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
  int nb_fourmis;
  std::vector<std::vector<std::string>> chemins;
  std::vector<std::unique_ptr<Fourmi>> fourmis;
  std::unordered_map<std::string, int> occupation;

  Fourmiliere6(int n) : nb_fourmis(n)
  {
    // Initialiser occupation
    for (const auto &kv : capacites)
      occupation[kv.first] = 0;
    // Trouver tous les chemins simples Sv->Sd
    std::vector<std::string> path;
    std::unordered_set<std::string> visited;
    dfs("Sv", path, visited);
    // Trier les chemins par longueur croissante
    std::sort(chemins.begin(), chemins.end(), [](const auto &a, const auto &b)
              { return a.size() < b.size(); });
    // Répartir les fourmis sur les chemins (plus de fourmis sur les chemins courts et à forte capacité)
    std::vector<int> fourmis_par_chemin(chemins.size(), 0);
    for (int i = 0; i < n; ++i)
    {
      int idx = i % chemins.size();
      fourmis.push_back(std::make_unique<Fourmi>(i + 1, idx));
      fourmis_par_chemin[idx]++;
    }
  }

  void dfs(const std::string &salle, std::vector<std::string> &path, std::unordered_set<std::string> &visited)
  {
    path.push_back(salle);
    visited.insert(salle);
    if (salle == "Sd")
    {
      chemins.push_back(path);
    }
    else if (adj.count(salle))
    {
      for (const auto &next : adj[salle])
      {
        if (!visited.count(next))
        {
          dfs(next, path, visited);
        }
      }
    }
    path.pop_back();
    visited.erase(salle);
  }

  void simuler()
  {
    int arrived = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 6 (optimisation, capacités, anti-cycles) :" << std::endl;
    while (arrived < nb_fourmis)
    {
      ++step;
      // On part de la fin pour éviter les blocages
      for (int i = nb_fourmis - 1; i >= 0; --i)
      {
        Fourmi *f = fourmis[i].get();
        if (f->arrivee)
          continue;
        const auto &chemin = chemins[f->chemin];
        int pos = f->position;
        if (pos < (int)chemin.size() - 1)
        {
          std::string next = chemin[pos + 1];
          // Vérifier capacité de la salle suivante (sauf Sd)
          if (next == "Sd" || occupation[next] < capacites[next])
          {
            // Libérer la salle actuelle (sauf Sv)
            if (chemin[pos] != "Sv" && capacites.count(chemin[pos]))
              occupation[chemin[pos]]--;
            // Avancer
            f->position++;
            // Occuper la nouvelle salle (sauf Sd)
            if (chemin[f->position] != "Sd" && capacites.count(chemin[f->position]))
              occupation[chemin[f->position]]++;
            // Marquer arrivée
            if (chemin[f->position] == "Sd")
              f->arrivee = true;
          }
        }
      }
      // Affichage
      std::cout << "\nÉtape " << step << ": ";
      for (int i = 0; i < nb_fourmis; ++i)
      {
        Fourmi *f = fourmis[i].get();
        const auto &chemin = chemins[f->chemin];
        std::cout << "F" << f->id << "(" << chemin[f->position] << ") ";
      }
      // Compter les fourmis arrivées
      arrived = 0;
      for (int i = 0; i < nb_fourmis; ++i)
      {
        if (fourmis[i]->arrivee)
          arrived++;
      }
    }
    std::cout << "\n\nToutes les fourmis sont arrivées en " << step << " étapes." << std::endl;
  }
};

int main()
{
  Fourmiliere6 f6(50);
  f6.simuler();
  return 0;
}