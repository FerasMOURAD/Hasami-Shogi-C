/*************************************/
/* Hasami Shogi - Version simplifiée */
/*************************************/

#include <stdio.h>
#include <string.h>

/* Constantes */
#define TAILLE 9
#define VIDE '.'
#define BLANC 'B'  /* Pions blancs */
#define NOIR 'N'   /* Pions noirs */
#define PIONS_MIN_POUR_PERDRE 5

/*
 * Structure Position
 * Représente une position sur le plateau avec ligne (0-8) et colonne (0-8)
 * Évite de gérer deux variables séparées et réduit les risques d'inversion
 */
typedef struct {
    int ligne;   /* 0-8 correspondant aux lettres A-I */
    int colonne; /* 0-8 correspondant aux chiffres 1-9 */
} Position;

/*
 * Structure Coup
 * Contient deux positions: départ et arrivée
 * Permet de modéliser un mouvement complet
 */
typedef struct {
    Position depart;
    Position arrivee;
} Coup;

/* Matrice M représentant le plateau de jeu */
char M[TAILLE][TAILLE];

/* Compteurs de pions pour chaque joueur */
int compteurBlancs = 18;
int compteurNoirs = 18;

/* Joueur courant: 0 = Blancs, 1 = Noirs */
int joueurCourant = 0;

/* Prototypes de fonctions pour éviter les erreurs de compilation */
int chaineVersPosition(const char *chaine, Position *pos);


/*************************************************************************/
/*Action                                                                 */
/*Role: initialisation de la matrice  (du plateau)                       */
/*Description des paramètres                                             */
/*M: Résultat: tableau [1..9][1..9] de chaînes de caractères: Le plateau */
/*************************************************************************/

void initialiserPlateau( char M[TAILLE][TAILLE]){
    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){

            /* Lignes A et B : pions noirs */
            if (i == 0 || i == 1){
                M[i][j] = NOIR;
            } else{
                 /* Lignes H et I : pions blancs */
                if (i == 7 || i == 8){
                    M[i][j] = BLANC;
                } else{
                    /* Autres lignes : cases vides */
                    M[i][j] = VIDE;
                }
            }
        }
    }
}


/**********************************************************************************/
/*Action: Affichage du plateau: Affiche les coordonnées et le contenu du plateau  */
/*Description des paramètres                                                      */
/*M: Donnée/Résultat: tableau [1..9][1..9] de chaînes de caractères: Le plateau   */
/**********************************************************************************/

void affichagePlateau(char M[TAILLE][TAILLE]){
    int i, j;
    /*Affichage des numéros de colonnes*/
    printf("  ");
    for (int j = 0; j < TAILLE; j++){
        printf("%d ", j + 1);
    } printf("\n");
    /*Affichage des lignes avec des lettres de A à I*/
    for ( i = 0; i < TAILLE; i++){
        printf("%c ", 'A' + i);

        for (j = 0; j < TAILLE; j++){
            printf("%c ", M[i][j]);
        }
        printf("\n");
    }
    printf("\nPions Blancs (B): %d\n", compteurBlancs);
    printf("Pions Noirs (N): %d\n", compteurNoirs);
}

/************************************************************************************************************/
/*Fonction:                                                                                                 */
/*Role: Lire et vérifie la saisie du joueur au format "LettreChiffre,LettreChiffre"                         */
/*Description des paramètres                                                                                */
/*coup :Donnée: pointeur vers la structure Coup pour le remplissage de départ et arrivée                    */
/************************************************************************************************************/
int lireCoup(Coup *coup){
    char saisie[100];
    char posDepart[10], posArrivee[10];
    int i, virguleIndex = -1;
    int longueur;

    if (scanf("%s", saisie) != 1){
        return -1; /*exit*/
    }
    longueur = strlen(saisie);

    if (longueur == 0){
        return 0;
    }

    /*on cherche la position de la virgule pour extraire
    les positions de départ et d'arrivée*/
    i = 0;
    while (saisie[i] != '\0'){
        if (saisie [i] == ','){
            virguleIndex = i;
        }
        i++;
    }

    if (virguleIndex == -1 || virguleIndex == 0){
        return 0;
    }

    /*on extrait les positions de départ et d'arrivée
    pour simplifier le travail sur la fonction chaineVersPosition*/
    strncpy (posDepart, saisie, virguleIndex);
    posDepart[virguleIndex] = '\0';

    strcpy (posArrivee, saisie + virguleIndex + 1);

    if (!chaineVersPosition (posDepart, &coup -> depart)){
        return 0;
    }
    if (!chaineVersPosition (posArrivee, &coup -> arrivee)){
        return 0;
    }
    return 1;
}





/**********************************************************************************************/
/*Fonction:                                                                                   */
/*Role: Convertion  d'une chaîne de caractères en Position pour avoir le format attendu       */
/*Description des paramètres                                                                  */
/*chaine: Donnée: de type const char *: chaîne de caractères représentant une position        */
/*pos: Donnée: de type Position *, pointeur vers une structure Position                       */
/**********************************************************************************************/

int chaineVersPosition(const char *chaine, Position *pos) {
    char lettre;
    int chiffre;

    if (strlen(chaine) < 2) {
        return 0;
    }

    lettre = chaine [0];
        if (lettre >= 'a' && lettre <= 'i'){
            lettre = lettre - 'a'+'A';
        }
    chiffre = chaine[1] - '0';

    /* Validation de la lettre (A-I) */
    if (lettre < 'A' || lettre > 'I') {
        return 0;
    }

    /* Validation du chiffre (1-9) */
    if (chiffre < 1 || chiffre > 9) {
        return 0;
    }

    pos->ligne = lettre - 'A';      /* Conversion de la lettre saisie en indices (0-8) */
    pos->colonne = chiffre - 1;     /* Conversion du chiffre saisi en un indice (0-8)  */

    return 1;
}


/*****************************************************************************************************************************/
/*Action                                                                                                                     */
/*Role: Capturer les pions ennemis dans une direction donnee                                                                 */
/*Description des paramètres                                                                                                 */
/*pos: Donnée: type Position: prenant la position d'un pion (ligne et colonne)                                               */
/*dligne, dcolonne: Données: Entiers; indiquant les directions verticales et horizontales du scan des pions adverses         */
/*****************************************************************************************************************************/

void captureDirection (Position pos, int dligne, int dcolonne){
    char pionJoueur, pionEnnemi;
    int verifLigne, verifColonne;
    int nbCaptures = 0;
    int i;

    /* on verifie si le tour actuel est celui du noir ou blanc*/
    if (joueurCourant == 0){
        pionJoueur = BLANC;
        pionEnnemi = NOIR;
    }
    else {
        pionEnnemi = BLANC;
        pionJoueur = NOIR;
    }

    /*on initialise verifLigne et verifColonne a la case voisine du pion joué*/
    verifColonne = pos.colonne + dcolonne;
    verifLigne = pos.ligne + dligne;

    /*on compte les pions ennemis consecutifs a l'aide d'une boucle while*/
    while (verifColonne >=0 && verifColonne < TAILLE && verifLigne>=0 && verifLigne < TAILLE && M[verifLigne][verifColonne] == pionEnnemi){
        nbCaptures ++;
        verifLigne = verifLigne + dligne;
        verifColonne = verifColonne + dcolonne;
    }

    /* on verifie maintenant si apres les pion ennemis consecutifs, il y a effectivement un pion ami a l'aide d'un if*/
    if (nbCaptures >0 && verifLigne >=0 && verifLigne < TAILLE && verifColonne>=0 && verifColonne < TAILLE &&
        M[verifLigne][verifColonne] == pionJoueur) {

            verifLigne = pos.ligne + dligne;
            verifColonne = pos.colonne + dcolonne; /*on les remet dans la case voisine pour remplacer les pions ennemis par du vide*/

            for (i = 0; i<nbCaptures; i++){
                M[verifLigne][verifColonne] = VIDE;

                if (pionEnnemi == BLANC){
                    compteurBlancs --;
                }
                else{
                    compteurNoirs --;
                }

                verifColonne = verifColonne + dcolonne;
                verifLigne = verifLigne + dligne;
            }
            if (pionEnnemi == BLANC){
                printf("Capture de %d pion(s) blanc(s)\n", nbCaptures);
            }
            else {
                printf("Capture de %d pion(s) noir(s)\n", nbCaptures);
            }
        }
}


/***************************************************************/
/*Fonction                                                     */
/*Role: Renvoyer la valeur absolue de l'entier à tester        */
/*Description des paramètres                                   */
/*x: Donnée: Entier: L'entier à vérifier                       */
/***************************************************************/

int valeurAbsolue(int x){
    if (x<0){
        return -x;
    }
    return x;
}

/***************************************************************************************************************************/
/*Fonction                                                                                                                 */
/*Rôle: Vérifie si le chemin est libre entre deux positions et gère le saut par-dessus un pion adjacent (ami ou ennemi)    */
/*Description des paramètres                                                                                               */
/*depart, arrivee: Donnée: de type Position:  coordonnées de départ et d'arrivée d'un pion sur le plateau                  */
/***************************************************************************************************************************/

int cheminValide(Position depart, Position arrivee) {
    int dLigne, dColonne;
    int ligneCourante, colonneCourante;
    int pionsEnChemin = 0;

    /* Calculer la direction */
    if (depart.ligne == arrivee.ligne) {
        /* Mouvement horizontal */
        dLigne = 0;
        if (arrivee.colonne > depart.colonne){
            dColonne = 1;
        }else{
            dColonne = -1;}
    }else if (depart.colonne == arrivee.colonne) {
        /* Mouvement vertical */
        dColonne = 0;
        if (arrivee.ligne > depart.ligne){
            dLigne = 1;
        }else{
            dLigne = -1;}
    } else {
        /* Mouvement diagonal interdit */
        return 0;
    }

    /* Parcourir le chemin */
    ligneCourante = depart.ligne + dLigne;
    colonneCourante = depart.colonne + dColonne;

    while (ligneCourante != arrivee.ligne || colonneCourante != arrivee.colonne) {
        if (M[ligneCourante][colonneCourante] != VIDE) {
            pionsEnChemin++;
        }
        ligneCourante += dLigne;
        colonneCourante += dColonne;
    }

    /* Chemin libre: déplacement normal valide */
    if (pionsEnChemin == 0) {
        return 1;
    }

    /* Saut par-dessus un pion adjacent (ami ou ennemi) */
    /* Distance totale = 2, un pion au milieu */
    if (valeurAbsolue(arrivee.ligne - depart.ligne) + valeurAbsolue(arrivee.colonne - depart.colonne) == 2) {
        int milieuLigne = (depart.ligne + arrivee.ligne) / 2;
        int milieuColonne = (depart.colonne + arrivee.colonne) / 2;

        if (M[milieuLigne][milieuColonne] != VIDE &&
            M[arrivee.ligne][arrivee.colonne] == VIDE) {
            return 1;
        }
    }

    return 0;
}

/************************************************************************************************************************/
/*Fonction                                                                                                              */
/*Rôle:  Valide un coup selon les règles du jeu; retourne 1 si succès, 0 sinon                                          */
/*Description des paramètres                                                                                            */
/*coup : donnée vérifiant si le coup est valide aux règles du jeu (ex. pas de diagonale, pas atterir sur un pion etc...)*/
/************************************************************************************************************************/
int validerCoup (Coup coup){
    char pionJoueur;
    Position depart = coup.depart;
    Position arrivee = coup.arrivee;

    /*on vérifie les limites du plateau et retourne 0 si l'utilisateur met du n'importe quoi*/
    if (depart.ligne<0 || depart.ligne>=TAILLE || depart.colonne <0 || depart.colonne>=TAILLE){
        return 0;
    }
    if (arrivee.ligne <0 || arrivee.ligne>=TAILLE || arrivee.colonne <0 || arrivee.colonne>=TAILLE){
        return 0;
    }

    /* on vérifie que le joueur déplace bien son propre pion*/
    if (joueurCourant == 0){
        pionJoueur = BLANC;
    }
    else {
        pionJoueur= NOIR;
    }

    /* on vérifie que la case de départ contient bien le pion du joueur */
    if (M[depart.ligne][depart.colonne] != pionJoueur){
        return 0;
    }

    /*on vérifie que la case d'arrivée est bien vide*/
    if (M[arrivee.ligne][arrivee.colonne] != VIDE ){
        return 0;
    }
    
    /* on vérifie que le mouvement est strictement horizontal OU vertical. Ceci se traduit par le fait que soit la colonne (resp. ligne) de départ est pareille 
    que la colonne (resp ligne) d'arrivée. Donc on retourne une erreur (i.e. return 0) si la colonne de départ (resp. ligne) ET la colonne d'arrivée (resp. ligne) 
    sont différentes.*/
    if (depart.ligne  != arrivee.ligne && depart.colonne != arrivee.colonne){
        return 0;
    }

    /* on vérifie qu'il y a bien un déplacement*/
    if (depart.ligne == arrivee.ligne && depart.colonne == arrivee.colonne){
        return 0;
    }

    /*on vérifique que le chemin est valide*/
    if (!cheminValide (depart, arrivee)){
        return 0;
    }

    return 1;
}

/***********************************************************************************************************************/
/*Action                                                                                                               */
/*Rôle: vérification et application des captures après un déplacement selon les règles du jeu                          */
/*Description des paramètres                                                                                           */
/*arrivee : donnée du point d'arrivée du pion joueur et son lieu "d'attaque"                                           */
/***********************************************************************************************************************/

void verifierCaptures (Position arrivee){
    /*on vérifie si on a formé un "sandwich" contre les pion ennemis dans les 4 directions*/
    captureDirection (arrivee, -1, 0); /*en Haut*/
    captureDirection (arrivee, 1, 0 ); /*en Bas*/
    captureDirection (arrivee, 0, 1 ); /*Droite*/
    captureDirection (arrivee, 0, -1); /*Gauche*/
}


/**********************************************************************************************************************/
/*Action                                                                                                              */
/*Rôle: Application du déplacement d'un pion                                                                          */
/*Description des paramètres                                                                                          */
/*coup: Donnée des positions de départ et d'arriveée dun pion joueur                                                  */
/**********************************************************************************************************************/
void appliquerDeplacement (Coup coup){
    char pion = M[coup.depart.ligne][coup.depart.colonne];

    M[coup.depart.ligne][coup.depart.colonne] = VIDE;
    M[coup.arrivee.ligne][coup.arrivee.colonne] = pion;

    /*on vérifie s'il y a des captures à faire*/
    verifierCaptures (coup.arrivee);
}


/************************************************************************************************************************/
/*Fonction                                                                                                              */
/*Rôle: vérification s'il y a fin de partie selon les règles du jeu                                                     */
/************************************************************************************************************************/
int testerFinPartie (){
    if (compteurNoirs <= PIONS_MIN_POUR_PERDRE){
        return 1; /*blancs gagnants*/
    }
    if (compteurBlancs <= PIONS_MIN_POUR_PERDRE){
        return 2; /*noirs gagnants*/
    }
    return 0; /* jeu continue*/
}

/* Programme principal*/

int main(){
    Coup coup;
    int resultatSaisie; /*pour dire si la forme d'une saisie est acceptée ou pas (e.g. A1,A7 OK, mais Pizza non)*/
    int resultatPartie;
    int partieEnCours = 1; /*utilisée pour éviter les breaks. Par exemple durant la lecture du coup, si resultatSaisie a pris la valeur
    -1, on veut sortir de toute la boucle au prochain tour et arrêter le jeu. */

    printf("================== HASAMI SHOGI ==================\n");
    printf("Bienvenue dans le jeu. Montrez-nous vos skills ici!\n");
    printf("Veuillez bien respecter le format des saisies, LettreChiffre,LettreChiffre (e.g. H7,F7).\n");
    printf("Le jeu commence. Bon courage!\n\n");

    /* Initialisation du plateau de jeu*/
    initialiserPlateau(M);
    /*Affichage de l'état initial du jeu */
    affichagePlateau(M);

    /*Boucle principale du jeu.*/
    while (partieEnCours){
        if (joueurCourant == 0){
             printf("\nBlancs joue: ");
        }
        else {
            printf("\nNoirs joue: ");
        }

        /*Lecture du coup*/
        resultatSaisie = lireCoup (&coup);

        if (resultatSaisie == -1){
            printf("\nFin de la partie!");
            partieEnCours = 0; /*On arrête le jeu au prochain tour.*/
        }

        else {
            if (resultatSaisie == 0){
                printf("Format invalide. Utilisez le format LettreChiffre,LettreChiffre (sans espaces).\n");
                continue;
            }

            if (!validerCoup(coup)){
                printf("Coup invalide.\n");
                continue;
            }
            
            /*Affichage du coup : on traduit du langage ordi en langage humain. (e.g. ligne de lettre A humains = ligne 0 ordi, colonne 1 humains = colonne 0 ordi)*/
            printf("Coup: %c%d -> %c%d\n",
            'A' + coup.depart.ligne, coup.depart.colonne + 1,
            'A' + coup.arrivee.ligne, coup.arrivee.colonne + 1);

            /*Application du déplacement */
            appliquerDeplacement(coup);

            /*Affichage du plateau après le coup*/
            affichagePlateau(M);

            /*On teste la condition de fin de partie.*/
            resultatPartie = testerFinPartie();
            if(resultatPartie == 1){
                printf("\n *** LES BLANCS ONT GAGNÉ ***\n");
                partieEnCours = 0; /*pour quitter le while*/
            }
            else if (resultatPartie == 2){
                printf("\n *** LES NOIRS ONT GAGNÉ ***\n");
                partieEnCours = 0;
            }

            /*Alternance des tours*/
            if (partieEnCours){
                joueurCourant = 1 - joueurCourant;
            }

        }
    }
    printf("Merci d'avoir joué avec nous ^_^\n"); /* ceci (^_^) n'est qu'un emoji souriant*/
}






