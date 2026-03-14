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
};

typedef struct Nod Nod;

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

void afisareListaCarti(Nod* lista) {
	while (lista) {
		afisareCarte(lista->info);
		lista = lista->next;
	}
}

void adaugaCarteInLista(Nod** lista, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
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

void adaugaLaInceputInLista(Nod** lista, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = *lista;
	*lista = nou;
}

Nod* citireListaCartiDinFisier(const char* numeFisier) {
	Nod* lista = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaCarteInLista(&lista, citireCarteFisier(f));
		}
	}
	fclose(f);
	return lista;
}

void dezalocareListaCarte(Nod** lista) {
	while (*lista) {
		Nod* p = *lista;
		(*lista) = (*lista)->next;
		if (p->info.editura) {
			free(p->info.editura);
		}
		if (p->info.numeAutor) {
			free(p->info.numeAutor);
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

void stergeCartiDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulCartilorUnuiAutor(Nod* lista, const char* numeAutor) {
	float sum = 0;
	while (lista) {
		if (strcmp(lista->info.numeAutor, numeAutor) == 0) {
			sum += lista->info.pret;
		}
		lista = lista->next;
	}
	return sum;
}

int main() {
	Nod* lista = citireListaCartiDinFisier("carti.txt");
	afisareListaCarti(lista);
	printf("Pret mediu: %.2f\n", calculeazaPretMediu(lista));
	printf("Pret mediu per autor: %.2f\n", calculeazaPretulCartilorUnuiAutor(lista,"Stanescu"));


	dezalocareListaCarte(&lista);

	return 0;
}