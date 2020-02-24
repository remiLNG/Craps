// Longin Remi 21600664 //

#define INITIAL_MONEY 50
#define PASS 1
#define DONT_PASS 2
#define TWO 2
#define TWELVE 12
#define SEVEN 7
#define YES "y"
#define NO "n"
#define SAVE_FILE_JOUEURS_NAME "saveJoueurs.txt"

/*STRUCT*/

//Pas besoin de typedef ici car on n'appellera pas Mise (tout sera appele dans Joueur)
struct Mise {
	int choice; 
	int money;
};

//On utilise des typedef pour éviter de rappeler struct
typedef struct Joueur {
	char* pseudo;
	int argent;
	struct Mise mise;
}Joueur;

typedef struct De {
	int face;
}De;


//joueur qu'on peut allouer dynamiquement
Joueur* joueurs;
//Creer Deux des
De de[2];

//ON doit compter le nombre de joueurs / Besoin d'un Booleen pour dire si on est en jeu ou pas(utile pour le main) / On doit compter les suspensions
int nombreDeJoueurs, jouer, numeroDeSuspension;

/*PARTIE*/
void initialise(int nbJoueurs);
void creerJoueurs();
void afficherJoueurs();
void choisirMise();
int joueurSuivant(int joueurActuel);
void resetJoueurs();

/*MISES*/
void passersWin();
void passersLoose();
void dontPassWin();
void dontPassLoose();

/*DES*/
int isCraps(De de1, De de2);
int twoOrTwelve(De de1, De de2);
int isSeven(De de1, De de2);
void randomiseDe(int joueurID);


/*SAVES*/
int isSaveJoueurs();
void savePlayersInSave();
void createPlayersFromSave();
