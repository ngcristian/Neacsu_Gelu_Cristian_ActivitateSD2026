#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Barca {
	int id;
	int nrMarinari;
	float pret;
	char* nume;
	unsigned char clasa;
};
typedef struct Barca Barca;

struct Nod {
	Barca info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Barca citireBarcaDinFisier(FILE* file) {
	Barca barca;
	char buffer[100];
	char sep[3] = { ",\n" };
	char* aux;
	fgets(buffer, 100, file);
	aux = strtok(buffer, sep);
	barca.id = atoi(aux);
	barca.nrMarinari = atoi(strtok(NULL, sep));
	barca.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	barca.nume = malloc(strlen(aux) + 1);
	strcpy(barca.nume, aux);
	barca.clasa = strtok(NULL, sep)[0];
	return barca;
}
void afisareBarca(Barca barca) {
	printf("ID: %d\n", barca.id);
	printf("Nr marinari: %d\n", barca.nrMarinari);
	printf("Pret: %2.f\n", barca.pret);
	printf("Nume: %s\n", barca.nume);
	printf("Clasa: %c\n\n", barca.clasa);
}
void addBarcaInArbore(Nod** rad, Barca barca) {
	if (*rad == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = barca;
		nou->st = NULL;
		nou->dr = NULL;
		*rad = nou;
	}
	else {
		if ((*rad)->info.id > barca.id) {
			addBarcaInArbore(&((*rad)->st), barca);
		}
		if ((*rad)->info.id < barca.id) {
			addBarcaInArbore(&((*rad)->dr), barca);
		}
	}
}
Nod* citireArboreDinFile(const char* numeFile) {
	Nod* rad = NULL;
	FILE* f = fopen(numeFile, "r");
	if (f) {
		while (!feof(f)) {
			Barca barca = citireBarcaDinFisier(f);
			addBarcaInArbore(&rad, barca);
		}
	}
	fclose(f);
	return rad;
}
void afisareArboreBarci(Nod* rad) {
	if (rad) {
		afisareArboreBarci(rad->st);
		afisareBarca(rad->info);
		afisareArboreBarci(rad->dr);
	}
}
void afisarePreordineArboreBarci(Nod* rad) {
	if (rad) {
		afisareBarca(rad->info);
		afisareArboreBarci(rad->st);
		afisareArboreBarci(rad->dr);
	}
}
void dezalocareArbore(Nod** rad) {
	if (*rad) {
		dezalocareArbore(&(*rad)->st);
		dezalocareArbore(&(*rad)->dr);
		free((*rad)->info.nume);
		free(*rad);
		*rad = NULL;
	}
}

int main(){

	Nod* rad = citireArboreDinFile("barci_tree.txt");
	afisarePreordineArboreBarci(rad);
	dezalocareArbore(&rad);

	printf("test");
	return 0;
}
