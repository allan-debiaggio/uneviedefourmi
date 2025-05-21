#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class Fourmi
{
public:
  int id;
  int position; // index dans le chemin choisi
  int chemin;   // 0: Sv->S1->S2->S4->S5->Sd, 1: Sv->S1->S3->S4->S6->Sd
  bool arrivee;
  Fourmi(int id_, int chemin_) : id(id_), position(0), chemin(chemin_), arrivee(false) {}
};

class Fourmiliere5
{
public:
  std::vector<std::vector<std::string>> chemins = {
      {"Sv", "S1", "S2", "S4", "S5", "Sd"},
      {"Sv", "S1", "S3", "S4", "S6", "Sd"}};
  std::vector<std::unique_ptr<Fourmi>> fourmis;
  std::unordered_map<std::string, int> capacites;  // capacité max de chaque salle
  std::unordered_map<std::string, int> occupation; // occupation courante de chaque salle
  int nb_fourmis;

  Fourmiliere5(int n) : nb_fourmis(n)
  {
    // Capacités des salles
    capacites["S1"] = 2;
    capacites["S4"] = 2;
    capacites["S2"] = 1;
    capacites["S3"] = 1;
    capacites["S5"] = 1;
    capacites["S6"] = 1;
    // Les autres (Sv, Sd) sont illimitées
    // Répartition équilibrée des fourmis sur les deux chemins
    for (int i = 0; i < n; ++i)
    {
      int ch = (i % 2 == 0) ? 0 : 1;
      fourmis.push_back(std::make_unique<Fourmi>(i + 1, ch));
    }
    // Initialiser occupation
    for (const auto &salle : {"S1", "S2", "S3", "S4", "S5", "S6"})
    {
      occupation[salle] = 0;
    }
  }

  void simuler()
  {
    int arrived = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 5 (capacités, optimisation) :" << std::endl;
    while (arrived < nb_fourmis)
    {
      ++step;
      // On part de la fin pour éviter les blocages
      for (int i = nb_fourmis - 1; i >= 0; --i)
      {
        Fourmi *f = fourmis[i].get();
        if (f->arrivee)
          continue;
        int pos = f->position;
        int ch = f->chemin;
        const std::vector<std::string> &chemin = chemins[ch];
        if (pos < (int)chemin.size() - 1)
        {
          std::string next = chemin[pos + 1];
          // Vérifier capacité de la salle suivante (sauf Sd)
          if (next == "Sd" || occupation[next] < capacites[next])
          {
            // Libérer la salle actuelle (sauf Sv)
            if (chemin[pos] != "Sv")
              occupation[chemin[pos]]--;
            // Avancer
            f->position++;
            // Occuper la nouvelle salle (sauf Sd)
            if (chemin[f->position] != "Sd")
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
        const std::vector<std::string> &chemin = chemins[f->chemin];
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
  Fourmiliere5 f5(10);
  f5.simuler();
  return 0;
}