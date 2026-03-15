#include<stdio.h>
#include<stdlib.h>

struct Carte {
	int id;
	int nrPagini;
	char* numeAutor;
	float pret;
	char varianta;
};

struct Carte initializare(int id, int nrPagini, const char* numeAutor, float pret, char varianta) {
	struct Carte c;
	c.id = id;
	c.nrPagini = nrPagini;
	c.numeAutor = (char*)malloc(sizeof(char) * (strlen(numeAutor) + 1));
	strcpy(c.numeAutor, numeAutor);
	c.pret = pret;
	c.varianta = varianta;
	return c;
}

void afisare(struct Carte c) {
	if (c.numeAutor != NULL) {
		printf("%d. Cartea scrise de %s, are %d pagini, varianta %c, cu pretul de %5.2f lei.\n", c.id, c.numeAutor, c.nrPagini, c.varianta, c.pret);
	}
	else {
		printf("%d. Cartea are %d pagini, varianta %c, cu pretul de %5.2f lei.\n", c.id, c.nrPagini, c.varianta, c.pret);
	}
}

void modificarePret(struct Carte* c, float pretNou) {
	if (pretNou > 0) {
		c->pret = pretNou;
	}
}

void dezalocare(struct Carte *c) {
	if (c->numeAutor != NULL) {
		free(c->numeAutor);
		c->numeAutor = NULL;
	}
}

int main() {
	struct Carte c;

	c = initializare(1, 120, "Creanga", 50.00, '1');

	afisare(c);

	modificarePret(&c, 45.00);
	afisare(c);
	dezalocare(& c);
	afisare(c);
	return 0;
}