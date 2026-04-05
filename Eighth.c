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

struct Heap {
	int lungime;
	Carte* vector;
	int nrCarti;
};
typedef struct Heap Heap;


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
Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrCarti = 0;
	heap.vector = (Carte*)malloc(sizeof(Carte) * lungime);
	return heap;
}
void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozLeft = 2 * pozitieNod + 1;
	int pozRight = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozLeft < heap.nrCarti && heap.vector[pozMax].nrPagini < heap.vector[pozLeft].nrPagini) {
		pozMax = pozLeft;
	}
	if (pozRight < heap.nrCarti && heap.vector[pozMax].nrPagini < heap.vector[pozRight].nrPagini) {
		pozMax = pozRight;
	}
	if (pozMax != pozitieNod) {
		Carte aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrCarti - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}
Heap citireHeapDeCartiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(file)) {
		heap.vector[heap.nrCarti++] = citireCarteFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrCarti - 2) / 2;i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrCarti;i++) {
		afisareCarte(heap.vector[i]);
	}
}
Carte extrageCarte(Heap* heap) {
	if (heap->nrCarti > 0) {
		Carte aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrCarti - 1];
		heap->vector[heap->nrCarti - 1] = aux;
		heap->nrCarti--;
		for (int i = (heap->nrCarti - 2) / 2;i >= 0;i--) {
			filtreazaHeap(*heap, i);
		}
		return aux; //shallow
	}
}
void afisareHeapHide(Heap heap) {
	for (int i = heap.nrCarti;i < heap.lungime; i++) {
		afisareCarte(heap.vector[i]);
	}
}
void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime;i++) {
		if (heap->vector[i].editura) {
			free(heap->vector[i].editura);
		}
		if (heap->vector[i].numeAutor) {
			free(heap->vector[i].numeAutor);
		}
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrCarti = 0;
}

int main() {
	Heap heap = citireHeapDeCartiDinFisier("carti_tree.txt");
	afisareHeap(heap);
	
	printf("Carti extrase:\n");
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));
	afisareCarte(extrageCarte(&heap));

	printf("Carti ascunse:\n");
	afisareHeapHide(heap);

	dezalocareHeap(&heap);

	return 0;
}