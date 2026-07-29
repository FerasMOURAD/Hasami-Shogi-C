# ♟️ Hasami Shogi (Version C Simplifiée)

Implémentation en C du jeu **Hasami Shogi** (はさみ将棋), un jeu de stratégie traditionnel japonais qui se joue sur un plateau de $9 \times 9$.

---

## 📌 Fonctionnalités

- **Plateau de $9 \times 9$** : Affichage clair avec repères par lettres (`A–I`) et chiffres (`1–9`).
- **Déplacements orthogonaux & Sauts** : Gestion des déplacements en ligne droite et prise en compte des sauts par-dessus un pion adjacent.
- **Système de capture ("Sandwich")** : Détection et élimination automatique des pions adverses encadrés horizontalement ou verticalement.
- **Saisie interactive** : Prise en main simple au format standard (ex: `H7,F7`).
- **Détection de victoire** : Compteur de pions en temps réel avec arrêt automatique dès qu'un joueur passe à $\le 5$ pions.

---

## 🎮 Règles du Jeu

- **Position initiale** : Chaque joueur dispose de 18 pions répartis sur deux lignes complètes (**Noirs** : Lignes `A` et `B`, **Blancs** : Lignes `H` et `I`).
- **Tours de jeu** : Les Blancs commencent (`0`), suivis des Noirs (`1`).
- **Mouvements** :
  - Les déplacements se font uniquement de manière orthogonale (horizontale ou verticale). Les diagonales sont interdites.
  - Un pion peut parcourir n'importe quel nombre de cases vides en ligne droite.
  - Un pion peut sauter par-dessus un autre pion (ami ou ennemi) situé juste à côté si la case immédiatement derrière est libre.
- **Captures** : Encadrer un ou plusieurs pions ennemis consécutifs entre deux de ses propres pions permet de les capturer et de les retirer du plateau.
- **Condition de victoire** : La partie se termine lorsqu'un joueur possède **5 pions ou moins**.

---

## 🚀 Compilation et Exécution

1.  Compilation : gcc -o HasamiShogi Hasami_Shogi.c -Wall

2. Lancement du jeu : ./HasamiShogi


## 🕹️ Comment Jouer

Entrez les coordonnées de départ et d'arrivée séparées par une virgule.
Format : LettreChiffre,LettreChiffre

Exemple de coup: 
Blancs joue: H7,F7
Coup: H7 -> F7

## 🛠️ Structure du Code
├── Hasami_Shogi.c     # Code source complet du jeu en C
└── README.md          # Documentation du projet



### Fonctions Clés

| Structure / Fonction | Rôle |
| `Position` / `Coup` | Structures modélisant les coordonnées et les mouvements sur le plateau. |
| `initialiserPlateau()` | Place les 36 pions initiaux sur la grille 9x9. |
| `cheminValide()` | Vérifie l'absence d'obstacles et la validité des sauts. |
| `captureDirection()` | Analyse et exécute la prise de pions dans une direction donnée. |
| `testerFinPartie()` | Évalue les compteurs pour déterminer le vainqueur. |



