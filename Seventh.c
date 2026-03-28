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
Carte initializareCarte(int id, int nrPagini, float pret, const char* editura, const char* numeAutor, char serie) {
	struct Carte c;
	c.id = id;
	c.nrPagini = nrPagini;
	c.pret = pret;
	if (editura != NULL) {
		c.editura = (char*)malloc(sizeof(char) * (strlen(editura) + 1));
		strcpy(c.editura, editura);
	}
	else {
		c.editura = NULL;
	}
	if (numeAutor != NULL) {
		c.numeAutor = (char*)malloc(sizeof(char) * (strlen(numeAutor) + 1));
		strcpy(c.numeAutor, numeAutor);
	}
	else {
		c.numeAutor = NULL;
	}
	c.serie = serie;
	return c;
}

void pushStack(Nod** varf, Carte carte) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = *varf;
	nou->carte = carte;
	*varf = nou;
}
Carte popStack(Nod** varf) {
	if ((*varf) == NULL) {
		return initializareCarte(0, 0, 0, NULL, NULL, ' ');
	}
	Carte c;
	c = (*varf)->carte;
	Nod* aux = *varf;
	*varf = (*varf)->next;
	free(aux);
	return c;
}
int emptyStack(Nod* varf) {
	if (varf == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}
Nod* citireStackCartiDinFisier(const char* numeFisier) {
	Nod* stiva = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			pushStack(&stiva, citireCarteFisier(f));
		}
	}
	fclose(f);
	return stiva;
}
void dezalocareStivaDeCarti(Nod** stiva) {
	while (*stiva) {
		Nod* p = *stiva;
		(*stiva) = (*stiva)->next;
		if (p->carte.editura) {
			free(p->carte.editura);
		}
		if (p->carte.numeAutor) {
			free(p->carte.numeAutor);
		}
		free(p);
	}
}
int size(Nod* stiva) {
	int count = 0;
	Nod* aux = stiva;
	while (aux) {
		count++;
		aux = aux->next;
	}
	return count;
}
void enqueue(Nod** coada, Carte carte) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->carte = carte;
	if (*coada == NULL) {
		(*coada) = nou;
	}
	else {
		Nod* aux = *coada;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}
Carte dequeue(Nod** coada) {
	if ((*coada) == NULL) {
		return initializareCarte(0, 0, 0, NULL, NULL, ' ');
	}
	Carte c;
	c = (*coada)->carte;
	Nod* aux = *coada;
	*coada = (*coada)->next;
	free(aux);
	return c;
}
Nod* citireCoadaDeCartiDinFisier(const char* numeFisier) {
	Nod* coada = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			enqueue(&coada, citireCarteFisier(f));
		}
	}
	fclose(f);
	return coada;
}
void dezalocareCoadaDeCarti(Nod** coada) {
	while (*coada) {
		Nod* p = *coada;
		(*coada) = (*coada)->next;
		if (p->carte.editura) {
			free(p->carte.editura);
		}
		if (p->carte.numeAutor) {
			free(p->carte.numeAutor);
		}
		free(p);
	}
}
Carte getCarteByID(Nod** stiva, int id) {
	if (*stiva == NULL) {
		return initializareCarte(0, 0, 0, NULL, NULL, ' ');
	}
	Nod* aux = NULL;
	Carte carteGasita = initializareCarte(0, 0, 0, NULL, NULL, ' ');
	int gasit = 0;
	while (!emptyStack(*stiva)) {
		Carte c = popStack(stiva);
		if (c.id == id && !gasit) {
			carteGasita = initializareCarte(c.id, c.nrPagini, c.pret,
				c.editura, c.numeAutor, c.serie);
			gasit = 1;
		}
		pushStack(&aux, c);
	}
	while (!emptyStack(aux)) {
		pushStack(stiva, popStack(&aux));
	}
	return carteGasita;
}
float calculeazaPretTotal(Nod* coada) {
	float pretTotal = 0;
	Nod* p = coada;
	while (p) {
		pretTotal += p->carte.pret;
		p = p->next;
	}
	return pretTotal;
}

int main() {
	Nod* stiva = NULL;
	pushStack(&stiva, initializareCarte(1, 100, 35, "Corint", "Creanga", 'A'));
	pushStack(&stiva, initializareCarte(2, 159, 60, "Litera", "Eminescu", 'C'));
	pushStack(&stiva, initializareCarte(3, 203, 45, "Polirom", "Blaga", 'B'));
	Carte c;
	printf("\nParcurgere stiva:\n");
	while (stiva != NULL) {
		c = popStack(&stiva);
		afisareCarte(c);
		free(c.editura);
		free(c.numeAutor);
	}
	printf("\n");
	Nod* coada = NULL;
	enqueue(&coada, initializareCarte(1, 100, 35, "Corint", "Creanga", 'A'));
	enqueue(&coada, initializareCarte(2, 159, 60, "Litera", "Eminescu", 'C'));
	enqueue(&coada, initializareCarte(3, 203, 45, "Polirom", "Blaga", 'B'));
	printf("Parcurgere coada:\n");
	while (coada != NULL) {
		c = dequeue(&coada);
		afisareCarte(c);
		free(c.editura);
		free(c.numeAutor);
	}
	printf("\n");
	printf("\nParcurgere stiva din fisier:\n");
	Nod* stivaFile = citireStackCartiDinFisier("carti.txt");
	while (stivaFile != NULL) {
		c = popStack(&stivaFile);
		afisareCarte(c);
		free(c.editura);
		free(c.numeAutor);
	}
	printf("\n");
	printf("\nParcurgere coada din fisier:\n");
	Nod* coadaFile = citireCoadaDeCartiDinFisier("carti.txt");
	while (coadaFile != NULL) {
		c = dequeue(&coadaFile);
		afisareCarte(c);
		free(c.editura);
		free(c.numeAutor);
	}
	Nod* stivaFile2 = citireStackCartiDinFisier("carti.txt");
	printf("\nCarte gasita dupa id:\n");
	Carte gasita = getCarteByID(&stivaFile2, 7);
	if (gasita.id != 0) {
		afisareCarte(gasita);
		free(gasita.editura);
		free(gasita.numeAutor);
	}
	else {
		printf("Cartea nu a fost gasita!\n");
	}
	printf("\n");
	Nod* coadaFile2 = citireCoadaDeCartiDinFisier("carti.txt");
	printf("Pretul total calculat este %.2f\n",calculeazaPretTotal(coadaFile2));
	
	dezalocareCoadaDeCarti(&coada);
	dezalocareStivaDeCarti(&stiva);
	dezalocareStivaDeCarti(&stivaFile);
	dezalocareStivaDeCarti(&stivaFile2);
	dezalocareCoadaDeCarti(&coadaFile);
	dezalocareCoadaDeCarti(&coadaFile2);
	return 0;
}