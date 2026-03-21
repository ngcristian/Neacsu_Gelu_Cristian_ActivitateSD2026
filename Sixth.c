#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct Carte {
	int id;
	int nrPagini;
	float pret;
	char* editura;
	char* numeAutor;
	unsigned char serie;
};
typedef struct Carte Carte;

struct Nod {
	Carte carte;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** tabela;
};
typedef struct HashTable HashTable;

Carte citireCarteFisier(FILE* file) {
	Carte carteCitita;
	char buffer[100];
	char sep[3] = { ",\n" };
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
void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("Nr. Pagini: %d\n", carte.nrPagini);
	printf("Pret: %0.2f\n", carte.pret);
	printf("Editura: %s\n", carte.editura);
	printf("Nume Autor: %s\n", carte.numeAutor);
	printf("Serie: %c\n\n", carte.serie);
}

void afisareListaCarti(Nod* first) {
	while (first) {
		afisareCarte(first->carte);
		first = first->next;
	}
}
void adaugaCarteInLista(Nod* first, Carte carteNoua) {
	Nod* p = first;
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->carte = carteNoua;
	nou->next = NULL;
	p->next = nou;
}
HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}
int calculeazaHash(const char* nume, int dimensiune) {
	int sum = 0;
	for (int i = 0; i < strlen(nume); i++) {
		sum += nume[i];
	}
	return sum % dimensiune;
}
void inserareCarteInTabela(HashTable hash, Carte carteNoua) {
	int pozitie = calculeazaHash(carteNoua.numeAutor, hash.dim);
	if (hash.tabela[pozitie] == NULL) {
		hash.tabela[pozitie] = (Nod*)malloc(sizeof(Nod));
		hash.tabela[pozitie]->carte = carteNoua;
		hash.tabela[pozitie]->next = NULL;
	}
	else {
		adaugaCarteInLista(hash.tabela[pozitie], carteNoua);
	}
}
HashTable citireCartiDinFisier(const char* numeFisier, int dimensiune) {
	HashTable hash = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Carte carte = citireCarteFisier(f);
		inserareCarteInTabela(hash, carte);
	}
	fclose(f);
	return hash;
}
void afisarreTabelaDeCarti(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			printf("Cartile de pe pozitie %d sunt:\n", i);
			afisareListaCarti(ht.tabela[i]);
		}
		else {
			printf("\nPe pozitia %d nu avem carti.\n", i);
		}
	}
}
void dezalocareListaCarti(Nod** first) {
	Nod* p = *first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->carte.editura != NULL) {
			free(aux->carte.editura);
		}
		if (aux->carte.numeAutor != NULL) {
			free(aux->carte.numeAutor);
		}
		free(aux);
	}
	*first = NULL;
}
void dezalocareTabelaCarti(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dezalocareListaCarti(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}
float calculeazaMediePretLista(Nod* first) {
	float sum = 0;
	int nrElemente = 0;
	while (first) {
		sum += first->carte.pret;
		nrElemente++;
		first = first->next;
	}
	if (nrElemente > 0) {
		return sum / nrElemente;
	}
	else {
		return 0;
	}
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturi = NULL;
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			(* nrClustere)++;
		}
	}
	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			preturi[contor] = calculeazaMediePretLista(ht.tabela[i]);
			contor++;
		}
	}
	return preturi;
}
Carte getCarteDinLista(Nod* first, const char* nume) {
	Carte carte;
	carte.id = -1;
	while (first && strcmp(first->carte.numeAutor, nume) != 0) {
		first = first->next;
	}
	if (first) {
		carte = first->carte;
		carte.editura = (char*)malloc(strlen(first->carte.editura) + 1);
		strcpy(carte.editura, first->carte.editura);
		carte.numeAutor = (char*)malloc(strlen(first->carte.numeAutor) + 1);
		strcpy(carte.numeAutor, first->carte.numeAutor);
	}
	return carte;
}

Carte getCarteDupaNumeAutor(HashTable ht, const char* numeCautat) {
	Carte carte;
	carte.id = -1;
	int poz = calculeazaHash(numeCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim) {
		return getCarteDinLista(ht.tabela[poz], numeCautat);
	}
	return carte;
}

int main() {
	
	HashTable ht = citireCartiDinFisier("carti.txt", 10);
	afisarreTabelaDeCarti(ht);
	int nrClustere = 0;
	float* preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("Preturile medii per clustere sunt: \n");
	for (int i = 0; i < nrClustere; i++) {
		printf("%.2f ", preturi[i]);
	}
	printf("\n");

	Carte carte = getCarteDupaNumeAutor(ht, "Patrascu Georgee");
	if (carte.id != -1) {
		printf("Cartea cautata:\n");
		afisareCarte(carte);
		if (carte.editura != NULL) {
			free(carte.editura);
		}
		if (carte.numeAutor != NULL) {
			free(carte.numeAutor);
		}
	}
	else {
		printf("Cartea nu a fost gasita, nu exista autorul");
	}
	dezalocareTabelaCarti(&ht);
	return 0;
}