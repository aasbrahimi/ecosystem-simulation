#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "ecosys.h"

//FINAL

int main(){
    srand(time(NULL));
    int x,y;
    Animal * liste = NULL;
    for(int i = 1; i <= 40; i++){
        x = rand()%(SIZE_X + 1);
        y = rand()%(SIZE_Y + 1);
        ajouter_animal(x, y, 100, &liste);
        
    }
    int cpt = compte_animal_it(liste);
    printf("Avant suppression, il y a %d animal(aux) dans la liste\n",cpt);
    
    enlever_animal(&liste, liste->suivant->suivant);
    cpt = compte_animal_it(liste);
    printf("Apres suppression, il y a %d animal(aux) dans la liste\n",cpt);
    liste = liberer_liste_animaux(liste); 

    Animal* liste_predateur = NULL;
    for(int i = 1; i <= 20; i++){
        x = rand()%(SIZE_X + 1);
        y = rand()%(SIZE_Y + 1);
        ajouter_animal(x, y, 100, &liste_predateur);
    }
    Animal* liste_proie = NULL;
    for(int i = 1; i <= 20; i++){
        x = rand()%(SIZE_X + 1);
        y = rand()%(SIZE_Y + 1);
        ajouter_animal(x, y, 100, &liste_proie);
    }
    printf("%d",compte_animal_it(liste_proie));
    printf("\nTEST0 \n");
    afficher_ecosys(liste_proie,liste_predateur);
    printf("TEST1 \n");
    ecrire_ecosys("data.txt",liste_predateur,liste_proie);
    printf("TEST2 \n");
    Animal* liste_proie2 = NULL;
    Animal* liste_predateur2 = NULL;
    
    lire_ecosys("data.txt", &liste_predateur2, &liste_proie2);

    afficher_ecosys(liste_proie2,liste_predateur2);

    

    liste_proie = liberer_liste_animaux(liste_proie);
    liste_predateur = liberer_liste_animaux(liste_predateur);
    liste_proie2 = liberer_liste_animaux(liste_proie2);
    liste_predateur2 = liberer_liste_animaux(liste_predateur2);
    

}