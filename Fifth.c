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


struct Nod {
	Carte info;
	struct Nod* next;
	struct Nod* prev;
};

typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};

typedef struct ListaDubla ListaDubla;

Carte citireCarteFisier(FILE* file) {
	Carte carteCitita;
	char buffer[100];
	char sep[3] = ",\n";
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

void afisareListaCarti(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareCarte(p->info);
		p = p->next;
	}
}
void afisareListaCartiFinal(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareCarte(p->info);
		p = p->prev;
	}
}

void adaugaCarteInLista(ListaDubla* lista, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->prev = NULL;
	nou->next = lista->first;
	if (lista->first) {
		lista->last->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDCartiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaCarteInLista(&lista, citireCarteFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDCarti(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->info.editura) {
			free(aux->info.editura);
		}
		if (aux->info.numeAutor) {
			free(aux->info.numeAutor);
		}
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.nrNoduri > 0) {
		float sum = 0;
		Nod* p = lista.first;
		while (p) {
			sum += p->info.pret;
			p = p->next;
		}
		return sum / lista.nrNoduri;
	}
	return 0;
}

void stergeCarteDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL) {
		return;
	}
	Nod* p = lista->first;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}
	if (p->prev == NULL) {
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else {
		p->prev->next = p->next;
	}
	if (p->next) {
		p->next->prev = p->prev;
	}
	else {
		lista->last = p->prev;
	}
	if (p->info.editura) {
		free(p->info.editura);
	}
	if (p->info.numeAutor) {
		free(p->info.numeAutor);
	}
	free(p);
	lista->nrNoduri--;
}

char* getNumeAutorCarteScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->info.pret > max->info.pret) {
				max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(max->info.numeAutor) + 1);
		strcpy_s(nume, strlen(max->info.numeAutor) + 1, max->info.numeAutor);
		return nume;
	}
	else {
		return NULL;
	}
}

int main() {

	ListaDubla lista = citireLDCartiDinFisier("carti.txt");
	printf("Afisare de la inceput\n");
	afisareListaCarti(lista);
	printf("###################\n");
	stergeCarteDupaID(&lista, 7);
	stergeCarteDupaID(&lista, 1);
	stergeCarteDupaID(&lista, 10);
	printf("Afisare de la final\n");
	afisareListaCartiFinal(lista);
	printf("###################\n");
	printf("Pret mediu: %5.2f\n",calculeazaPretMediu(lista));
	printf("###################\n");
	char* numeAutor = getNumeAutorCarteScumpa(lista);
	printf("Nume autor carte scumpa: %s", numeAutor);
	if (numeAutor) {
		free(numeAutor);
	}

	dezalocareLDCarti(&lista);
	return 0;
}