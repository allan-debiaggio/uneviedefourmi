# Une vie de Fourmi 🐜
  
## Problématique 📋
  
### Une fourmilière, des fourmis, des graphes et des algorithmes. 📈
  
Créer un programme en C++ qui : 
- Résout le problème de déplacement dans la fourmilière : Toutes les fourmis doivent aller du vestibule au dortoir en respectant la configuration des salles données par les problèmes.
- Représente un graphe d'une fourmilière donnée.
- Affiche les étapes nécessaires au déplacement des fourmis, formaté de la façon suivante :
  
+++ E1 +++  
f1 - Sv - S1  
f2 - Sv - S2  
+++ E2 +++  
f1 - S1 - Sd  
f2 - S2 - Sd  
f3 - Sv - S1  
+++ E3 +++  
f3 - S1 - Sd  
  
### Consignes supplémentaires 📑
  
- Les fourmis sont des nombres entiers naturels (Pas de fourmi à virgule, les pauvres 😱).
- Les fourmis se déplacent toutes à la même vitesse.
- Les salles de la fourmilière ne peuvent accueillir qu'une fourmi à la fois (en dehors du vestibule et dortoir, sauf si spécifié autrement)
- Une fourmi ne peut s'engager dans un tunnel que si la salle de destination est vide (en dehors du dortoir) ou si la fourmi qui l'occupe est en train de partir.
- Les tunnels sont traversés instantanément par les fourmis. C'est une fourmilière à trous de ver. Stylé. 😎
- Les tunnels n'ont pas de limite de fourmis, tant que les salles d'accueil ont la possibilité de les accueillir.
- L'intégralité des fourmis doivent rejoindre le dortoir en un minimum d'étapes.
  
### Problèmes ❔
  
Les problèmes à résoudre seront les suivants :
  
- Fourmilière 0 🐜  
f=2  
S1  
S2  
Sv - S1  
Sv - S2  
S1 - Sd  
S2 - Sd  
  
- Fourmilière 1 🐜  
f=5  
S1  
S2  
Sv - S1  
S1 - S2  
S2 - Sd  

- Fourmilière 2 🐜  
f=5  
S1  
S2  
Sv - S1  
S1 - S2  
S2 - Sd  
Sd - Sv  
  
- Fourmilière 3 🐜  
f=5  
S1  
S2  
S3  
S4  
Sv - S1  
S1 - S2  
S4 - Sd  
S1 - S4  
S2 - S3  
  
- Fourmilière 4 🐜  
f=10  
S1 { 2 }  
S2  
S3  
S4 { 2 }  
S5  
S6  
S3 - S4  
Sv - S1  
S1 - S2  
S2 - S4  
S4 - S5  
S5 - Sd  
S4 - S6  
S6 - Sd  
S1 - S3  
  
- Fourmilière 5 🐜  
f=50  
S1 { 8 }  
S2 { 4 }  
S3 { 2 }  
S4 { 4 }  
S5 { 2 }  
S6 { 4 }  
S7 { 2 }  
S8 { 5 }  
S9  
S10  
S11  
S12  
S13 { 4 }  
S14 { 2 }  
S1 - S2  
S2 - S3  
S3 - S4  
S4 - Sd  
Sv - S1  
S2 - S5  
S5 - S4  
S13 - Sd  
S8 - S12  
S12 - S13  
S6 - S7  
S7 - S9  
S9 - S14  
S14 - Sd  
S7 - S10  
S10 - S14  
S1 - S6  
S6 - S8  
S8 - S11  
S11 - S13  
  
### Aller plus loin 🚀
  
- Créer une petite interface simple pour visualiser le cheminement des fourmis à l'intérieur de la fourmilière.
- Réaliser un benchmark des différents algorithmes connus.
  
## Solutions apportées 💡
  
## Conclusion :bowtie: