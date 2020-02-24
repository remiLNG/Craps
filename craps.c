// Longin Remi 21600664 //

#include "craps.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>


/*PARTIE*/
void initialise(int nbJoueurs) {
	//On alloue notre tableau de joueurs de taille nbJoueurs
	joueurs = (Joueur*)malloc(nbJoueurs * sizeof(Joueur));
	jouer = 1; //Variable jouer est un booleen donc fixe a 1 pour pouvoir boucler et l'utliser dans le main dans la boucle principale
	numeroDeSuspension = 0; //Suspension initialise a 0 (besoin pour seconde phase) 
	nombreDeJoueurs = nbJoueurs; //nombreDeJoueurs est de type int
}

//On initialise les joueurs avec le choix et la mise fixee a 0 ainsi que la somme de depart fixee a 50 euros
void creerJoueurs() {
	char tmp[50];
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		printf("Joueur n%d, Rentrez votre pseudo:\n", i);
		scanf("%s", &tmp);
		//On alloue le char* du joueur pour pouvoir copier son pseudo
		joueurs[i].pseudo = (char*)malloc((strlen(tmp) + 1) * sizeof(char));
		//On copie le pseudo
		strcpy(joueurs[i].pseudo, tmp);
		//On initialise les differentes variables
		joueurs[i].argent = INITIAL_MONEY; //50 euros
		joueurs[i].mise.choice = 0; //il a pas encore fait de choix
		joueurs[i].mise.money = 0; // sa mise est à 0
	}

}

//On affiche le pseudo des joueurs en jeu et leur argent
void afficherJoueurs() {
	printf("Liste des joueurs: \n");
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		printf("%s : %d\n", joueurs[i].pseudo, joueurs[i].argent);
	}
	printf("\n\n");
}


//Gerer les mises en prenant compte de tous les cas possibles (premiere et seconde phase)
void choisirMise() {
	int i = 0;
	//On affiche les differents choix possible selon phase de jeu
	for (i; i < nombreDeJoueurs; i++) {
		//Si on a de l'argent on peut jouer
		if (joueurs[i].argent > 0) {
			printf("\nA votre tour, %s\n", joueurs[i].pseudo);
			//Si le choix du joueur etait Pass et qu'on est pas en suspension (debut Deuxieme phase)
			if (joueurs[i].mise.choice == PASS && numeroDeSuspension == 0) {
				printf("Vous pouvez augmenter votre mise ou ne rien faire (entrez %d (votre ancienne mise) pour ne rien faire) \n", joueurs[i].mise.money);
			}
			//Si le choix du joueur etait Don't Pass et qu'on est pas en suspension (debut Deuxieme phase)
			else if (joueurs[i].mise.choice == DONT_PASS && numeroDeSuspension == 0) {
				printf("Vous pouvez diminuer, retirer ou ne rien faire\n");
				printf("\nEntrez un nombre negatif pour diminuer votre mise\n");
			}
			//Si il n'y a pas de suspension et pas eu encore de choix (premiere phase)
			else if(numeroDeSuspension == 0) {
				printf("Choisissez votre mise, %s\nEcrivez '1' pour PASS\nEcrivez '2' pour Don't pass\n", joueurs[i].pseudo);
				int choice = 0;
				scanf("%d", &choice);
				joueurs[i].mise.choice = choice;
			}
			
			//mise a 0 et hasmised = 0 pour dire qu'on a pas encore mise et hasmised = 1 on a effectue une "action" impactant la mise donc on peut poursuivre le jeu
			int mise = 0, hasmised = 0;
			//On affiche encore les differents choix et on applique selon ce qui a ete choisit
			do {
				//Si on est pas en suspension
				if (numeroDeSuspension == 0) {
					//Choix des joueurs (debut Deuxieme phase)
					//Si les joueurs ont mise donc joue (premiere phase)
					if (joueurs[i].mise.money > 0) {
						printf("Sachant que vous avez deja mise: %d euros \n", joueurs[i].mise.money);
					}

					//On rappelle le choix de la mise pour la deuxieme phase
					printf("Combien voulez vous miser d'argent ?\n");
					scanf("%d", &mise);
					
					//Si le choix du joueur etait sur Don't Pass et qu'il entre la meme mise qu'au debut => pas de changement
					if (mise == joueurs[i].mise.money && joueurs[i].mise.choice == DONT_PASS) {
						printf("Votre mise n'a pas ete changee\n");
						hasmised = 1;
					} 
					
					//Si le joueur a mise sur "Pass", il peut augmenter sa mise			
					if (mise < 0 && joueurs[i].mise.choice == PASS) {
						printf("Vous pouvez uniquement augmenter votre mise ou ne rien faire\n");
						
					}else if (mise <= 0 && joueurs[i].mise.money == 0) {
						//Cas ou on verifie toujours que la mise doit etre > 0 
						printf("Votre mise doit être superieure a 0 euros\n");
					} //Si le choix du joueur etait sur Pass et qu'il entre la meme mise qu'au debut => pas de changement
					else if (mise == joueurs[i].mise.money && joueurs[i].mise.choice == PASS) {
						printf("Votre mise n'a pas ete changee\n");
						hasmised = 1;
					} 
					//Le joueur qui a mise sur Don't Pass au debut ne peut pas augmenter sa mise (debut Deuxieme phase) 
					else if (mise > joueurs[i].mise.money && joueurs[i].mise.choice == DONT_PASS && joueurs[i].mise.money > 0) {
							printf("Vous ne pouvez pas augmenter votre mise\n");
							printf("Combien voulez vous miser d'argent ?\n");
							scanf("%d", &mise);
						
					}
					else if (mise == 0 && joueurs[i].mise.choice == DONT_PASS && joueurs[i].mise.money > 0) {
						//Si le joueur a mise sur "Don't pass", il peut annuler sa mise et donc l'a supprimer 
						printf("Votre mise a ete supprimee\n");
						joueurs[i].argent = joueurs[i].argent + joueurs[i].mise.money; //il reprend ce qu'il a mise
						joueurs[i].mise.money = 0; //on remet sa mise a 0
						hasmised = 1; 
					}
					else if (mise < 0 && joueurs[i].mise.choice == DONT_PASS && joueurs[i].mise.money > 0) {
						//Si le joueur a mise sur "Don't pass", il peut diminuer sa mise
						while (joueurs[i].mise.money + mise < 0) { //Si la diminution est trop grande (mise finale < 0)
							printf("Vous avez trop diminue votre mise\nDe combien voulez vous diminuer votre argent?\n");
							scanf("%d", mise);
						}

						printf("Votre mise a ete diminuee\n");
						joueurs[i].argent = joueurs[i].argent - mise; 
						joueurs[i].mise.money = joueurs[i].mise.money + mise; 
						hasmised = 1;
					}
					
					//Entrer une mise tant qu'elle est > 0 et que le joueur a assez d'argent pour jouer.
					else if (mise > 0 && joueurs[i].argent - mise >= 0) {
						//Debite la mise sur l'argent du joueur
						joueurs[i].mise.money = joueurs[i].mise.money + mise; 
						joueurs[i].argent = joueurs[i].argent - mise;
						hasmised = 1;
						mise = 0;
					}
					
					//Cas ou le joueur veut miser plus que ce qu'il lui reste comme argent
					else if (mise > 0 && joueurs[i].argent - mise < 0) {
						printf("Vous n'avez pas assez d'argent,il vous reste: %d euros.\n", joueurs[i].argent);
					}	
							
				}
				//Premiere suspension
				else if (numeroDeSuspension == 1) {
				
					//On peut doubler sa mise				
					int choixDoublermise = 0; //Booleen 
				
					printf("Premiere suspension. voulez vous doubler votre mise ?\nEntrez '1' pour la doubler\nEntrez '2' pour ne rien faire\n");
					scanf("%d", &choixDoublermise);
					
					//On verifie qu'il a plus d'argent que ce qu'il a mise 
					if (joueurs[i].argent >= joueurs[i].mise.money) {
						if (choixDoublermise == 1) { //il decide de doubler sa mise
							joueurs[i].argent = joueurs[i].argent - joueurs[i].mise.money; //la mise est debite sur son argent
							joueurs[i].mise.money = 2 * joueurs[i].mise.money; //On double ce que le joueur a mise
						}
					}
					//On verifie si il a assez d'argent pour doubler sa mise
					else {
						printf("Vous n'avez pas assez d'argent\n");
					}
					hasmised = 1;
				}
				else {					
					//Suspension suivante: on peut diminuer par facteur de 2 sa nouvelle mise				
					int choixDivisermise = 0; //Booleen
				
					printf("%dieme Suspension, Vous pouvez diminuer par facteur de 2 votre mise ?\nEntrez '1' pour la diviser par 2\nEntrez '2' pour ne rien faire\n",numeroDeSuspension );
					scanf("%d", &choixDivisermise);

					if (choixDivisermise == 1) { //Il decide de diviser sa mise
						joueurs[i].argent = joueurs[i].argent + joueurs[i].mise.money / 2; 
						joueurs[i].mise.money = joueurs[i].mise.money / 2;	//On divise par 2 l'ancienne mise		
					}
					hasmised = 1;
				}
			} while (!hasmised); //Boucle tant qu'il n'a pas mise
		}
		//Si son argent < 0 
		else {
			printf("%s, vous n'avez plus d'argent donc vous ne pouvez plus jouer.", joueurs[i].pseudo);
		}
	}
}


//Fonction qui renvoie l'index du joueur suivant (index du tableau de joueur)
int joueurSuivant(int numeroDuLanceur) {
	//Si le numero actuel du lanceur est egale au nombre de joueur -1, il s'agit du dernier joueur
	//Onfait un tour complet, on revient au premier joueur, donc le joueur numero 0
	if (numeroDuLanceur == nombreDeJoueurs - 1) {
		return 0;
	}
	else {
		//Sinon le tour est au prochain joueur
		return numeroDuLanceur + 1;
	}

}

/*DES*/

//permet de trouver un nombre aléa entre 1 et 6 (pour les faces des des)
void randomiseDe(int numeroDuJoueur) {
	printf("\nC'est a %s de lancer les des !\n", joueurs[numeroDuJoueur].pseudo);
	//manpage :"fonction qui utilise son argument comme "graine" pour la  génération d'une nouvelle séquence de nombre pseudo-aléatoires,
	//qui seront fournis par rand()"
	srand(time(NULL));

	de[0].face = 5; //rand() % (1 - 7) + 1; //[1; 7[
	de[1].face = 5; //rand() % (1 - 7) + 1; //[1; 7[

	printf("\nDes: %d + %d = %d\n\n", de[0].face, de[1].face, de[0].face + de[1].face);
}


int isCraps(De de1, De de2) {
	//On additionne la face des Des pour verifier si c'est un CRAPS (2 ou 12)
	if (de1.face + de2.face == 2 || de1.face + de2.face == 12) {
		return 1;
	}
	return 0;
}

int twoOrTwelve(De de1, De de2) {
	//On additionne la face des Des pour verifier si c'est un 2 ou un 12
	if (de1.face + de2.face == TWO) {
		return TWO;
	}
	else if(de1.face + de2.face == TWELVE) {
		return TWELVE;
	}
	return 0;
}

int isSeven(De de1, De de2) {
	//On additionne la face des Des pour verifier si c'est un 7
	if (de1.face + de2.face == SEVEN) {
		return 1;
	}
	return 0;
}



/*mise*/
void passersWin() {
	//Tous les joueurs ayant mise sur PASS ont gagne
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		//Si le Ième joueur a mise sur PASS
		if (joueurs[i].mise.choice == PASS) {
			int tmp = joueurs[i].mise.money;
			printf("Le joueur '%s' a gagne: %d euros\n", joueurs[i].pseudo, tmp);

			//Le joueur recupere son argent (qui correspont a sa mise) et gagne le montant de sa mise (donc mise + mise)
			joueurs[i].argent = joueurs[i].argent + 2 * tmp;
			joueurs[i].mise.money = 0;
		}
	}
}

void passersLoose() {
	//Les joueurs ayant mise sur PASS ont perdu
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		if (joueurs[i].mise.choice == PASS) {
			//Comme son compte a deja ete debite, on remet sa mise a O
			int tmp = joueurs[i].mise.money;
			printf("Le joueur '%s' a perdu: %d euros !\n", joueurs[i].pseudo, tmp);
			joueurs[i].mise.money = 0;
		}
	}
}

void dontPassWin() {
	//Les joueurs qui ont mise sur DONT PASS ont gagne
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		if (joueurs[i].mise.choice == DONT_PASS) {
			//meme principe que pour passersWin()
			int tmp = joueurs[i].mise.money;
			printf("Le joueur '%s' a gagne: %d euros !\n", joueurs[i].pseudo, tmp);
			//Le joueur recupere son argent (qui correspont à sa mise) et gagne le montant de sa mise (donc mise + mise)
			joueurs[i].argent = joueurs[i].argent + 2 * tmp;
			joueurs[i].mise.money = 0;
		}
	}
}

void dontPassLoose() {
	//Les joueurs ayant mise sur DONT PASS ont perdu
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		if (joueurs[i].mise.choice == DONT_PASS) {
			//même principe que pour passersLoose()
			int tmp = joueurs[i].mise.money;
			printf("Le joueur '%s' a perdu: %d euros \n", joueurs[i].pseudo, tmp);
			joueurs[i].mise.money = 0;
		}
	}
}

//On remet a 0 la mise et le choix des joueurs lors d'une nouvelle partie
void resetJoueurs() {
	int i = 0;
	for (i; i < nombreDeJoueurs; i++) {
		joueurs[i].mise.money = 0;
		joueurs[i].mise.choice = 0;
	}
}

/*SAVES*/


//Sauvegarde le pseudo et l'argent des joueurs dans un fichier cree en ecriture
void savePlayersInSave() {
	//Ouvre fichier et le cree en ecriture si il n'existe pas
	FILE* saveFile = fopen(SAVE_FILE_JOUEURS_NAME, "w+");
	int i = 0;
	//Une fois le fichier ouvert, on peut ecrire à l'intérieur
	for (i; i < nombreDeJoueurs; i++) {
		//On a le pseudo et l'argent delimite par un - 
		fprintf(saveFile, "%s-%d-\n", joueurs[i].pseudo, joueurs[i].argent);
	}
	//On ferme le fichier
	fclose(saveFile);
}


int isSaveJoueurs() {
	//On cree un pointeur sur le fichier de sauvegarde
	FILE* file = fopen(SAVE_FILE_JOUEURS_NAME, "r");
	//si le fichier est différent de NULL, cela veut dire qu'il existe, on renvoit dont 1, sinon 0
	if (file != NULL) {
		//On ferme le fichier
		fclose(file);
		return 1;
	}
	return 0;
}

//On va analyser les lignes de notre fichier 
char** processLine(const char* line) {
	//On alloue la memoire => on a un tableau de char*
	char** infos = (char**)malloc(2 * sizeof(char*));
	int i = 0;
	//pour chaque index, on alloue la memoire de la taille suivante: char[20]
	for (i; i < 2; i++) {
		infos[i] = (char*)malloc(20 * sizeof(char));
	}
	//strtok renvoie la ligne jusqu'au premier ';'
	//on copie ensuite ce bout de ligne dans infos[0]
	//et on recommence pour avoir l'autre bout de ligne
	strcpy(infos[0], strtok(line, "-"));
	//man strtok: "Dans chaque appel ulterieur fait pour analyser la même chaine, str doit etre NULL."
	strcpy(infos[1], strtok(NULL, "-"));
	//Structure du fichier de sauvegarde:
	//<Pseudo>;<Argent>;
	//monPseudo;50;

	return infos;
}


//Obtenir le nombre de joueur qui a ete sauvegarde dans la derniere partie jouee
int getPlayerNumberFromSave() {
	//On ouvre le fichier en lecture
	FILE* saveFile = fopen(SAVE_FILE_JOUEURS_NAME, "r");
	char str[50];

	int numberOfPlayers = 0;
	//Lis ligne par ligne le fichier de save, et renvoie le nombre de lignes (et donc le nombre de joueurs)
	while (fgets(str, 50, saveFile) != NULL) {
		numberOfPlayers = numberOfPlayers + 1;
	}
	//On ferme le fichier
	fclose(saveFile);
	
	//On retourne le nombre de joueurs dans le fichier 
	return numberOfPlayers;
}

//On recree les anciens joueurs avec leur pseudo et leur argent a la fin (tout ce qui a ete sauvegarde)
void createPlayersFromSave() {
	FILE* saveFile = fopen(SAVE_FILE_JOUEURS_NAME, "r");
	char str[255];

	initialise(getPlayerNumberFromSave());

	int numeroDuJoueur = 0;
	while (fgets(str, 255, saveFile) != NULL) {
		//On alloue la memoire pour le pseudo du joueur (qui est un char*)
		//strlen renvoi la taille du char* passe en parametre 
		joueurs[numeroDuJoueur].pseudo = (char*)malloc(strlen(str) * sizeof(char));

		//On découpe notre ligne
		char** result = processLine(str);

		//strcpy copie le contenue de result[0] dans joueurs[nombreDeJoueurs]
		strcpy(joueurs[numeroDuJoueur].pseudo, result[0]);
		//atoi converti le char* en int -> ici on converti la string qui correspont a l'argent du joueur
		joueurs[numeroDuJoueur].argent = atoi(result[1]);
		//On remet leur choix et leur mise a 0
		joueurs[numeroDuJoueur].mise.choice = 0;
		joueurs[numeroDuJoueur].mise.money = 0;

		numeroDuJoueur = numeroDuJoueur + 1;
	}
	//On ferme le fichier
	fclose(saveFile);
}








