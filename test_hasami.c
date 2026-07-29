#include <stdio.h>
#include <assert.h>
#include <string.h>

/* En définissant main comme main_jeu, on neutralise le main() du fichier original */
#define main main_jeu
#include "Hasami_Shogi.c"
#undef main

/* Tests Unitaires */
void test_chaineVersPosition() {
    Position pos;
    
    // Test position valide H7
    assert(chaineVersPosition("H7", &pos) == 1);
    assert(pos.ligne == 7);
    assert(pos.colonne == 6);

    // Test position minuscule a1
    assert(chaineVersPosition("a1", &pos) == 1);
    assert(pos.ligne == 0);
    assert(pos.colonne == 0);

    // Test position invalide Z9
    assert(chaineVersPosition("Z9", &pos) == 0);

    // Test position hors limites A0
    assert(chaineVersPosition("A0", &pos) == 0);

    printf("[OK] Test chaineVersPosition valide !\n");
}

void test_valeurAbsolue() {
    assert(valeurAbsolue(-5) == 5);
    assert(valeurAbsolue(5) == 5);
    assert(valeurAbsolue(0) == 0);
    printf("[OK] Test valeurAbsolue valide !\n");
}

int main() {
    printf("=== EXECUTION DES TESTS UNITAIRES ===\n");
    test_chaineVersPosition();
    test_valeurAbsolue();
    printf("======================================\n");
    printf("Tous les tests ont reussi avec succes !\n");
    return 0;
}