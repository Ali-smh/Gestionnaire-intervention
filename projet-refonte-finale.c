#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#pragma warning (disable: 4996)

//Definition des nouveaux types
/**
 * @enum Boolean
 * @brief valeurs booleenes
 */
typedef enum { FALSE, TRUE } Boolean;

/**
 * @brief Dimensions maximales
 */
enum {
	NB_COMMANDE = 9,
	NB_ROLE = 3,
	LONG_MAX_ENTREE = 80,
	LONG_MAX_COMMANDE = 15,
	LONG_MAX_ROLE = 3,
	LONG_MAX_NOM = 31,
	MAX_NB_ENTREPRISE = 50,
	MAX_MISSIONS = 500,
	MAX_SOUS_TRAITANCE = 5,
	MAX_RAPPORT = 4
};

//Definition des nouveaux types : structures
/**
 * @struct Entreprise
 */
typedef struct{
	char nom[LONG_MAX_NOM];
	char role[LONG_MAX_ROLE];
	int identifiant;
	Boolean estOccupee;
}Entreprise;

/**
 * @struct Resultat
 */
typedef struct {
	char signification[LONG_MAX_ENTREE];
	float majoration;
}Resultat;

/**
 * @struct Mission
 */
typedef struct{
	char nom[LONG_MAX_NOM];
	int identifiant;
	float remuneration;
	int niveau_sous_traitance;
	Entreprise publicateur;
	Entreprise occupant;
	Entreprise anciensOccupants[MAX_RAPPORT];
	Resultat rapports[MAX_RAPPORT];
	Boolean estAttribuee;
	Boolean estTerminee;
	int nbRapport;
	int idChaineSousTraitance;
}Mission;


//Fonctions d'utilité

/**
 * @brief Affiche l'id, le nom, le nom du publicateur, la remuneration et le niveau de sous-traitance d'une mission
 */
void afficheMission(Mission mission)
{
	printf("%d %s %s %.2f (%d)\n", mission.identifiant, mission.nom, mission.publicateur.nom, mission.remuneration, mission.niveau_sous_traitance);
}

/**
 * @brief Ajoute une mission aux dans un tableau de missions
 */
void ajouteMission(Mission* mission, Mission tabMissions[MAX_MISSIONS], int* nbMissions)
{
	strcpy(tabMissions[*nbMissions].nom, mission->nom);
	tabMissions[*nbMissions].identifiant = *nbMissions + 1;
	tabMissions[*nbMissions].remuneration = mission->remuneration;
	tabMissions[*nbMissions].niveau_sous_traitance = 0;
	tabMissions[*nbMissions].publicateur = mission->publicateur;
	tabMissions[*nbMissions].nbRapport = mission->nbRapport;
	tabMissions[*nbMissions].estAttribuee = FALSE;
	tabMissions[*nbMissions].estTerminee = FALSE;
	for (int i = 0; i < mission->nbRapport; ++i) { tabMissions[*nbMissions].rapports[i] = mission->rapports[i];}
	(*nbMissions)++;
}

/**
 * @brief Recherche une entreprise par identifiant parmis un tableau d'entreprises. Renvoie l'indice de l'entreprise si elle est trouvee sinon -1
 */
int rechercheEntrepriseParId(int identifant, const Entreprise tabEntreprise[MAX_NB_ENTREPRISE], int nbEntreprise)
{
	for (int i = 0; i < nbEntreprise; ++i) { if (tabEntreprise[i].identifiant == identifant) return i; }
	return -1;
}

/**
 * @brief Recherche une mission par identifiant parmis un tableau de mission. Renvoie l'indice de la mission si elle est trouvee sinon -1
 */
int rechercheMissionParId(int identifant, const Mission tabMissions[MAX_MISSIONS], int nbMission)
{
	for (int i = 0; i < nbMission; ++i){if (tabMissions[i].identifiant == identifant)return i;}
	return -1;
}

/**
 * @brief Recherche le niveau maximum de sous-traitance d'une chaine de sous-traitance
 */
int recherhceNivMaxSousTraitance(Mission tabMissions[MAX_MISSIONS], int* nbMission, int idChaineSousTraitance)
{
	int maxNivSousTraitance = 0;
	for (int j = 0; j < *nbMission; ++j)
	{
		if (tabMissions[j].niveau_sous_traitance > maxNivSousTraitance && tabMissions[j].idChaineSousTraitance == idChaineSousTraitance)
		{
			maxNivSousTraitance = tabMissions[j].niveau_sous_traitance;
		}
	}
	return maxNivSousTraitance;
}

/**
 * @brief Renvoie TRUE si l'entreprise d'identifiant "identifiantEntreprise" est un ancien occupant de la mission "mission".
 */
Boolean estUnAncienOccupant(int identifiantEntreprise, Mission mission)
{
	for (int i = 0; i < MAX_RAPPORT; i++) { if (mission.anciensOccupants[i].identifiant == identifiantEntreprise) return TRUE; }
	return FALSE;
}

/**
 * @brief Ajoute une entreprise à aux entreprises inscrites
 */
void ajouteEntreprise(Entreprise* entreprise, Entreprise listeEntreprises[MAX_NB_ENTREPRISE], int* nbInscription)
{
	strcpy(listeEntreprises[*nbInscription].nom, entreprise->nom);
	strcpy(listeEntreprises[*nbInscription].role, entreprise->role);
	listeEntreprises[*nbInscription].identifiant = *nbInscription + 1;
	(*nbInscription)++;
}


/**
 * @brief Renvoie TRUE si un role existe, sinon FALSE
 */
Boolean verificationRole(char role[LONG_MAX_ROLE]) {
	char tabRoles[NB_ROLE][LONG_MAX_ROLE] = { "OP","AG","IN" };
	for (int i = 0; i < NB_ROLE; i++) { if (strcmp(role, tabRoles[i]) == 0) return TRUE; }
	return FALSE;
}

/**
 * @brief Vérifie qu'un nom n'est pas deja present dans un tableau. Renvoie TRUE si c'est le cas, sinon FALSE
 */
Boolean verificationNom(char nom[LONG_MAX_NOM], const Entreprise listeInscrits[MAX_NB_ENTREPRISE])
{
	for (int i = 0; i < MAX_NB_ENTREPRISE; ++i) { if (strcmp(nom, listeInscrits[i].nom) == 0) return FALSE; }
	return TRUE;
}

// Fonctions générales

/**
 * @brief Procède à l'inscription d'une entreprise.
 */
void inscription(Entreprise* entreprise, Entreprise listeInscrits[MAX_NB_ENTREPRISE], int* nbInscription)
{
	char roleSaisie[LONG_MAX_ROLE];
	char nomSaisie[LONG_MAX_NOM];
	scanf("%s", roleSaisie);
	scanf("%s", nomSaisie);
	if (!verificationRole(roleSaisie)) printf("Role incorrect\n");
	else if (!verificationNom(nomSaisie, listeInscrits)) printf("Nom incorrect\n");
	else
	{
		strcpy(entreprise->nom, nomSaisie);
		strcpy(entreprise->role, roleSaisie);
		ajouteEntreprise(entreprise, listeInscrits, nbInscription);
		printf("Inscription realisee (%d)\n", *nbInscription);
	}
}

/**
 * @brief Procede à la publication d'une mission
 */
void publication(Mission* mission, Mission tabMissions[MAX_MISSIONS], Entreprise tabEntreprises[MAX_NB_ENTREPRISE], int* nbMissions)
{
	int identifiantEntrepriseSaisie;
	char nomMissionSaisie[LONG_MAX_NOM];
	float remunerationSaisie;
	int indiceEntrepiseTrouvee;
	scanf("%d", &identifiantEntrepriseSaisie);
	scanf("%s", nomMissionSaisie);
	scanf("%f", &remunerationSaisie);
	indiceEntrepiseTrouvee = rechercheEntrepriseParId(identifiantEntrepriseSaisie, tabEntreprises, MAX_NB_ENTREPRISE);
	if (indiceEntrepiseTrouvee == -1 || strcmp(tabEntreprises[indiceEntrepiseTrouvee].role, "OP") != 0) printf("Identifiant incorrect\n");
	else if (remunerationSaisie <= 0.0) printf("Remuneration incorrecte\n");
	else
	{
		strcpy(mission->nom, nomMissionSaisie);
		mission->remuneration = remunerationSaisie;
		mission->publicateur = tabEntreprises[indiceEntrepiseTrouvee];
		ajouteMission(mission, tabMissions, nbMissions);
		printf("Mission publiee (%d)\n", *nbMissions);
	}
}

/**
 * @brief Affiche les missions non attribuées
 */
void consultation(const Mission listeMissions[MAX_MISSIONS], int* nbMission)
{
	Mission missionNA;
	Boolean MissionsTrouvees = FALSE;
	for (int i = 0; i < *nbMission; ++i)
	{
		if (listeMissions[i].estAttribuee == FALSE && listeMissions[i].estTerminee == FALSE)
		{
			missionNA = listeMissions[i];
			afficheMission(missionNA);
			MissionsTrouvees = TRUE;
		}
	}
	if (MissionsTrouvees == FALSE) printf("Aucune mission disponible\n");
}

/**
 * @brief Affiche le detail d'une missions non attribuée
 */
void detail(Mission tabMissions[MAX_MISSIONS], int* nbMission)
{
	Mission missionTrouvee;
	Boolean possedeRapports = FALSE;
	int indiceMissionTrouvee;
	missionTrouvee.nbRapport = 0;
	int identifiantMission;
	scanf("%d", &identifiantMission);
	indiceMissionTrouvee = rechercheMissionParId(identifiantMission, tabMissions, *nbMission);
	missionTrouvee = tabMissions[indiceMissionTrouvee];
	if (indiceMissionTrouvee == -1 || missionTrouvee.estAttribuee == TRUE) printf("Identifiant incorrect \n");
	else
	{
		afficheMission(missionTrouvee);
		if (missionTrouvee.nbRapport > 0)
		{
			for (int j = missionTrouvee.nbRapport - 1; j >= 0; --j)
			{
				printf("%s\n", missionTrouvee.rapports[j].signification);
			}
		}
	}
}

/**
 * @brief Sous-traite une mission et publie automatiquement une nouvelle mission
 */
void sous_traitance(Entreprise tabEntreprises[MAX_NB_ENTREPRISE], Mission tabMission[MAX_MISSIONS], int* nbMission, int* nbEntreprise, int* nbChaineSousTraitance)
{
	int identifiantEntreprise;
	int identifiantMission;
	float remunerationSaisie;
	int indiceMissionTrouvee;
	int indiceEntrepriseTrouvee;
	Entreprise occupantVide = { "Vide", "IN", 0 };

	scanf("%d", &identifiantEntreprise);
	scanf("%d", &identifiantMission);
	scanf("%f", &remunerationSaisie);

	indiceMissionTrouvee = rechercheMissionParId(identifiantMission, tabMission, *nbMission);
	indiceEntrepriseTrouvee = rechercheEntrepriseParId(identifiantEntreprise, tabEntreprises, *nbEntreprise);

	if (indiceEntrepriseTrouvee == -1 || strcmp(tabEntreprises[indiceEntrepriseTrouvee].role, "AG")) printf("Entreprise incorrecte \n");
	else if (indiceMissionTrouvee == -1) printf("Mission incorrecte\n");
	else if (remunerationSaisie <= 0.0) printf("Remuneration incorrecte\n");
	else if (indiceMissionTrouvee != -1)
	{
		if (tabMission[indiceMissionTrouvee].estAttribuee == TRUE || tabMission[indiceMissionTrouvee].niveau_sous_traitance >= MAX_SOUS_TRAITANCE) printf("Mission incorrecte\n");
		else
		{		
			//La mission sous-traitee devient attribuee
			if (tabMission[indiceMissionTrouvee].niveau_sous_traitance == 0) (*nbChaineSousTraitance)++;
			tabMission[indiceMissionTrouvee].occupant = occupantVide;
			tabMission[indiceMissionTrouvee].estAttribuee = TRUE;
			tabMission[indiceMissionTrouvee].idChaineSousTraitance = *nbChaineSousTraitance;
			tabMission[*nbMission] = tabMission[indiceMissionTrouvee];
			//Creation de la nouvelle mission
			tabMission[*nbMission].identifiant += 1;
			tabMission[*nbMission].niveau_sous_traitance += 1;
			tabMission[*nbMission].remuneration = remunerationSaisie;
			tabMission[*nbMission].occupant = occupantVide;
			tabMission[*nbMission].publicateur = tabEntreprises[indiceEntrepriseTrouvee];
			tabMission[*nbMission].estAttribuee = FALSE;
			printf("Sous-traitance enregistree (%d)\n", tabMission[*nbMission].identifiant);
			(*nbMission)++;
		}
	}
}

/**
 * @brief Afficher un compte-rendu de chaque mission impliquant l’entreprise désignée
 */
void recapitulatif(Entreprise tabEntreprise[MAX_NB_ENTREPRISE], int* nbEntreprise, Mission tabMissions[MAX_MISSIONS], int* nbMission)
{
	int identifiantEntreprise;
	int indiceEntrepriseTrouvee;
	Entreprise entrepriseTrouvee;

	scanf("%d", &identifiantEntreprise);

	indiceEntrepriseTrouvee = rechercheEntrepriseParId(identifiantEntreprise, tabEntreprise, *nbEntreprise);
	entrepriseTrouvee = tabEntreprise[indiceEntrepriseTrouvee];

	if (indiceEntrepriseTrouvee == -1) printf("Entreprise incorrecte\n");
	else
	{
		//Affichage du compte-rendu
		Boolean afficheNA = FALSE, afficheAttribuees = FALSE, afficheTerminees = FALSE, afficheArealiser = FALSE, afficheRealisees = FALSE;
		//Non attribuées
		for (int i = 0; i < *nbMission; ++i) {
			if (tabMissions[i].publicateur.identifiant == entrepriseTrouvee.identifiant) {
				if (tabMissions[i].estAttribuee == FALSE && tabMissions[i].estTerminee == FALSE) {
					if (afficheNA == FALSE) {
						printf("* non attribuees\n");
						afficheNA = TRUE;
					}
					afficheMission(tabMissions[i]);
				}
			}
		}
		//Attribuees
		for (int i = 0; i < *nbMission; ++i) {
			if (tabMissions[i].publicateur.identifiant == entrepriseTrouvee.identifiant) {
				if (tabMissions[i].estAttribuee == TRUE && tabMissions[i].estTerminee == FALSE) {
					if (afficheAttribuees == FALSE) {
						printf("* attribuees\n");
						afficheAttribuees = TRUE;
					}
					afficheMission(tabMissions[i]);
				}
			}
		}
		//Terminees
		for (int i = 0; i < *nbMission; ++i) {
			if (tabMissions[i].publicateur.identifiant == entrepriseTrouvee.identifiant) {
				if (tabMissions[i].estTerminee == TRUE) {
					if (afficheTerminees == FALSE) {
						printf("* terminees\n");
						afficheTerminees = TRUE;
					}
					afficheMission(tabMissions[i]);
				}
			}
		}
		// A realiser
		for (int i = 0; i < *nbMission; ++i) {
			if (tabMissions[i].occupant.identifiant == entrepriseTrouvee.identifiant) {
				if (tabMissions[i].estTerminee == FALSE) {
					if (afficheArealiser == FALSE) {
						printf("* a realiser\n");
						afficheArealiser = TRUE;
					}
					afficheMission(tabMissions[i]);
				}
			}
		}
		//Realisees
		for (int i = 0; i < *nbMission; ++i) {
			if (tabMissions[i].occupant.identifiant == entrepriseTrouvee.identifiant) {
				if (tabMissions[i].estTerminee == TRUE) {
					if (afficheRealisees == FALSE) {
						printf("* realisees\n");
						afficheRealisees = TRUE;
					}
					afficheMission(tabMissions[i]);
				}
			}
		}
	}
}

/**
 * @brief Fais le rapport d'une mission
 */
void rapport(Mission tabMissions[MAX_MISSIONS], Entreprise tabEntreprises[MAX_NB_ENTREPRISE], int *nbMission) {

	int identifiantMission;
	int codeDeRetour;
	int indiceMissionTrouvee;
	Mission missionTrouvee;
	Mission nouvelleMission;
	Entreprise occupantVide = {"Vide", "IN", 0};
	Resultat resultatRapports[MAX_RAPPORT] = {
		{"Succes", 0},
		{"Local non accessible", 1.00},
		{"Pas  de signal dans le boitier general", 1.055},
		{"Recepteur defectueux", 1.04}
	};

	scanf("%d", &identifiantMission);
	scanf("%d", &codeDeRetour);

	indiceMissionTrouvee = rechercheMissionParId(identifiantMission, tabMissions, *nbMission);
	missionTrouvee = tabMissions[indiceMissionTrouvee];
	//Recherche du niv max de sous traitance dans la chaine de sous traitance de la mission trouvee
	int maxNivSousTraitance = recherhceNivMaxSousTraitance(tabMissions, nbMission, missionTrouvee.idChaineSousTraitance);

	if (indiceMissionTrouvee == -1 || missionTrouvee.estAttribuee == FALSE || maxNivSousTraitance != missionTrouvee.niveau_sous_traitance)
	{
		printf("Mission incorrecte \n");
	}
	else if (codeDeRetour >= MAX_RAPPORT || codeDeRetour < 0)
	{
		printf("Code de retour incorrect\n");
	}
	else
	{
		if (codeDeRetour == 0)
		{
			tabMissions[indiceMissionTrouvee].estTerminee = TRUE;
			tabMissions[indiceMissionTrouvee].estAttribuee = FALSE;
			//Toutes les missions de la meme chiane de sous-traitance deviennent aussi terminees
			for (int i = 0; i < *nbMission; ++i)
			{
				if (tabMissions[i].idChaineSousTraitance == tabMissions[indiceMissionTrouvee].idChaineSousTraitance)
				{
					tabMissions[i].estTerminee = TRUE;
					tabMissions[i].estAttribuee = FALSE;
				}
			}
			printf("Rapport enregistre\n");
		}
		else
		{
			//Création de la nouvelle mission
			nouvelleMission = missionTrouvee;
			nouvelleMission.remuneration *= resultatRapports[codeDeRetour].majoration;
			nouvelleMission.rapports[nouvelleMission.nbRapport] = resultatRapports[codeDeRetour];
			nouvelleMission.anciensOccupants[nouvelleMission.nbRapport] = nouvelleMission.occupant;
			//Selection de la nouvelle mission
			for (int k = 0; k < MAX_RAPPORT; ++k)
			{
				tabMissions[indiceMissionTrouvee + 1].anciensOccupants[k] = nouvelleMission.anciensOccupants[k];
			}
			tabMissions[indiceMissionTrouvee + 1].anciensOccupants[nouvelleMission.nbRapport] = missionTrouvee.occupant;
			nouvelleMission.nbRapport += 1;
			nouvelleMission.identifiant += 1;
			printf("Rapport enregistre (%d)\n", nouvelleMission.identifiant);
			ajouteMission(&nouvelleMission, tabMissions, nbMission);
			//La mission dont on fait le rapport devient terminee
			tabMissions[indiceMissionTrouvee].estTerminee = TRUE;

		}
		for (int j = 0; j < MAX_NB_ENTREPRISE; ++j)
		{
			if (tabEntreprises[j].identifiant == missionTrouvee.occupant.identifiant)
			{
				tabEntreprises[j].estOccupee = FALSE;
			}
		}
	}
}

/**
 * @brief Procède à l'acceptation d'une mission par une entreprise
 */
void accceptation(Mission tabMissions[MAX_MISSIONS], Entreprise tabEntreprises[MAX_NB_ENTREPRISE], int *nbMissions, int* nbEntreprises) {
	int identifiantEntreprise;
	int identifiantMission;
	int indiceMissionTrouvee;
	int indiceEntrepriseTrouvee;
	Entreprise entrepriseTrouvee;
	Mission missionTrouvee;

	scanf("%d", &identifiantEntreprise);
	scanf("%d", &identifiantMission);

	indiceMissionTrouvee = rechercheMissionParId(identifiantMission, tabMissions, *nbMissions);
	missionTrouvee = tabMissions[indiceMissionTrouvee];
	indiceEntrepriseTrouvee = rechercheEntrepriseParId(identifiantEntreprise, tabEntreprises, *nbEntreprises);
	entrepriseTrouvee = tabEntreprises[indiceEntrepriseTrouvee];
	
	if (indiceEntrepriseTrouvee == -1 || 
		strcmp(entrepriseTrouvee.role, "OP") == 0 || 
		(entrepriseTrouvee.estOccupee == TRUE && missionTrouvee.estAttribuee == FALSE) ||
		estUnAncienOccupant(identifiantEntreprise, missionTrouvee) == TRUE)
	{
		printf("Entreprise incorrecte\n");
	}
	else if (indiceMissionTrouvee == -1 || missionTrouvee.estAttribuee == TRUE) printf("Mission incorrecte\n");
	else
	{
		//La mission trouvee passe en attribué 
		tabMissions[indiceMissionTrouvee].occupant = entrepriseTrouvee;
		tabMissions[indiceMissionTrouvee].estAttribuee = TRUE;
		tabEntreprises[indiceEntrepriseTrouvee].estOccupee = TRUE;
		printf("Acceptation enregistree\n");
	}
}

//Main
/**
 * @brief C'est ici que tout s'execute
 */
int main()
{
	Entreprise entreprise;
	Mission mission;
	Entreprise entreprisesInscrites[MAX_NB_ENTREPRISE];
	Mission missionsPubliees[MAX_MISSIONS];
	int nbInscriptions = 0, nbMissions = 0, nbChaineSousTraitance = 0;
	mission.nbRapport = 0;
	char commande[LONG_MAX_COMMANDE];
	do {
		scanf("%s", commande);
		if (strcmp(commande, "inscription") == 0) inscription(&entreprise, entreprisesInscrites, &nbInscriptions);
		else if (strcmp(commande, "mission") == 0) publication(&mission, missionsPubliees, entreprisesInscrites, &nbMissions);
		else if (strcmp(commande, "consultation") == 0) consultation(missionsPubliees, &nbMissions);
		else if (strcmp(commande, "detail") == 0) detail(missionsPubliees, &nbMissions);
		else if (strcmp(commande, "acceptation") == 0) accceptation(missionsPubliees, entreprisesInscrites, &nbMissions, &nbInscriptions);
		else if (strcmp(commande, "rapport") == 0) rapport(missionsPubliees, entreprisesInscrites, &nbMissions);
		else if (strcmp(commande, "recapitulatif") == 0) recapitulatif(entreprisesInscrites, &nbInscriptions, missionsPubliees, &nbMissions);
		else if (strcmp(commande, "sous-traitance") == 0) sous_traitance(entreprisesInscrites, missionsPubliees, &nbMissions, &nbInscriptions, &nbChaineSousTraitance);
	}while(strcmp("exit", commande) != 0);
}