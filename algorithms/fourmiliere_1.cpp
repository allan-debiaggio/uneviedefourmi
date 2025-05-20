#include <iostream>
#include <vector>
#include <string>

// Structure représentant une fourmi
struct Fourmi
{
  int id;
  std::string position;
};

int main()
{
  int nb_fourmis = 5;
  std::vector<Fourmi> fourmis;
  std::vector<std::string> chemin = {"Sv", "S1", "S2", "Sd"};
  int chemin_length = chemin.size();

  // Initialisation des fourmis à la position de départ
  for (int i = 0; i < nb_fourmis; ++i)
  {
    fourmis.push_back({i + 1, chemin[0]});
  }

  std::vector<int> positions(nb_fourmis, 0); // position de chaque fourmi sur le chemin
  int arrived = 0;
  int step = 0;
  std::cout << "Déplacement des fourmis dans la fourmilière 1 :" << std::endl;
  while (arrived < nb_fourmis)
  {
    ++step;
    std::cout << "\nÉtape " << step << ": ";
    for (int i = 0; i < nb_fourmis; ++i)
    {
      if (positions[i] < chemin_length - 1)
      {
        positions[i]++;
        fourmis[i].position = chemin[positions[i]];
      }
    }
    // Affichage de la position de chaque fourmi
    for (int i = 0; i < nb_fourmis; ++i)
    {
      std::cout << "F" << fourmis[i].id << "(" << fourmis[i].position << ") ";
    }
    // Compter les fourmis arrivées
    arrived = 0;
    for (int i = 0; i < nb_fourmis; ++i)
    {
      if (fourmis[i].position == "Sd")
        arrived++;
    }
  }
  std::cout << "\n\nToutes les fourmis sont arrivées en " << step << " étapes." << std::endl;
  return 0;
}