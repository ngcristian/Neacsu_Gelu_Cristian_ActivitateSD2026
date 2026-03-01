#include<stdio.h>
#include<malloc.h>

typedef struct Bilet Bilet;

struct Bilet {
	int id;
	int serie;
	char* destinatie;
	float pret;
	char clasa;
};
struct Bilet initializare(int id, int serie, const char* destinatie, float pret, char clasa) {
	struct Bilet b;
	b.id = id;
	b.serie = serie;
	b.destinatie = (char*)malloc(sizeof(char) * (strlen(destinatie) + 1));
	strcpy(b.destinatie, destinatie);
	b.pret = pret;
	b.clasa = clasa;
	return b;
}

struct Bilet copiaza(Bilet b) {
	return initializare(b.id, b.serie, b.destinatie, b.pret, b.clasa);
}

void afisare(struct Bilet b) {
	printf("%d. Biletul cu seria %d, clasa %c, spre destinatia %s, costa %5.2f lei.\n",
		b.id, b.serie, b.clasa, b.destinatie, b.pret);
}

void afisareVector(struct Bilet* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(*vector);
		vector++;
	}
}

struct Bilet* copiazaPrimeleNElemente(struct Bilet* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate < nrElemente) {
		struct Bilet* vectorNou = malloc(sizeof(Bilet) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}
		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Bilet** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		if ((*vector)[i].destinatie != NULL) {
			free((*vector)[i].destinatie);
		}
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaAnumiteElemente(struct Bilet* vector, char nrElemente, float pragPret, struct Bilet** vectorNou, int* dimensiune) {

	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret < pragPret) {
			(*dimensiune)++;
		}
	}
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	*vectorNou = malloc(sizeof(Bilet) * (*dimensiune));
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret < pragPret) {
			(*vectorNou)[*dimensiune] = copiaza(vector[i]);
			(*dimensiune)++;
		}
	}
}

struct Bilet getPrimulElementConditionat(struct Bilet* vector, int nrElemente, const char* destinatieCautata) {
	struct Bilet b;
	b.id = -1;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(destinatieCautata, vector[i].destinatie) == 0) {
			b = copiaza(vector[i]);
		}
	}
	return b;
}
	


int main() {

	struct Bilet bilet1 = initializare(1, 100, "Bucuresti", 50.00, 'B');
	afisare(bilet1);

	int nrBilete = 3;
	Bilet* vector = (Bilet*)malloc(sizeof(Bilet) * nrBilete);
	vector[0] = initializare(2, 200, "Cluj", 80.00, 'B');
	vector[1] = initializare(3, 300, "Constanta", 100.50, 'A');
	vector[2] = initializare(4, 400, "Timisoara", 50.00, 'B');

	afisareVector(vector, nrBilete);
	printf("\n\n");

	int nrBileteCopiate = 2;
	printf("Primele %d elemente\n", nrBileteCopiate);
	Bilet* partial = copiazaPrimeleNElemente(vector, nrBilete, nrBileteCopiate);
	afisareVector(partial, nrBileteCopiate);
	dezalocare(&partial, &nrBileteCopiate);
	printf("\n\n");

	Bilet* bileteScumpe = NULL;
	int nrBileteScumpe = 0;
	copiazaAnumiteElemente(vector, nrBilete, 90.00, &bileteScumpe, &nrBileteScumpe);
	printf("Bilete ieftine:\n");
	afisareVector(bileteScumpe, nrBileteScumpe);
	dezalocare(&bileteScumpe, &bileteScumpe);
	printf("\n\n");

	Bilet biletCautat = getPrimulElementConditionat(vector, nrBilete, "Constanta");
	printf("Bilet cautat:\n");
	afisare(biletCautat);
	if (biletCautat.id != -1) {
		free(biletCautat.destinatie);
	}
	free(bilet1.destinatie);
	dezalocare(&vector, &nrBilete);

	return 0;
}