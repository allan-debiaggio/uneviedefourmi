#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Fourmi
{
public:
  int id;
  int position; // index dans le chemin courant
  int chemin;   // 0: Sv->S1->S2->S3 (cul-de-sac), 1: Sv->S1->S4->Sd (valide)
  bool arrivee;
  bool retour; // true si la fourmi revient en arrière
  Fourmi(int id_, int chemin_) : id(id_), position(0), chemin(chemin_), arrivee(false), retour(false) {}
};

class Fourmiliere4
{
public:
  std::vector<std::vector<std::string>> chemins = {
      {"Sv", "S1", "S2", "S3"},
      {"Sv", "S1", "S4", "Sd"}};
  std::vector<std::unique_ptr<Fourmi>> fourmis;
  std::vector<int> occupation; // occupation des salles S1, S2, S3, S4 (0 = libre, 1 = occupée)
  int nb_fourmis;

  Fourmiliere4(int n) : nb_fourmis(n)
  {
    // Répartition : 1 fourmi sur le mauvais chemin, le reste sur le bon
    for (int i = 0; i < n; ++i)
    {
      int ch = (i == 0) ? 0 : 1;
      fourmis.push_back(std::make_unique<Fourmi>(i + 1, ch));
    }
    // Salles : Sv, S1, S2, S3, S4, Sd
    // occupation[0]=S1, [1]=S2, [2]=S3, [3]=S4
    occupation = std::vector<int>(4, 0);
  }

  int salle_index(const std::string &salle)
  {
    if (salle == "S1")
      return 0;
    if (salle == "S2")
      return 1;
    if (salle == "S3")
      return 2;
    if (salle == "S4")
      return 3;
    return -1; // Sv ou Sd
  }

  void simuler()
  {
    int finished = 0;
    int step = 0;
    std::cout << "Déplacement des fourmis dans la fourmilière 4 (retour possible du cul-de-sac) :" << std::endl;
    while (finished < nb_fourmis)
    {
      ++step;
      // On part de la fin pour éviter les collisions
      for (int i = nb_fourmis - 1; i >= 0; --i)
      {
        Fourmi *f = fourmis[i].get();
        if (f->arrivee)
          continue;
        int pos = f->position;
        int ch = f->chemin;
        std::vector<std::string> &chemin = chemins[ch];
        // Si la fourmi est sur le mauvais chemin
        if (ch == 0 && !f->retour)
        {
          if (pos < (int)chemin.size() - 1)
          {
            std::string next = chemin[pos + 1];
            int idx_next = salle_index(next);
            // Si la salle suivante est libre (S3 n'est jamais occupée plus d'une fois)
            if (idx_next == -1 || occupation[idx_next] == 0)
            {
              int idx_cur = salle_index(chemin[pos]);
              if (idx_cur >= 0)
                occupation[idx_cur] = 0;
              f->position++;
              if (salle_index(chemin[f->position]) >= 0)
                occupation[salle_index(chemin[f->position])] = 1;
              // Si on arrive à S3, on commence le retour
              if (chemin[f->position] == "S3")
              {
                f->retour = true;
              }
            }
          }
        }
        else if (ch == 0 && f->retour)
        {
          // Retour en arrière vers Sv
          if (pos > 0)
          {
            std::string prev = chemin[pos - 1];
            int idx_prev = salle_index(prev);
            if (idx_prev == -1 || occupation[idx_prev] == 0)
            {
              int idx_cur = salle_index(chemin[pos]);
              if (idx_cur >= 0)
                occupation[idx_cur] = 0;
              f->position--;
              if (salle_index(chemin[f->position]) >= 0)
                occupation[salle_index(chemin[f->position])] = 1;
              // Si on revient à Sv, on change de chemin
              if (chemin[f->position] == "Sv")
              {
                f->chemin = 1;
                f->position = 0;
                f->retour = false;
              }
            }
          }
        }
        else if (ch == 1)
        {
          // Chemin correct vers Sd
          if (pos < (int)chemin.size() - 1)
          {
            std::string next = chemin[pos + 1];
            int idx_next = salle_index(next);
            if (next == "Sd" || (idx_next >= 0 && occupation[idx_next] == 0))
            {
              int idx_cur = salle_index(chemin[pos]);
              if (idx_cur >= 0)
                occupation[idx_cur] = 0;
              f->position++;
              if (chemin[f->position] != "Sd" && salle_index(chemin[f->position]) >= 0)
                occupation[salle_index(chemin[f->position])] = 1;
              if (chemin[f->position] == "Sd")
                f->arrivee = true;
            }
          }
        }
      }
      // Affichage
      std::cout << "\nÉtape " << step << ": ";
      for (int i = 0; i < nb_fourmis; ++i)
      {
        Fourmi *f = fourmis[i].get();
        std::vector<std::string> &chemin = chemins[f->chemin];
        std::string etat = f->arrivee ? "(arrivée)" : (f->retour ? "(retour)" : "");
        std::cout << "F" << f->id << "(" << chemin[f->position] << ")" << etat << " ";
      }
      // Compter les fourmis terminées (arrivées)
      finished = 0;
      for (int i = 0; i < nb_fourmis; ++i)
      {
        if (fourmis[i]->arrivee)
          finished++;
      }
    }
    std::cout << "\n\nSimulation terminée : toutes les fourmis sont arrivées à Sd." << std::endl;
  }
};

int main()
{
  Fourmiliere4 f4(5);
  f4.simuler();
  return 0;
}