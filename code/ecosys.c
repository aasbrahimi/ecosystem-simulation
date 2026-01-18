#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

//FINAL

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  liste = animal;
  return liste;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal** liste_animal) {
  //liste_animal = &tete, *liste_animal = tete
  if (x<0 || x>SIZE_X || y<0 || y>SIZE_Y)return;
  Animal * animalNv = creer_animal(x,y,energie);
  *liste_animal = ajouter_en_tete_animal(*liste_animal,animalNv);
}
//tete = ajouter_en_tete_animal(tete,nv);
//ajouter_animal(x,y,energie,&tete) 

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
  if(liste == NULL || *liste == NULL || animal == NULL)return;
  Animal* temp;
  if(animal == *liste){
    temp = *liste;
    *liste = (*liste)->suivant;
    free(temp);
  }
Animal* prec;
Animal* it = *liste;
while(it!=NULL){
  if(it==animal)break;
  prec = it;
  it = it->suivant;
}
if(it==animal){
  prec->suivant = it->suivant;
  free(it);
}

}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
   Animal* it = liste;
   Animal* temp; //Pour stocker l'adresse de l'element a supprimer.
   while(it!=NULL){
    temp = it;
    it=it->suivant; 
    free(temp);
   }
  printf("supression terminé\n");
  return NULL; 
}



/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if(la == NULL) return 0;
  else return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  unsigned int cpt = 0;
  Animal* it = la;
  while(it!=NULL){
    cpt++;
    it = it->suivant;
  }
  return cpt;
}


/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa!=NULL) {
    if(pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa!=NULL) {
    if(pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y){
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    } 
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    Animal* it = la;
    while(it!=NULL){
      if(rand()/(float)RAND_MAX < p_ch_dir){
        it->dir[0] = rand()%3-1;
        it->dir[1] = rand()%3-1;
      }
      it->x = (it->x + it->dir[0] + SIZE_X) % SIZE_X;
      it->y = (it->y + it->dir[1] + SIZE_Y) % SIZE_Y;
      it = it->suivant;
    }
}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce){
  Animal* it = liste_animal?*liste_animal:NULL;
  while(it!=NULL){
    if(rand()/(float)RAND_MAX < p_reproduce){
      ajouter_animal(it->x,it->y,it->energie/2,liste_animal);
      it->energie = it->energie/2;
    }
    it = it->suivant;

  }
}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    Animal* it = liste_proie?*liste_proie:NULL;
    bouger_animaux(it);
    Animal* temp;
    while(it!=NULL){
      it->energie -= 1;
      if(it->energie <= 0){
        temp = it;
        it=it->suivant;
        enlever_animal(liste_proie,temp);
        continue;
      }
      it = it->suivant;
    }
    reproduce(liste_proie,p_reproduce_proie);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    Animal* it = l;
    while(it!=NULL){
      if(it->x == x && it->y == y)return it;
      it = it->suivant;
    }

  return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
   Animal* it = liste_predateur?*liste_predateur:NULL;
    bouger_animaux(it);
    Animal* temp;
    Animal* proie;
    while(it!=NULL){
      it->energie -= 1;
      proie = animal_en_XY(*liste_proie,it->x,it->y);
      if(proie!=NULL){
        it->energie += proie->energie;
        enlever_animal(liste_proie,proie);
      }
      if(it->energie <= 0){
        temp = it;
        it=it->suivant;
        enlever_animal(liste_predateur,temp);
        continue;
      }
      
      it = it->suivant;
    }
    reproduce(liste_predateur,p_reproduce_predateur);
}



/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){

   /*A Completer*/


}

void ecrire_ecosys_bis(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie){
  FILE* f = fopen(nom_fichier,"w");
  
  fprintf(f,"<Proies>\n");
  Animal* it = liste_proie;
  while(it!=NULL){
    fprintf(f,"x=%d y=%d dir=[%d %d] e=%f \n",it->x, it->y, it->dir[0],it->dir[1], it->energie);
    it = it->suivant;
  }
  fprintf(f,"</Proies>\n");

  fprintf(f,"<Predateurs>\n");
  it = liste_predateur;
  while(it!=NULL){
    fprintf(f,"x=%d y=%d dir=[%d %d] e=%f \n",it->x, it->y, it->dir[0],it->dir[1], it->energie);
    it=it->suivant;
  }
  fprintf(f,"</Predateurs>\n");
  
  fclose(f);
}

void ecrire_ecosys(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie){
  FILE* f = fopen(nom_fichier,"w");
  if (f==NULL){
    printf("Erreur");
    return;
  }
  fprintf(f,"<Proies>\n");
  Animal* it = liste_proie;
  while(it!=NULL){
    fprintf(f,"x=%d y=%d dir=[%d %d] e=%f \n",it->x, it->y, it->dir[0],it->dir[1], it->energie);
    it = it->suivant;
  }
  fprintf(f,"</Proies>\n");

  fprintf(f,"<Predateurs>\n");
  it = liste_predateur;
  while(it!=NULL){
    fprintf(f,"x=%d y=%d dir=[%d %d] e=%f \n",it->x, it->y, it->dir[0],it->dir[1], it->energie);
    it = it->suivant;
  }
  fprintf(f,"</Predateurs>\n");
  
  fclose(f);
}



void lire_ecosys(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie){
  FILE* f = fopen(nom_fichier,"r");
  if (f==NULL){
    printf("Erreur");
    return;
  }
  char buffer[256];
  fgets(buffer,256,f);
  assert(strncmp(buffer,"<Proies>",8) == 0);//Pour savoir si buffer a bien pris la premiere ligne
  fgets(buffer,256,f); // a chaque appel de fgets il passe a la ligne suivante
  int x_lu,y_lu, dir_lu[2];
  float energie_lu;
  while(strncmp(buffer,"</Proies>",9) != 0){
    sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f \n",&x_lu,&y_lu,&dir_lu[0],&dir_lu[1],&energie_lu);
    Animal* a = creer_animal(x_lu,y_lu,energie_lu);
    a->dir[0] = dir_lu[0];
    a->dir[1] = dir_lu[1];
    a->suivant = *liste_proie;
    *liste_proie = a;
    fgets(buffer,256,f);
    //printf("%s\n",buffer);
    //printf("%d\n",compte_animal_it(*liste_proie));
  }
  
  fgets(buffer,256,f);
  assert(strncmp(buffer,"<Predateurs>",12) == 0);//Pour savoir si buffer a bien pris la premiere ligne
  fgets(buffer,256,f); // a chaque appel de fgets il passe a la ligne suivante
  while(strncmp(buffer,"</Predateurs>",13) != 0){
    sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f \n",&x_lu,&y_lu,&dir_lu[0],&dir_lu[1],&energie_lu);
    Animal* a = creer_animal(x_lu,y_lu,energie_lu);
    a->dir[0] = dir_lu[0];
    a->dir[1] = dir_lu[1];
    a->suivant = *liste_predateur;
    *liste_predateur = a;
    fgets(buffer,256,f);
    //printf("%s\n",buffer);
    //printf("%d\n",compte_animal_it(*liste_predateur));
  }
  fclose(f);
}

void lire_ecosys_bis(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie){
  FILE* f = fopen(nom_fichier,"r");
  int x,y;
  int dir[2];
  float energie;
  char str[20];
  while(fscanf(f,"%s\n",str) != EOF){
    if(strcmp(str,"<Proies>")== 0){
      while(fscanf(f,"x=%d y=%d dir=[%d %d] e=%f \n",&x,&y,&dir[0],&dir[1],&energie) == 5){
        ajouter_animal(x,y,energie,liste_proie);
        (*liste_proie)->dir[0] = dir[0];
        (*liste_proie)->dir[1] = dir[1];
      }
      fscanf(f,"%s\n",str);
    }
    else if(strcmp(str,"<Predateurs>")== 0){
      while(fscanf(f,"x=%d y=%d dir=[%d %d] e=%f \n",&x,&y,&dir[0],&dir[1],&energie) == 5){
        ajouter_animal(x,y,energie,liste_predateur);
        (*liste_predateur)->dir[0] = dir[0];
        (*liste_predateur)->dir[1] = dir[1];
      }
      fscanf(f,"%s\n",str);
    }
  }

}