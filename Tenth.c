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
	struct Nod* next;
};

typedef struct Nod Nod;

Computer citireCompFisier(FILE* file) {
	Computer compCitit;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	compCitit.id = atoi(strtok(buffer, sep));
	compCitit.ramMemory = atoi(strtok(NULL, sep));
	compCitit.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	compCitit.model = (char*)malloc(strlen(aux) + 1);
	strcpy(compCitit.model, aux);
	compCitit.powerClass = strtok(NULL, sep)[0];
	return compCitit;
}

void afisareCarte(Computer comp) {
	printf("Id: %d\n", comp.id);
	printf("Ram Memory: %d\n", comp.ramMemory);
	printf("Pret: %0.2f\n", comp.pret);
	printf("Model: %s\n", comp.model);
	printf("Power Class: %c\n\n", comp.powerClass);
}

void afisareListaComputers(Nod* lista) {
	while (lista) {
		afisareCarte(lista->info);
		lista = lista->next;
	}
}

void adaugaCompInLista(Nod** lista, Computer compNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = compNou;
	nou->next = NULL;
	if (*lista) {
		Nod* p = *lista;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*lista = nou;
	}
}

void adaugaLaInceputInLista(Nod** lista, Computer compNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = compNou;
	nou->next = *lista;
	*lista = nou;
}

Nod* citireListaComputersDinFisier(const char* numeFisier) {
	Nod* lista = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaCompInLista(&lista, citireCompFisier(f));
		}
	}
	fclose(f);
	return lista;
}

void dezalocareListaComputer(Nod** lista) {
	while (*lista) {
		Nod* p = *lista;
		(*lista) = (*lista)->next;
		if (p->info.model) {
			free(p->info.model);
		}
		free(p);
	}
}

float calculeazaPretMediu(Nod* lista) {
	float sum = 0;
	int count = 0;
	while (lista) {
		sum += lista->info.pret;
		count++;
		lista = lista->next;
	}
	if (count > 0) {
		return sum / count;
	}
	return 0;
}

void stergeComputerDinSeria(Nod** lista, char serieCautata) {
	while ((*lista) && (*lista)->info.powerClass == serieCautata) {
		Nod* aux = *lista;
		(*lista) = aux->next;
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if ((*lista)) {
		Nod* p = *lista;
		while (p) {
			while (p->next && p->next->info.powerClass != serieCautata) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.model) {
					free(aux->info.model);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}
}

float calculeazaPretulCompUnuiModel(Nod* lista, const char* model) {
	float sum = 0;
	while (lista) {
		if (strcmp(lista->info.model, model) == 0) {
			sum += lista->info.pret;
		}
		lista = lista->next;
	}
	return sum;
}

int main() {
	Nod* lista = citireListaComputersDinFisier("computers.txt");
	afisareListaComputers(lista);
	printf("Pret mediu: %.2f\n", calculeazaPretMediu(lista));
	printf("Pret mediu per model: %.2f\n", calculeazaPretulCompUnuiModel(lista, "Asus"));
	stergeComputerDinSeria(&lista, 'A');
	printf("\n###################\n\n");
	afisareListaComputers(lista);
	dezalocareListaComputer(&lista);

	return 0;
}