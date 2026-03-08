#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* editura;
	char* numeAutor;
	unsigned char serie;
};
typedef struct StructuraCarte Carte;

void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("Nr. Pagini: %d\n", carte.nrPagini);
	printf("Pret: %0.2f\n", carte.pret);
	printf("Editura: %s\n", carte.editura);
	printf("Nume Autor: %s\n", carte.numeAutor);
	printf("Serie: %c\n\n", carte.serie);
}

void afisareVectorCarti(Carte* carti, int nrCarti) {
	for (int i = 0; i < nrCarti; i++) {
		afisareCarte(carti[i]);
	}
}

void adaugaCarteInVector(Carte** carti, int * nrCarti, Carte carteNoua) {
	Carte* aux = (Carte*)malloc(sizeof(Carte) * ((*nrCarti) + 1));
	for (int i = 0; i < *nrCarti; i++) {
		aux[i] = (*carti)[i];
	}
	aux[(*nrCarti)] = carteNoua;
	free(*carti);
	(*carti) = aux;
	(*nrCarti)++;
}

Carte citireCarteFisier(FILE* file) {
	Carte carteCitita;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	carteCitita.id = atoi(strtok(buffer, sep));
	carteCitita.nrPagini = atoi(strtok(NULL, sep));
	carteCitita.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	carteCitita.editura = (char*)malloc(strlen(aux) + 1);
	strcpy(carteCitita.editura, aux);
	aux = strtok(NULL, sep);
	carteCitita.numeAutor = (char*)malloc(strlen(aux) + 1);
	strcpy(carteCitita.numeAutor, aux);
	carteCitita.serie = strtok(NULL, sep)[0];
	return carteCitita;



}

Carte* citireVectorCartiFisier(const char* numeFisier, int* nrCartiCitite) {
	FILE* file = fopen(numeFisier, "r");
	Carte* carti = NULL;
	(*nrCartiCitite) = 0;
	while (!feof(file)) {
		adaugaCarteInVector(&carti, nrCartiCitite, citireCarteFisier(file));
	}
	fclose(file);
	return carti;
}

void dezalocareVectorCarti(Carte** vector, int* nrCarti) {
	for (int i = 0; i < *nrCarti; i++) {
		if ((*vector)[i].editura != NULL) {
			free((*vector)[i].editura);
		}
		if ((*vector)[i].numeAutor != NULL) {
			free((*vector)[i].numeAutor);
;		}
	}
	free(*vector);
	(*vector) = NULL;
	(*nrCarti) = 0;
}

int main() {

	int nrCarti = 0;
	Carte* carti = citireVectorCartiFisier("carti.txt", &nrCarti);
	afisareVectorCarti(carti, nrCarti);
	dezalocareVectorCarti(&carti, &nrCarti);
	return 0;
}