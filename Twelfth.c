#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraComputer {
	int id;
	int ramMemory;
	float pret;
	char* model;
	unsigned char powerClass;
};
typedef struct StructuraComputer Computer;

struct Nod {
	Computer info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Computer citireCompDinFisier(FILE* file) {
	Computer compCitit;
	char buffer[100];
	char sep[3] = { ",\n"};
	fgets(buffer, 100, file);
	char* aux;
	aux = strtok(buffer, sep);
	compCitit.id = atoi(aux);
	compCitit.ramMemory = atoi(strtok(NULL, sep));
	compCitit.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	compCitit.model = malloc(strlen(aux) + 1);
	strcpy(compCitit.model, aux);
	compCitit.powerClass = strtok(NULL, sep)[0];
	return compCitit;
}
void afisareComp(Computer comp) {
	printf("Id: %d\n", comp.id);
	printf("Ram Memory: %d\n", comp.ramMemory);
	printf("Pret: %0.2f\n", comp.pret);
	printf("Model: %s\n", comp.model);
	printf("Power Class: %c\n\n", comp.powerClass);
}

void adaugaComputerInArbore(Nod** cap, Computer compNou) {
	if (*cap == NULL) {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = compNou;
		nou->st = NULL;
		nou->dr = NULL;
		*cap = nou;
	}
	else {
		if ((*cap)->info.id > compNou.id) {
			adaugaComputerInArbore(&((*cap)->st), compNou);
		}
		if ((*cap)->info.id < compNou.id) {
			adaugaComputerInArbore(&((*cap)->dr), compNou);
		}
	}
}
Nod* citireArboreDeCompDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Computer comp = citireCompDinFisier(f);
			adaugaComputerInArbore(&cap, comp);
		}
	}
	fclose(f);
	return cap;
}
void afisareCompsDinArbore(Nod* cap) {
	if (cap) {
		afisareCompsDinArbore(cap->st);
		afisareComp(cap->info);
		afisareCompsDinArbore(cap->dr);
	}
}
void afisarePreordine(Nod* cap) {
	if (cap) {
		afisareComp(cap->info);
		afisareCompsDinArbore(cap->st);
		afisareCompsDinArbore(cap->dr);
	}
}
void dezalocareArboreDeComps(Nod** cap) {
	if (*cap) {
		dezalocareArboreDeComps(&(*cap)->st);
		dezalocareArboreDeComps(&(*cap)->dr);
		if ((*cap)->info.model) {
			free((*cap)->info.model);
		}
		free(*cap);
		*cap = NULL;
	}
}
Computer getComputerByID(Nod* cap, int id) {
	Computer comp;
	comp.id = -1;
	if (cap) {
		if (cap->info.id == id) {
			comp = cap->info;
			comp.model = malloc(sizeof(char) * strlen(cap->info.model) + 1);
			strcpy(comp.model, cap->info.model);
		}
		if (id < cap->info.id) {
			comp = getComputerByID(cap->st, id);
		}
		if (id > cap->info.id) {
			comp = getComputerByID(cap->dr, id);
		}
	}
	return comp;
}

int determinaNumarNoduri(Nod* cap) {
	if (cap) {
		return determinaNumarNoduri(cap->st) + determinaNumarNoduri(cap->dr) + 1;
	}
	return 0;
}
int maxim(int a, int b) {
	return (a > b ? a : b);
}
int calculeazaInaltimeArbore(Nod* cap) {
	if (cap) {
		return maxim(calculeazaInaltimeArbore(cap->st), calculeazaInaltimeArbore(cap->dr)) + 1;
	}
	return 0;
}
float calculeazaPreTotal(Nod* cap) {
	if (cap) {
		return cap->info.pret + calculeazaPreTotal(cap->st) + calculeazaPreTotal(cap->dr);
	}
	return 0;
}
float calculeazaPretulCompPerModel(Nod* cap, const char* modelComp) {
	if (cap) {
		float suma = calculeazaPretulCompPerModel(cap->st, modelComp) + calculeazaPretulCompPerModel(cap->dr, modelComp);
		if (strcmp(cap->info.model, modelComp) == 0) {
			suma += cap->info.pret;
		}
		return suma;
	}
	return 0;
}

int main() {
	
	Nod* nod = citireArboreDeCompDinFisier("computers_tree.txt");
	afisarePreordine(nod);

	printf("Computer cautate:");
	afisareComp(getComputerByID(nod, 7));

	printf("Numar noduri: %d\n", determinaNumarNoduri(nod));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(nod));
	printf("Pret total: %.2f\n", calculeazaPreTotal(nod));
	printf("Suma preturilor computers unui model: %.2f\n", calculeazaPretulCompPerModel(nod, "Mac"));

	dezalocareArboreDeComps(&nod);
	return 0;
}