#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"


//FINAL

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000


/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/

void affichage(Animal* l){
  Animal*it = l;
  int i = 1;
  while(it!=NULL){

    printf("%d ) x = %d, y = %d, energie = %f\n",i,it->x,it->y,it->energie);
    i++;
    it = it->suivant;
  }
}

int main(void) {

 
  Animal* ani = creer_animal(0, 0, 10.0);
  ani->dir[0] = -1;
  ani->dir[1] = -1;
  printf("Avant déplacement: x = %d et y = %d\n",ani->x,ani->y);
  bouger_animaux(ani);
  printf("Après déplacement: x = %d et y = %d\n",ani->x,ani->y);

  printf("\n");

  reproduce(&ani,1.0);
  Animal* it = ani;
  while(it!=NULL){
    printf("x = %d, y = %d, energie = %f\n",it->x,it->y,it->energie);
    it=it->suivant;
  }

  printf("\n");
  int x;
  int y;
  Animal* liste_proie = NULL;
  Animal* liste_predateur = NULL;
    for(int i = 1; i <= 20; i++){
        x = rand()%(SIZE_X + 1);
        y = rand()%(SIZE_Y + 1);
        ajouter_animal(x, y, 100, &liste_proie);
        x = rand()%(SIZE_X + 1);
        y = rand()%(SIZE_Y + 1);
        ajouter_animal(x, y, 100, &liste_predateur);
    }
  
  int monde[SIZE_X][SIZE_Y] = {0};
  int i;
  for(i = 0; i<200 && liste_proie!=NULL && liste_predateur!=NULL; i++){
    //rafraichir_proies(&liste_proie, monde);
    rafraichir_predateurs(&liste_predateur,&liste_proie);
    afficher_ecosys(liste_proie,liste_predateur);
    /*printf("PROIES:\n");
    affichage(liste_proie);
    printf("PREDATEURS:\n");
    affichage(liste_predateur);
    printf("---------------------------------------------------------\n");*/
  }
  printf("Fin de simulation apres %d iteration(s)\n",i);
  
  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_predateur);
  return 0;
}
