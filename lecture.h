#ifndef _LECTURE_H
#define _LECTURE_H

#include <stdio.h>

#define NB_VOEUX 10
/** fic: nom d'un fichier suppose au bon format,
 * a lancer avant de lire le contenu d'un fichier */
FILE *debut_lecture(const char *fic);

/** f : valeur recuperee grace a debut_lecture
 * a lancer quand on a fini de lire le contenu d'un fichier */
void fin_lecture(FILE *f);

/** f : valeur recuperee grace a debut_lecture
 * lit les donnees d'une ligne dans un fichier de voeux et les stocke
 * dans les adresses fournies en argument, avec l'hypothese qu'il y
 * a NB_VOEUX voeux
 * renvoie le nombre d'arguments lus, ou EOF si la fin du fichier
 * est atteinte (EOF est defini dans stdio.h) 
 * POUR LE PREMIER RENDU */
int lecture_eleve_suivant(FILE *f, int *eleve, int *score, int *lycees);

/** f : valeur recuperee grace a debut_lecture
 * lit les donnees d'une ligne dans un fichier de lycees et les stocke
 * dans les adresses fournies en argument
 * renvoie le nombre d'arguments lus, ou EOF si la fin du fichier
 * est atteinte (EOF est defini dans stdio.h) */
int lecture_lycee_suivant(FILE *f, int *lycee, int *capacite);


/** f : valeur recuperee grace a debut_lecture
 * lit les donnees d'une ligne dans un fichier de voeux et les stocke
 * dans les adresses fournies en argument, avec l'hypothese qu'il y
 * a NB_VOEUX voeux
 * renvoie le nombre d'arguments lus, ou EOF si la fin du fichier
 * est atteinte (EOF est defini dans stdio.h) 
 * POUR LE DEUXIEME RENDU */
int lecture_eleve_suivant_zones(FILE *f, int *eleve, int *scores, int *lycees);
#endif
