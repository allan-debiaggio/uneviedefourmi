#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Fourmi
{
public:
  int id;
  int position; // index dans le chemin
  bool arrivee;
  Fourmi(int id_) : id(id_), position(0), arrivee(false) {}
};

class Fourmiliere3
{
public:
  std::vector<std::string> chemin;
  std::vector<std::unique_ptr<Fourmi>> fourmis;
  std::vector<int> occupation; // 0 = libre, 1 = occupée (pour S1 et S2)
  int nb_fourmis;

  Fourmiliere3(int n) : nb_fourmis(n)
  {
    chemin = {"Sv", "S1", "S2", "Sd"};
    occupation = std::vector<int>(chemin.size(), 0);
    for (int i = 0; i < n; ++i)
    {
      fourmis.push_back(std::make_unique<Fourmi>(i + 1));
    }
  }

  void simuler()
  {
    int arrived = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 3 (cycle, une seule fourmi par salle S1/S2) :" << std::endl;
    while (arrived < nb_fourmis)
    {
      ++step;
      // On part de la fin pour éviter les collisions
      for (int i = nb_fourmis - 1; i >= 0; --i)
      {
        Fourmi *f = fourmis[i].get();
        if (f->arrivee)
          continue;
        int pos = f->position;
        if (pos < (int)chemin.size() - 1)
        {
          // Vérifier si la salle suivante est libre (Sd toujours libre)
          if (chemin[pos + 1] == "Sd" || occupation[pos + 1] == 0)
          {
            // Libérer la salle actuelle (sauf Sv)
            if (chemin[pos] != "Sv")
              occupation[pos] = 0;
            // Avancer
            f->position++;
            // Occuper la nouvelle salle (sauf Sd)
            if (chemin[f->position] != "Sd")
              occupation[f->position] = 1;
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
  Fourmiliere3 f3(5);
  f3.simuler();
  return 0;
}