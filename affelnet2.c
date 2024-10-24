#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lecture.h"
#include "affelnet.h"

/*
Lorand Lenny
Spitz Thomas

gcc -O -Werror -Wall -Wpedantic affelnet.c lecture.c
*/

struct eleves{
    int id;
    int score;
};

struct lycees{
    int id;
    int place;
};
// I - Récupération des données
/**
 * @brief 
 * Cette fonction permet simplement d'extraire les données des différents fichiers csv et de les stocker dans des tableaux.
 * @param eleves tableau représentant les élèves 
 * @param voeux tableau représentant les voeux pour chaque élève
 * @param lycee tableau représentant les lycées et leurs nombres de places
 */
void recuperation(int **eleves,int **voeux,int **lycee){
    FILE *f = debut_lecture("voeux_alexis.csv"); // Fichier à choisir contenant les voeux pour chaque élève, leur id et leur score

    for (int i=0;i<NB_ELEVES;i=i+1){
        eleves[i]=(int *)malloc(2*sizeof(int));
        voeux[i]=(int *)malloc(NB_VOEUX*sizeof(int));
        lecture_eleve_suivant(f,&eleves[i][0] , &eleves[i][1], voeux[i]);
    }
    fin_lecture(f);

    f = debut_lecture("lycees_alexis.csv"); // Fichier à choisir contenant les lycées et leurs nombres de places
    for (int i=0;i<NB_LYCEES;i=i+1){
        lycee[i]=(int *)malloc(2*sizeof(int));
        lecture_lycee_suivant(f, &lycee[i][0], &lycee[i][1]);
}

fin_lecture(f);
}


// II - Classement les élèves
/**
 * @brief 
 * fonction utilisé dans qsort permettant de comparer deux éléments
 * @param a 
 * @param b 
 * @return int 
 */
int comparaison(const void *a, const void *b){
  struct eleves *eleve_1 = (struct eleves *)a;
  struct eleves *eleve_2 = (struct eleves *)b;
  return (eleve_2->score-eleve_1->score);
}

// III - Attribution des lycées
/**
 * @brief 
 * Cette fonction permet d'attribuer à chaque élève si possible, un voeux, dans l'ordre de ses préférences.
 * @param lyc pointeur sur struct lycees contenant chaque lycées et leurs places.
 * @param students pointeur sur struct eleves qui contient chaque numéro et chaque score de chaque élève.
 * @param voeux double pointeur sur entier contenant les voeux de chaque candidat.
 * @return int* (tableau contenant pour chaque élève le lycée qui lui est attribué)
 */
int *attribution(struct lycees *lyc,struct eleves *students, int**voeux)
{
    int *tab = (int *)malloc(NB_ELEVES *sizeof(int));
    for(int i = 0;i<NB_ELEVES;i=i+1)
    {
        int id_eleve = students[i].id;
        int j = 0;
        while(lyc[voeux[id_eleve][j]].place == 0) // On cherche un lycée qui possède une place libre dans les voeux de l'élève
        {
            if(j<NB_VOEUX) 
            {
            j = j+1;
            }
            else // Si on a dépasser tout ses voeux, on sort de la boucle
            {
                break;
            }
        }
    
        if(j==NB_VOEUX) // Si on a dépassé tout ses voeux, on mettra l'élève dans le lycée -1 (aucun lycée pour nous)
        {
            tab[id_eleve] = -1;
        }
        else
        {
            tab[id_eleve] = voeux[id_eleve][j]; // Sinon on lui attribue le lycée correspondant
            lyc[voeux[id_eleve][j]].place = lyc[voeux[id_eleve][j]].place - 1; // Et on retire une place à ce lycée.
        }
    }
    return tab;
}
/**
 * @brief 
 * Cette fonction permet de passer d'un tableau contenant pour chaque élève leur lycée aux élèves associés pour chaque lycée.
 * @param liste_lycee_par_eleve Pointeur sur entier représentant pour chaque élève son lycée
 * @param lyc pointeur sur struct lycees contenant la liste des lycées et leurs places.
 * @param students pointeur sur struct eleves représentant les élèves dans l'ordre
 * @return int** 
 */
int **liste_eleve_par_lycee(int *liste_lycee_par_eleve,struct lycees *lyc, struct eleves *students)
{
    int **tab = (int **)malloc(NB_LYCEES * sizeof(int *));
    for(int i = 0;i<NB_LYCEES;i=i+1) // On attribue de la mémoire à chaque lycée en fonction de son nombre de places
    {
        tab[i] = (int *)malloc(lyc[i].place * sizeof(int));
        for(int j = 0;j<lyc[i].place;j=j+1) // Et on le remplie de -1
        {
            tab[i][j] = -1;
        }
    }
    for(int i = 0;i<NB_ELEVES;i=i+1)
    {
        if(liste_lycee_par_eleve[students[i].id]!=-1) // Si l'élève a un lycée attribué
        {
            int j = 0;
            while(tab[liste_lycee_par_eleve[students[i].id]][j] != -1) // On cherche une case vide dans le tableau du lycée attribué à l'élève
            {
                j = j+1;
            }
            tab[liste_lycee_par_eleve[students[i].id]][j] = students[i].id; // Puis on y met l'élève
        }
    }
    return tab;
}
// IV - Tests des fonctions :

    // comparaison et qsort :
    void testcomparaison(void){
        struct eleves *tableautest = (struct eleves *)malloc(sizeof(struct eleves)*10);
        int scoretest[]={255,896,112,32,789,552,666,370,406,900};
        for(int i = 0;i<10;i=i+1)
        {
            tableautest[i].id = i;
            tableautest[i].score = scoretest[i];
        }
        qsort(tableautest,10,sizeof(struct eleves *),comparaison);
        assert(tableautest[0].id==9);
        assert(tableautest[0].score==900);
        assert(tableautest[3].id==6);
        assert(tableautest[3].score==666);
        assert(tableautest[5].id==8);
        assert(tableautest[5].score==406);
        assert(tableautest[9].id==3);
        assert(tableautest[9].score==32);
    }

    // attribution :
    void testattribution(void){
        // pour ce test, il faut modifier les valeurs de affelnet.h et lecture.h et commenter le main jusqu'au test
        // NB_ELEVES 20 ; NB_LYCEES 4 ; NB_CHOIX 3
        struct lycees *testtableau_lyc = (struct lycees *)malloc(4*sizeof(struct lycees));
        int testlycee_place[]={3,1,5,6};
        for (int i = 0;i<4;i=i+1){
            testtableau_lyc[i].id=i;
            testtableau_lyc[i].place = testlycee_place[i];
        }
        struct eleves *teststudents= (struct eleves *)malloc(20*sizeof(struct eleves));
        int testelevesid[]={9,7,5,14,12,3,6,0,4,17,19,2,8,1,11,10,15,18,13,16}; 
        for (int i = 0;i<20;i=i+1){
            teststudents[i].id=testelevesid[i];
            teststudents[i].score= 0; // pas utilisé dans la fonctions
        
        }
        int **testvoeux =(int **)malloc(20 * sizeof(int *));
        for(int i=0;i<20;i=i+1){
            testvoeux[i]=(int *)malloc(3*sizeof(int));
            for (int j=0;j<3;j=j+1){
                testvoeux[i][j]=(i+j)%4;
            }

        }      
        int *r=attribution(testtableau_lyc,teststudents,testvoeux);
        assert (r[9] == 1);
        assert (r[7] == 3);
        assert (r[5] == 2);
        assert (r[14] == 2);
        assert (r[12] == 0);
        assert (r[3] == 3);
        assert (r[6] == 2);
        assert (r[0] == 0);
        assert (r[4] == 0);
        assert (r[17] == 2);
        assert (r[19] == 3);
        assert (r[2] == 2);
        assert (r[8] == -1);
        assert (r[1] == 3);
        assert (r[11] == 3);
        assert (r[10] == 3);
        assert (r[15] == -1);
        assert (r[18]== -1);
        assert (r[13] == -1);
        assert (r[16]== -1);

    }

    // liste_eleve_par_lycee :
    //pareil que pour le test precedent
    void testliste_eleve_par_lycee(void){
        int r[]={0,3,2,3,0,2,2,3,-1,1,3,3,0,-1,2,-1,-1,2,-1,3};
        struct lycees *testtableau_lyc = (struct lycees *)malloc(4*sizeof(struct lycees));
        int testlycee_place[]={3,1,5,6};
        for (int i = 0;i<4;i=i+1){
                testtableau_lyc[i].id=i;
                testtableau_lyc[i].place = testlycee_place[i];
        }
        struct eleves *teststudents= (struct eleves *)malloc(20*sizeof(struct eleves));
        int testelevesid[]={9,7,5,14,12,3,6,0,4,17,19,2,8,1,11,10,15,18,13,16}; 
        for (int i = 0;i<20;i=i+1){
            teststudents[i].id=testelevesid[i];
            teststudents[i].score= 0; // pas utilisé dans la fonctions
        }
        int **tab=liste_eleve_par_lycee(r,testtableau_lyc,teststudents);
        assert(tab[0][0]==12);
        assert(tab[0][1]==0);
        assert(tab[0][2]==4);
        assert(tab[1][0]==9);
        assert(tab[2][0]==5);
        assert(tab[2][1]==14);
        assert(tab[2][2]==6);
        assert(tab[2][3]==17);
        assert(tab[2][4]==2);
        assert(tab[3][0]==7);
        assert(tab[3][1]==3);
        assert(tab[3][2]==19);
        assert(tab[3][3]==1);
        assert(tab[3][4]==11);
        assert(tab[3][5]==10);
    }    

// V - Main

int main(void){
    int **eleves=(int **)malloc(NB_ELEVES*sizeof(int*));
    int **voeux=(int **)malloc(NB_ELEVES*sizeof(int*));
    int **lycee=(int **)malloc(NB_LYCEES*sizeof(int*));
    recuperation(eleves,voeux,lycee);
    struct eleves *tableau = (struct eleves *)malloc(sizeof(struct eleves)*NB_ELEVES); // On crée le tableau de struct eleves contenant pour chaque élève son id et son score
    for(int i = 0;i<NB_ELEVES;i=i+1)
    {
        tableau[i].id = eleves[i][0];
        tableau[i].score = eleves[i][1];
    }
    qsort(tableau,NB_ELEVES,sizeof(struct eleves *),comparaison); // on trie ce tableau
    struct lycees *tableau_lyc = (struct lycees *)malloc(sizeof(struct lycees)*NB_LYCEES); // On crée le tableau des lyc contenant leur id et leurs places disponibles
    for(int i = 0;i<NB_LYCEES;i=i+1)
    {
        tableau_lyc[i].id = lycee[i][0];
        tableau_lyc[i].place = lycee[i][1];
    }
    int *tab_lyc_par_eleve = attribution(tableau_lyc,tableau,voeux); // On attribue à chaque élève un lycée
    for(int i = 0;i<NB_LYCEES;i=i+1) // Comme on le modifie dans attribution (le nombre de places est modifié), on le réinitialise.
    {
        tableau_lyc[i].id = lycee[i][0];
        tableau_lyc[i].place = lycee[i][1];
    }
    int **tab_final = liste_eleve_par_lycee(tab_lyc_par_eleve,tableau_lyc,tableau); // On crée le tableau contenant pour chaque lycée la liste des élèves attribués.
    for(int i = 0;i<NB_LYCEES;i=i+1) // Boucle permettant l'affichage
    {
        printf("lycee : %d, eleves : ",i);
        for(int j = 0;j<tableau_lyc[i].place;j=j+1)
        {
            if(tab_final[i][j]!=-1) // Pour pas laisser des -1 à la fin de l'affichage si jamais toutes les places d'un lycée ne sont pas attribués.
            {
            printf("%d, ",tab_final[i][j]);
            }
        }
        printf("\n");
    }
    //tests (à decommenter en commentant le reste et en modifiant affelnet.h et lecture.h):
    /*
    testcomparaison();
    testattribution();
    testliste_eleve_par_lycee();
    */
    return 0;

}
