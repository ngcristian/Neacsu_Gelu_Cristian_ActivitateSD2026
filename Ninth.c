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

typedef struct NodPrin NodPrin;
typedef struct NodSec NodSec;

struct NodPrin {
	Carte info;
	NodSec* vecini;
	struct NodPrin* next;
};

struct NodSec {
	NodPrin* nodInfo;
	NodSec* next;
};


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

void inserarePrinList(NodPrin** cap, Carte carte) {
	NodPrin* nou = (NodPrin*)malloc(sizeof(NodPrin));
	nou->info = carte;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*cap) {
		NodPrin* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}
void inserareSecList(NodSec** cap, NodPrin* nodInfo) {
	NodSec* nou = (NodSec*)malloc(sizeof(NodSec));
	nou->nodInfo = nodInfo;
	nou->next = *cap;
	*cap = nou;
}

NodPrin* cautaNodDupaID(NodPrin* cap, int idCautat) {
	while (cap && cap->info.id != idCautat) {
		cap = cap->next;
	}
	return cap;
}
void inserareMuchie(NodPrin* graf, int idStart, int idStop) {
	NodPrin* nodStart = cautaNodDupaID(graf, idStart);
	NodPrin* nodStop = cautaNodDupaID(graf, idStop);
	if (nodStart && nodStop) {
		inserareSecList(&(nodStart->vecini), nodStop);
		inserareSecList(&(nodStop->vecini), nodStart);
	}
}
NodPrin* citireNodurCartiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodPrin* graf = NULL;
	if (file) {
		while (!feof(file)) {
			inserarePrinList(&graf, citireCarteFisier(file));
		}
		fclose(file);
	}
	return graf;
}
void citireMuchiiDinFisier(NodPrin* graf, const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	while (file && !feof(file)) {
		int idStart = 0;
		int idStop = 0;
		fscanf(file, "%d %d", &idStart, &idStop);
		inserareMuchie(graf, idStart, idStop);
	}
	fclose(file);
}
void dezalocareSecList(NodSec** cap) {
	while (*cap) {
		NodSec* temp = (*cap);
		(*cap) = (*cap)->next;
		free(temp);
	}
}
void dezalocareNoduriGraf(NodPrin** graf) {
	while (*graf) {
		NodPrin* temp = (*graf);
		(*graf) = (*graf)->next;
		dezalocareSecList(&(temp->vecini));
		if (temp->info.editura != NULL) {
			free(temp->info.editura);
		}
		if (temp->info.numeAutor != NULL) {
			free(temp->info.numeAutor);
		}
		free(temp);
	}
}
typedef struct NodStiva NodStiva;
struct NodStiva {
	int id;
	NodStiva* next;
};
void push(NodStiva** cap, int id) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = *cap;
	*cap = nou;
}
int pop(NodStiva** cap) {
	if (*cap) {
		NodStiva* temp = *cap;
		*cap = (*cap)->next;
		int id = temp->id;
		free(temp);
		return id;
	}
	else {
		return -1;
	}
}
int calzuleazaNrNoduriGraf(NodPrin* listPrin) {
	int count = 0;
	while (listPrin) {
		count++;
		listPrin = listPrin->next;
	}
	return count;
}

void afisareGrafInAdancime(NodPrin* graf, int idPlecare) {
	int nrNoduri = calzuleazaNrNoduriGraf(graf);
	unsigned char* vizitate = (unsigned char*)malloc(sizeof(unsigned char) * nrNoduri);
	for (int i = 0; i < nrNoduri;i++) {
		vizitate[i] = 0;
	}
	NodStiva* stiva = NULL;
	vizitate[idPlecare - 1] = 1;
	push(&stiva, idPlecare);
	while (stiva) {
		int idNod = pop(&stiva);
		NodPrin* nodCurent = cautaNodDupaID(graf, idNod);
		afisareCarte(nodCurent->info);
		NodSec* p = nodCurent->vecini;
		while (p) {
			if (vizitate[p->nodInfo->info.id - 1] == 0) {
				push(&stiva, p->nodInfo->info.id);
				vizitate[p->nodInfo->info.id - 1] = 1;
			}
			p = p->next;
		}
	}
}
typedef struct NodCoada NodCoada;
struct NodCoada {
	int id;
	NodCoada* next;
};
void enqueue(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = NULL;
	if (*cap) {
		NodCoada* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else{
		*cap = nou;
	}
}
int dequeue(NodCoada** cap) {
	if (*cap) {
		NodCoada* temp = *cap;
		*cap = (*cap)->next;
		int id = temp->id;
		free(temp);
		return id;
	}
	else {
		return -1;
	}
}
void afisareGraficInLatime(NodPrin* graf, int idPlecare) {
	int nrNoduri = calzuleazaNrNoduriGraf(graf);
	unsigned char* vizitate = (unsigned char*)malloc(sizeof(unsigned char) * nrNoduri);
	for (int i = 0; i < nrNoduri;i++) {
		vizitate[i] = 0;
	}
	NodCoada* coada = NULL;
	vizitate[idPlecare - 1] = 1;
	enqueue(&coada, idPlecare);
	while (coada) {
		int idNod = dequeue(&coada);
		NodPrin* nodCurent = cautaNodDupaID(graf, idNod);
		afisareCarte(nodCurent->info);
		NodSec* p = nodCurent->vecini;
		while (p) {
			if (vizitate[p->nodInfo->info.id - 1] == 0) {
				enqueue(&coada, p->nodInfo->info.id);
				vizitate[p->nodInfo->info.id - 1] = 1;
			}
			p = p->next;
		}
	}
}

int main() {

	NodPrin* graf = NULL;
	graf = citireNodurCartiDinFisier("carti.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");

	afisareGrafInAdancime(graf, 1);
	printf("==============================\n");
	afisareGraficInLatime(graf, 1);

	dezalocareNoduriGraf(&graf);
	return 0;
}