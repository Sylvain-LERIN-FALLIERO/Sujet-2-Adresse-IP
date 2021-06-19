/******************************************************************************
* ASR => M2101								      *
*******************************************************************************
*									      *
* N° de Sujet : 2							      *
*									      *
*******************************************************************************
*									      *
* Intitulé : Analyse adresse IP						      *
*									      *
*******************************************************************************
*									      *
* Nom-prénom1 : LERIN-FALLIERO Sylvain					      *
*									      *
* Nom-prénom2: /							      *
*									      *
*******************************************************************************
*									      *
* Nom du fichier : main.c						      *
*									      *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*Vérifier le format.*/
int verifierFormat(char ip[]);
/*Extraire les champs et les convertir en valeurs numériques.*/
/*On utilise des long dans le reste du programme pour pouvoir utiliser la fonction strtol.*/
long extraireSegment(char ip[], int seg);
/*Déterminer la classe de l'adresse.*/
char determinerClasse(long oct1, long oct2);
/*Déterminer le type de l'adresse.*/
char determinerType(long oct1, long oct2, long oct3, long oct4, long mask);
/*Déterminer l'adresse réseau et l'adresse de l'hôte.*/
/*On utilise des pointeurs pour pouvoir stocker les différentes valeurs des octets.*/
void calculIpReseauEtHote(long *oct1, long *oct2, long *oct3, long *oct4, long mask);
/*Afficher les résultats.*/
void affichage(char class, char type, long oct1, long oct2, long oct3, long oct4, long mask);

void main(){

	char ip[18]; /*Adresse IP à analyser.*/
	long oct1; /*Premier octet de l'adresse IP.*/
	long oct2; /*Deuxième octet de l'adresse IP.*/
	long oct3; /*Troisième octet de l'adresse IP.*/
	long oct4; /*Quatrième octet de l'adresse IP.*/
	long mask; /*Masque de l'adresse IP.*/
	char class; /*La classe de  l'adresse.*/
	char type; /*Le type de l'adresse.*/

	printf("Entrez une adresse IP :\n");
	scanf("%s", ip);

	/*Vérifier le format.*/
	if (verifierFormat(ip) == 1){
		printf("Format d'adresse IP valide.\n");

		/*Extraire les champs et les convertir en valeurs numériques.*/
		oct1 = extraireSegment(ip, 1);
		oct2 = extraireSegment(ip, 2);
		oct3 = extraireSegment(ip, 3);
		oct4 = extraireSegment(ip, 4);
		mask = extraireSegment(ip, 5);

		/*Déterminer la classe de l'adresse.*/
		class = determinerClasse(oct1,oct2);
		
		
		/*Déterminer le type de l'adresse.*/
		type = determinerType(oct1,oct2,oct3,oct4,mask);

		/*Déterminer l'adresse réseau et l'adresse de l'hôte.*/
		calculIpReseauEtHote(&oct1,&oct2,&oct3,&oct4,mask);

		/*Afficher les résultats.*/
		affichage(class,type,oct1,oct2,oct3,oct4,mask);	
		
		
	} else {
		printf("Format d'adresse IP invalide.\n");
	}
}

/*TRAITEMENT : VERIFIER LE FORMAT*/
int verifierFormat(char ip[]){
/*Retourne 0 à chaque fois qu'un élément invalide la chaine de caractère. Retourne 1 si elle à le bon format.*/

	char stock[3]; /*Stocke les caractères avant des les placer dans les segments.*/
	int y = 0; /*Compteur pour stock.*/
	int seg = 0; /*Compteur de séparateurs ( . . . / ).*/

	for (int i = 0; i < strlen(ip); i++){
	/*Boucle qui vérifie chaque caractère de la chaine*/

		/*Si les caractères sont des chiffres entre 0 et 9, ils sont stockés temporairement.
		stock représente le segment en cours.*/
		if (ip[i] >= '0' && ip[i] <= '9'){	
			stock[y] = ip[i];
			y++;

			/*On vérifie que les chiffres du segment en cours sont compris entre 0 et 255.*/
			/*La chaine stock est convertie en long integer afin de pouvoir faire la comparaison.*/
			if (!(strtol(stock, '\0', 10) >= 0 && strtol(stock, '\0', 10) <= 255)){
				return 0;
			}	
		

		/*Les trois premiers séparateurs doivent être un '.'
		et le segment précédent ne doit pas être vide.*/
		} else if (seg < 3 && ip[i] == '.' && stock[0] != '\0'){
			y = 0;
			seg++;

			/*On "vide" manuellement le stock pour faire de la place au prochain segment.*/
			stock[0] = '\0';
			stock[1] = '\0';
			stock[2] = '\0';
		
		

		/*Le dernier séparateur doit être un '/', et on vérifie la validité du segment précédent.*/
		} else if (seg == 3 && ip[i] == '/' && stock[0] != '\0'){
			y = 0;
			seg++;

			/*On "vide" manuellement le stock pour faire de la place au prochain segment.*/
			stock[0] = '\0';
			stock[1] = '\0';
			stock[2] = '\0';
		
		
		/*Si le caractère n'est ni un chiffre entre 0 et 9, ni . ni / alors il est invalide.*/
		} else {
			return 0;
		}
	
	}

	/*Vérifie que les 4 séparateurs sont bien en place.*/
	if (seg != 4){
		return 0;
	}

	/*Vérifie qu'un segment est bien présent après le dernier séparateur.*/
	if (stock[0] == '\0'){
		return 0;
	}

	/*Vérifie que la valeur du dernier segment (masque) est correcte.*/
	if (!(strtol(stock, '\0', 10) >= 0 && strtol(stock, '\0', 10) <= 32)){
		return 0;
	}

	/*Si le programme n'a trouvé aucune erreur, le format est valide.*/
	return 1;
}


/*TRAITEMENT : EXTRAIRE LES CHAMPS et CONVERTIR EN VALEUR NUMERIQUE*/
long extraireSegment(char ip[], int segment){
/*Fonctionne de façon similaire à la fonction précédente, mais va s'arréter pour renvoyer le segment stock désiré converti en valeur numérique.*/

	char stock[3]; /*Stocke les caractères avant des les placer dans les segments.*/
	int y = 0; /*Compteur pour stock.*/
	int seg = 0; /*Compteur de séparateurs ( . . . / ).*/

	for (int i = 0; i < strlen(ip); i++){
	/*Boucle qui vérifie chaque caractère de la chaine*/

		/*Si les caractères sont des chiffres entre 0 et 9, ils sont stockés temporairement.
		stock représente le segment en cours.*/
		if (ip[i] >= '0' && ip[i] <= '9'){	
			stock[y] = ip[i];
			y++;
		/*Les trois premiers séparateurs doivent être un '.'
		et le segment précédent ne doit pas être vide.*/
		} else if (seg < 3 && ip[i] == '.' && stock[0] != '\0'){
			y = 0;
			seg++;

			/*Si le segment précédent est le segment désiré, il est converti en valeur numérique et 			renvoyé.*/
			if (seg == segment){
				return strtol(stock, '\0', 10);
			}

			/*On "vide" manuellement le stock pour faire de la place au prochain segment.*/
			stock[0] = '\0';
			stock[1] = '\0';
			stock[2] = '\0';
		
		

		/*Le dernier séparateur doit être un '/', et on vérifie la validité du segment précédent.*/
		} else if (seg == 3 && ip[i] == '/' && stock[0] != '\0'){
			y = 0;
			seg++;
				
			/*Si le segment précédent est le segment désiré, il est converti en valeur numérique et 			renvoyé.*/
			if (seg == segment){
				return strtol(stock, '\0', 10);
			}
			
			/*On "vide" manuellement le stock pour faire de la place au prochain segment.*/
			stock[0] = '\0';
			stock[1] = '\0';
			stock[2] = '\0';
		}
	}
	
	/*Si le segment désiré est le dernier segment, on le traite ici.*/
	if (segment == 5){
		return strtol(stock, '\0', 10);
	}
}

/*TRAITEMENT : DECODER IP - CLASSE*/
char determinerClasse(long oct1, long oct2){
/*On va comparer les segments de notre adresse à ceux des diffférentes classes d'adresse.*/
	char class;
	
	/*Classe A.*/
	if (oct1 >= 0 && oct1 <= 126){
		class = 'A';
	/*Classe B.*/
	} else if (oct1 >= 128 && oct1 <= 191){
		class = 'B';
	/*Classe C.*/
	} else if (oct1 >= 192 && oct1 <= 223){
		class = 'C';
	/*Classe D.*/
	} else if (oct1 >= 224 && oct1 <= 239){
		class = 'D';
	/*Classe E.*/
	} else if (oct1 >= 240 && oct1 <= 255){
		class = 'E';
	}

	
	/*Vérifie si l'adresse est une adresse privée (en fonction de sa classe).*/
	if (class == 'A' && oct1 == 10){
		class = 'a';
	}
	if (class == 'B' && oct1 == 172 && oct2 >= 16 && oct2 <=31){
		class = 'b';
	}
	if (class == 'C' && oct1 == 192 && oct2 == 168){
		class = 'c';
	}

	/*La valeur class est traitée dans l'affichage.*/
	return class;
}

/*TRAITEMENT : DECODER IP - TYPE*/
char determinerType(long oct1, long oct2, long oct3, long oct4, long mask){
/*Vérifier si l'adresse est localhost, broadcast ou multicast.*/
	
	/*Localhost.*/
	if (oct1 == 127 && oct2 == 0 && oct3 == 0 && oct4 == 1){
		return 'l';
	}
	/*Multicast.*/
	else if (oct1 >= 224 && oct1 <= 239){
		return 'm';
	}
	/*Broadcast.*/
	else if (mask >= 0 && mask <= 8 && oct2 == 255 && oct3 == 255 && oct4 == 255){
		return 'b';
	}
		
	else if (mask >= 9 && mask <= 16 && oct3 == 255 && oct4 == 255){
		return 'b';
	}
	else if (mask >= 17 && mask <= 24 && oct4 == 255){
		return 'b';
	}
	
}

/*TRAITEMENT : CALCULER @RESEAU/@HÔTE*/
void calculIpReseauEtHote(long *oct1, long *oct2, long *oct3, long *oct4, long mask){
/*Calcule l'adresse réseau et l'adresse de l'hôte en fonction du masque.*/
/*On stocke les valeurs des octets de l'adresse réseau dans des pointeurs afin de pouvoir les réutiliser
dans la fonction d'affichage.*/
		if (mask >= 0 && mask <= 8){
			if (*oct1 > 256-pow(2,(8-mask))){
				*oct1 = 256-pow(2,(8-mask));
			}
			*oct2 = 0;
			*oct3 = 0;
			*oct4 = 0;
		}
		if (mask >= 9 && mask <= 16){
			if (*oct2 > 256-pow(2,(16-mask))){
				*oct2 = 256-pow(2,(16-mask));
			}
			*oct3 = 0;
			*oct4 = 0;
		}
		if (mask >= 17 && mask <= 24){
			if (*oct3 > 256-pow(2,(24-mask))){
				*oct3 = 256-pow(2,(24-mask));
			}
			*oct4 = 0;
		}
		if (mask >= 25 && mask <= 32){
			if (*oct4 > 256-pow(2,(32-mask))){
				*oct4 = 256-pow(2,(32-mask));
			}
		}
		/*On ne renvoie que l'adresse réseau, pour l'adresse hôte il suffira d'ajouter 1 à oct4.*/
}

/*TRAITEMENT : AFFICHAGE*/
void affichage(char class, char type, long oct1, long oct2, long oct3, long oct4, long mask){
/*On rassemble toutes les variables obtenues pour enfin afficher les résultats.*/

		/*Affichage de la classe.*/		
		switch (class){
			case 'a':
				printf("Adresse pivée de classe A.\n");
			break;
			case 'A':
				printf("Adresse publique de classe A.\n");
			break;
			case 'b':
				printf("Adresse pivée de classe B.\n");
			break;
			case 'B':
				printf("Adresse publique de classe B.\n");
			break;
			case 'c':
				printf("Adresse pivée de classe C.\n");
			break;
			case 'C':
				printf("Adresse publique de classe C.\n");
			break;
			case 'D':
				printf("Adresse de classe D.\n");
			break;
			case 'E':
				printf("Adresse de classe E.\n");
			break;
		}

		/*Affichage du type.*/
		switch (type){
			case 'l':
				printf("C'est une adresse localhost.\n");
			break;
			case 'm':
				printf("C'est une adresse multicast.\n");
			break;
			case 'b':
				printf("C'est une adresse broadcast.\n");
			break;
		}

		/*Affichage de l'adresse réseau.*/
		printf("Adresse réseau = %ld.%ld.%ld.%ld\n", oct1, oct2, oct3, oct4);

		/*Affichage de l'adresse de l'hôte.*/
		printf("Adresse de l'hôte = %ld.%ld.%ld.%ld\n", oct1, oct2, oct3, oct4+1);
}
