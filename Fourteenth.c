#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Barca Barca;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Barca {
	char* nume;
	int nrMarinari;
	float pret;
};
struct Nod {
	Barca info;
	Nod* next;
};
struct HashTable {
	int dimensiune;
	Nod** vector;
};

Barca initBarca(const char* nume, int nrMarinari, float pret) {
	Barca barca;
	barca.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(barca.nume, nume);
	barca.nrMarinari = nrMarinari;
	barca.pret = pret;
	return barca;
}
void afisareBarca(Barca barca) {
	printf("Nume: %s\n", barca.nume);
	printf("Nr marinari: %d\n", barca.nrMarinari);
	printf("Pret: %.2f\n\n", barca.pret);
}
void afisareListaBarci(Nod* cap) {
	while (cap) {
		afisareBarca(cap->info);
		cap = cap->next;
	}
}
void inserareLaSfarsit(Nod** cap, Barca barca) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = barca;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}
HashTable initHT(int size) {
	HashTable tabela;
	tabela.dimensiune = size;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * size);
	for (int i = 0; i < size;i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}
int initHash(int dim, int nrMarinari) {
	return nrMarinari % dim;
}
void inserareTabela(HashTable tabela, Barca barca) {
	if (tabela.dimensiune > 0) {
		int pozitie = initHash(tabela.dimensiune, barca.nrMarinari);
		if (pozitie >= 0 && pozitie < tabela.dimensiune) {
			inserareLaSfarsit(&(tabela.vector[pozitie]), barca);
		}
	}
}
void afisareTabela(HashTable tabela) {
	for (int i = 0;i < tabela.dimensiune;i++) {
		printf("Pozitia: %d\n", i);
		afisareListaBarci(tabela.vector[i]);
	}
}
void dezalocareLista(Nod** cap) {
	while ((*cap)) {
		free((*cap)->info.nume);
		Nod* copie = *cap;
		*cap = (*cap)->next;
		free(copie);
	}
}
void dezalocareTabela(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		dezalocareLista(&(tabela.vector[i]));
	}
	free(tabela.vector);
}

int main() {
	HashTable tabela = initHT(3);
	inserareTabela(tabela, initBarca("ABC", 10, 25));
	inserareTabela(tabela, initBarca("BCD", 5, 35));
	inserareTabela(tabela, initBarca("DCS", 15, 15));

	afisareTabela(tabela);
	dezalocareTabela(tabela);

	
	printf("test");

	return 0;
}
