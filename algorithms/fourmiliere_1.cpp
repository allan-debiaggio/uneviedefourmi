#include <iostream>
#include <vector>
#include <string>

// Structure représentant une fourmi
struct Fourmi
{
  int id;
  int position; // index dans le chemin
  bool arrivee;
};

int main()
{
  int nb_fourmis = 5;
  std::vector<std::string> chemin;
  chemin.push_back("Sv");
  chemin.push_back("S1");
  chemin.push_back("S2");
  chemin.push_back("Sd");
  int chemin_length = chemin.size();

  std::vector<Fourmi> fourmis;
  for (int i = 0; i < nb_fourmis; ++i)
  {
    fourmis.push_back({i + 1, 0, false}); // toutes en Sv
  }

  // Occupation des salles (hors Sv et Sd)
  std::vector<int> occupation(chemin_length, 0); // 0 = libre, 1 = occupée
  

  int arrived = 0;
  int step = 0;
  std::cout << "Déplacement des fourmis dans la fourmilière 1 :" << std::endl;
  while (arrived < nb_fourmis)
  {
    ++step;
    // On part de la fin pour éviter les collisions
    for (int i = nb_fourmis - 1; i >= 0; --i)
    {
      if (fourmis[i].arrivee)
        continue;
      int pos = fourmis[i].position;
      if (pos < chemin_length - 1)
      {
        // Vérifier si la salle suivante est libre (sauf Sd)
        if (chemin[pos + 1] == "Sd" || occupation[pos + 1] == 0)
        {
          // Libérer la salle actuelle (sauf Sv)
          if (chemin[pos] != "Sv")
            occupation[pos] = 0;
          // Avancer
          fourmis[i].position++;
          // Occuper la nouvelle salle (sauf Sd)
          if (chemin[fourmis[i].position] != "Sd")
            occupation[fourmis[i].position] = 1;
          // Marquer arrivée
          if (chemin[fourmis[i].position] == "Sd")
            fourmis[i].arrivee = true;
        }
      }
    }
    // Affichage
    std::cout << "\nÉtape " << step << ": ";
    for (int i = 0; i < nb_fourmis; ++i)
    {
      std::cout << "F" << fourmis[i].id << "(" << chemin[fourmis[i].position] << ") ";
    }
    // Compter les fourmis arrivées
    arrived = 0;
    for (int i = 0; i < nb_fourmis; ++i)
    {
      if (fourmis[i].arrivee)
        arrived++;
    }
  }
  std::cout << "\n\nToutes les fourmis sont arrivées en " << step << " étapes." << std::endl;
  return 0;
}