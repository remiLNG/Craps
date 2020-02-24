// Longin Remi 21600664 //

#include "craps.h"
#include <stdio.h>
#include <stdlib.h>


int main(void) {
	srand(time(NULL));
	//monPseudo et mon argent sauvegarde
	char chargerSauvegarde[2] = "";
	//On est en jeu
	jouer = 1;
	//On initialise le num du joueur a 0
	int numeroDuJoueur = 0;

	//Si le fichier de sauvegarde des joueurs existe
	if (isSaveJoueurs()) {
		//On demande si il veut le charger
		//On boucle tant que la reponse est "y" ou "n"
		do {
			printf("Ancienne sauvegarde trouvee, voulez vous la charger y/n ?\n");
			scanf("%s", &chargerSauvegarde);			
		} while (strcmp(chargerSauvegarde, YES) && strcmp(chargerSauvegarde, NO));
	}

	//Si on repond oui, on cree les joueurs depuis la sauvegarde
	if (!strcmp(chargerSauvegarde,YES)) {
		createPlayersFromSave();
	} else {
		//Sinon, on demande combien de joueurs nous devons creer
		printf("Bienvenue dans le jeu du CRAPS\nVeulliez entrer le nombre de joueurs: ");
		scanf("%d", &nombreDeJoueurs);
		//On alloue la memoire et initialise certaines variables
		initialise(nombreDeJoueurs);
		//On creer les joeuurs
		creerJoueurs();
	}

	/* Boucle principale */
	
	do {
		//On affiche les joueurs
		afficherJoueurs();
		//On demande a chaque joueurs leur mise
		choisirMise();		
		//On lance les des (resultat aléatoire)
		randomiseDe(numeroDuJoueur);

		//Si on a un CRAPS (2 ou 12)
		if (isCraps(de[0], de[1])) {
			printf("CRAPS ! le tour s'arrete.\n");
			//Les joueurs ayant vote sur 'PASS' ont perdu
			passersLoose();
			//Si ca vaut 2
			if (twoOrTwelve(de[0], de[1]) == TWO) {
				//Les joueurs ayant vote sur 'DONT PASS' ont gagne
				dontPassWin();				
			}
			else if (twoOrTwelve(de[0], de[1]) == TWELVE) {
				//Sinon, si ca vaut 12, le coup est nul
				printf("Le coup est nul. La manche recommence. rien ne change sur la table\n");				
			}
		} else if(isSeven(de[0], de[1])) {
			//Si on obtient 7, les joueurs ayant mise sur PASS ont gagne
			passersWin();	
			//Don't pass ont perd
			dontPassLoose();
			//On remet les mises et les choix des joueurs à 0
			resetJoueurs();
		}else {
			//debut seconde phase
			int valeurDuPoint = de[0].face + de[1].face;
			printf("La valeur du point est: %d\n", valeurDuPoint);

			int hasPlayed = 0;
			do {  //Deuxieme phase 
			
				//demander augmentation de la mise sur pass
				//demander diminution / suppression sur dont pass
				//if mise == PASS ||
				//mise < anciene mise
				//si annule mise => perdu
				printf("\nVous pouvez modifier votre mise\n");
				choisirMise();

				randomiseDe(numeroDuJoueur);

				//Si on refait le point, les joueurs ayant mise sur:
				//Pass gagne
				//Don't pass perd
				if (de[0].face + de[1].face == valeurDuPoint) {					
					passersWin();
					dontPassLoose();
					hasPlayed = 1;
				}
				else if (isSeven(de[0], de[1])) {
					//Si on fait 7 
					//Pass perd
					//D'ont pass gagne
					passersLoose();
					dontPassWin();
					//C'est au prochain joueur de lancer les dés (sens des aiguilles d'une montre)
					numeroDuJoueur = joueurSuivant(numeroDuJoueur);
					hasPlayed = 1;
				}
				else if (de[0].face + de[1].face != valeurDuPoint || !isSeven(de[0], de[1])) {
					//Si il fait ni "le point" ni 7, on est en suspension
					numeroDeSuspension = numeroDeSuspension + 1;
				}

			} while (!hasPlayed);			
		}

		printf("\n\nTour suivant\n\n");
		resetJoueurs(); //On reset le choix et la mise des joueurs a chaque nouvelle partie
		savePlayersInSave(); //On sauvegarde la partie jouee
		numeroDeSuspension = 0;
	} while (jouer == 1); //tant qu'on est "en jeu"

	return 0;
}
