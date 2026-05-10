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

void afisareComp(Computer comp) {
	printf("Id: %d\n", comp.id);
	printf("Ram Memory: %d\n", comp.ramMemory);
	printf("Pret: %0.2f\n", comp.pret);
	printf("Model: %s\n", comp.model);
	printf("Power Class: %c\n\n", comp.powerClass);
}

Computer initComputer(int id, int ramMemory, float pret, const char* model, unsigned char powerClass) {
	Computer computer;
	computer.id = id;
	computer.ramMemory = ramMemory;
	computer.pret = pret;
	computer.model = (char*)malloc(strlen(model) + 1);
	strcpy(computer.model, model);
	computer.powerClass = powerClass;
	return computer;
}
int calculDiferentaInaltime(Nod* cap) {
	if (cap != NULL) {
		return calculeazaInaltimeArbore(cap->st) - calculeazaInaltimeArbore(cap->dr);
	}
	else {
		return 0;
	}
}
void rotireLaStanga(Nod** cap) {
	Nod* aux = (*cap)->dr;
	(*cap)->dr = aux->st;
	aux->st = (*cap);
	(*cap) = aux;
}
void rotireLaDreapta(Nod** cap) {
	Nod* aux = (*cap)->st;
	(*cap)->st = aux->dr;
	aux->dr = (*cap);
	(*cap) = aux;
}
void addCompInArbore(Nod** cap, Computer computerNou) {
	if (*cap == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = computerNou;
		nod->dr = NULL;
		nod->st = NULL;
		*cap = nod;
	}
	else {
		if ((*cap)->info.id > computerNou.id) {
			addCompInArbore(&((*cap)->st), computerNou);
		}
		if ((*cap)->info.id < computerNou.id) {
			addCompInArbore(&((*cap)->dr), computerNou);
		}
	}
	int difInalttimi = calculDiferentaInaltime(*cap);
	if (difInalttimi == 2) {
		if (calculDiferentaInaltime((*cap)->st) == -1) {
			rotireLaStanga(&(*cap)->st);
		}
		rotireLaDreapta(cap);
	}
	if (difInalttimi == -2) {
		if (calculDiferentaInaltime((*cap)->dr) == 1) {
			rotireLaDreapta(&(*cap)->dr);
		}
		rotireLaStanga(cap);
	}
}
void afisareCompDinArbore(Nod* cap) {
	if (cap) {
		afisareCompDinArbore(cap->st);
		afisareComp(cap->info);
		afisareCompDinArbore(cap->dr);
	}
}
void afisarePreordine(Nod* cap) {
	if (cap) {
		afisareComp(cap->info);
		afisarePreordine(cap->st);
		afisarePreordine(cap->dr);
	}
}
void dezalocareArboreDeComps(Nod** cap) {
	if (*cap) {
		dezalocareArboreDeComps(&(*cap)->st);
		dezalocareArboreDeComps(&(*cap)->dr);
		free((*cap)->info.model);
		free(*cap);
		*cap = NULL;
	}
}
Computer getComputerByID(Nod* cap, int id) {
	Computer computer;
	computer.id = -1;
	if (cap) {
		if (cap->info.id == id) {
			computer = cap->info;
			computer.model = malloc(sizeof(char) * (strlen(cap->info.model) + 1));
			strcpy(computer.model, cap->info.model);
		}
		if (id < cap->info.id) {
			computer = getComputerByID(cap->st, id);
		}
		if (id > cap->info.id) {
			computer = getComputerByID(cap->dr, id);
		}
	}
	return computer;
}
int determinaNrNoduri(Nod* cap) {
	if (cap) {
		return determinaNrNoduri(cap->st) + determinaNrNoduri(cap->dr) + 1;
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
float calculeazaPretTotal(Nod* cap) {
	if (cap) {
		return cap->info.pret + calculeazaPretTotal(cap->st) + calculeazaPretTotal(cap->dr);
	}
	return 0;
}
float calculeazaPretCompByModel(Nod* cap, const char* model) {
	if (cap) {
		float suma = calculeazaPretCompByModel(cap->st, model) + calculeazaPretCompByModel(cap->dr, model);
		if (strcmp(cap->info.model, model) == 0) {
			suma += cap->info.pret;
		}
		return suma;
	}
	return 0;
}

int main() {

	Nod* cap = NULL;

	addCompInArbore(&cap, initComputer(1, 8, 600, "Asus", 'A'));
	addCompInArbore(&cap, initComputer(2, 4, 350, "HP", 'C'));
	addCompInArbore(&cap, initComputer(3, 32, 1150, "Lenovo", 'S'));
	addCompInArbore(&cap, initComputer(4, 16, 800, "Mac", 'B'));
	addCompInArbore(&cap, initComputer(5, 16, 750, "Asus", 'S'));
	addCompInArbore(&cap, initComputer(6, 8, 550, "Dell", 'A'));
	afisarePreordine(cap);

	printf("Computer cautat: ");
	afisareComp(getComputerByID(cap, 3));

	printf("Numar nodur: %d\n", determinaNrNoduri(cap));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(cap));
	printf("Pret toal: %.2f\n", calculeazaPretTotal(cap));
	printf("Suma prreturilor computerului unui model: %.2f\n", calculeazaPretCompByModel(cap, "Asus"));

	dezalocareArboreDeComps(&cap);
	return 0;
}