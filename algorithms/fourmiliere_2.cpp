#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Fourmi
{
public:
  int id;
  int position; // index du chemin sur lequel elle se trouve
  int chemin;   // 0 pour Sv->S1->Sd, 1 pour Sv->S2->Sd
  bool arrivee;
  Fourmi(int id_, int chemin_) : id(id_), position(0), chemin(chemin_), arrivee(false) {}
};

class Fourmiliere2
{
public:
  std::vector<std::vector<std::string>> chemins = {
      {"Sv", "S1", "Sd"},
      {"Sv", "S2", "Sd"}};
  std::vector<std::unique_ptr<Fourmi>> fourmis;
  int occupation[2] = {0, 0}; // occupation[0] pour S1, occupation[1] pour S2
  int nb_fourmis;

  Fourmiliere2(int n) : nb_fourmis(n)
  {
    // Répartition équilibrée des fourmis sur les deux chemins
    for (int i = 0; i < n; ++i)
    {
      int ch = (i % 2 == 0) ? 0 : 1;
      fourmis.push_back(std::make_unique<Fourmi>(i + 1, ch));
    }
  }

  void simuler()
  {
    int arrived = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 2 (une seule fourmi par salle S1/S2) :" << std::endl;
    while (arrived < nb_fourmis)
    {
      ++step;
      // Libérer les salles S1/S2 (on le fait avant de déplacer)
      occupation[0] = 0;
      occupation[1] = 0;
      // On part de la fin pour éviter les collisions
      for (int i = nb_fourmis - 1; i >= 0; --i)
      {
        Fourmi *f = fourmis[i].get();
        if (f->arrivee)
          continue;
        int pos = f->position;
        int ch = f->chemin;
        // Si pas encore à Sd
        if (pos < 2)
        {
          // Vérifier si la salle suivante est libre (Sd toujours libre)
          if (chemins[ch][pos + 1] == "Sd" || occupation[ch] == 0)
          {
            f->position++;
            if (chemins[ch][f->position] == "Sd")
            {
              f->arrivee = true;
            }
            else
            {
              occupation[ch] = 1; // occuper S1 ou S2
            }
          }
        }
      }
      // Affichage
      std::cout << "\nÉtape " << step << ": ";
      for (int i = 0; i < nb_fourmis; ++i)
      {
        Fourmi *f = fourmis[i].get();
        std::cout << "F" << f->id << "(" << chemins[f->chemin][f->position] << ") ";
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
  Fourmiliere2 f2(2);
  f2.simuler();
  return 0;
}