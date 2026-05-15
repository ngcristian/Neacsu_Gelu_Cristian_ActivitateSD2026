#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct Barca {
	int id;
	int nrMarinari;
	float pret;
	char* nume;
	unsigned char clasa;
};
typedef struct Barca Barca;

struct Heap {
	int lungime;
	int nrElemente;
	Barca* barci;
};
typedef struct Heap Heap;

Barca citireBarcaDinFisier(FILE* file) {
	Barca barca;
	char buffer[100];
	char sep[3] = { ",\n" };
	char* aux;
	fgets(buffer, 100, file);
	aux = strtok(buffer, sep);
	barca.id = atoi(aux);
	barca.nrMarinari = atoi(strtok(NULL, sep));
	barca.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	barca.nume = malloc(strlen(aux) + 1);
	strcpy(barca.nume, aux);
	barca.clasa = strtok(NULL, sep)[0];
	return barca;
}
void afisareBarca(Barca barca) {
	printf("ID: %d\n", barca.id);
	printf("Nr marinari: %d\n", barca.nrMarinari);
	printf("Pret: %2.f\n", barca.pret);
	printf("Nume: %s\n", barca.nume);
	printf("Clasa: %c\n\n", barca.clasa);
}
Heap initHeap(int size) {
	Heap heap;
	heap.lungime = size;
	heap.nrElemente = 0;
	heap.barci = malloc(sizeof(Barca) * size);
	return heap;
}
void filtreazaHeap(Heap heap, int pozNod) {
	int pozSt = 2 * pozNod + 1;
	int pozDr = 2 * pozNod + 2;
	int pozMin = pozNod;
	if (pozSt < heap.nrElemente && heap.barci[pozMin].pret > heap.barci[pozSt].pret) {
		pozMin = pozSt;
	}
	if (pozDr < heap.nrElemente && heap.barci[pozMin].pret > heap.barci[pozDr].pret) {
		pozMin = pozDr;
	}
	if (pozMin != pozNod) {
		Barca aux = heap.barci[pozMin];
		heap.barci[pozMin] = heap.barci[pozNod];
		heap.barci[pozNod] = aux;
		if (pozMin < (heap.nrElemente - 2) / 2) {
			filtreazaHeap(heap, pozMin);
		}
	}
}
Heap citireHeapDinFisier(const char* numeFisier) {
	Heap heap = initHeap(3);
	heap.nrElemente = 0;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			heap.barci[heap.nrElemente++] = citireBarcaDinFisier(f);
		}
	}
	fclose(f);
	for (int i = (heap.nrElemente - 2) / 2;i >= 0;i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareBarca(heap.barci[i]);
	}
}
void afisareHideHeap(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		afisareBarca(heap.barci[i]);
	}
}
Barca extrageBarcaDinHeap(Heap* heap) {
	Barca barca;
	barca.id = -1;
	if (heap->nrElemente > 0) {
		barca = heap->barci[0];
		heap->barci[0] = heap->barci[heap->nrElemente - 1];
		heap->barci[heap->nrElemente - 1] = barca;
		heap->nrElemente--;
		for (int i = (heap->nrElemente - 2) / 2;i >= 0;i--) {
			filtreazaHeap(*heap, i);
		}
	}
	return barca;
}
void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime;i++) {
		free(heap->barci[i].nume);
	}
	free(heap->barci);
	heap->barci = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {

	Heap heap = citireHeapDinFisier("barci.txt");
	afisareHeap(heap);
	printf("Extrageri:\n");
	afisareBarca(extrageBarcaDinHeap(&heap));
	afisareBarca(extrageBarcaDinHeap(&heap));
	printf("Heap vizibil:\n");
	afisareHeap(heap);
	printf("Heap ascuns: \n");
	afisareHideHeap(heap);
	dezalocareHeap(&heap);
	printf("test");
	return 0;
}