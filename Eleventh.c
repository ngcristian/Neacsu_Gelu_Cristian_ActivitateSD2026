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

struct Heap {
	int lungime;
	int nrComp;
	Computer* infoComp;
};
typedef struct Heap Heap;

Computer citireCompFile(FILE* file) {
	Computer compCitit;
	char buffer[100];
	char sep[3] = { ",\n" };
	fgets(buffer, 100, file);
	compCitit.id = atoi(strtok(buffer, sep));
	compCitit.ramMemory = atoi(strtok(NULL, sep));
	compCitit.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	compCitit.model = (char*)malloc(strlen(aux) + 1);
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

Heap initHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrComp = 0;
	heap.infoComp = (Computer*)malloc(sizeof(Computer) * lungime);
	return heap;
}
void filterHeap(Heap heap, int pozNod) {
	int pozMax = pozNod;
	int pozLeft = 2 * pozNod + 1;
	int pozRight = 2 * pozNod + 2;
	if (pozLeft < heap.nrComp && heap.infoComp[pozMax].id < heap.infoComp[pozLeft].id) {
		pozMax = pozLeft;
	}
	if (pozRight < heap.nrComp && heap.infoComp[pozMax].id < heap.infoComp[pozRight].id) {
		pozMax = pozRight;
	}
	if (pozMax != pozNod) {
		Computer aux = heap.infoComp[pozMax];
		heap.infoComp[pozMax] = heap.infoComp[pozNod];
		heap.infoComp[pozNod] = aux;
		if (pozMax <= (heap.nrComp - 2) / 2) {
			filterHeap(heap, pozMax);
		}
	}
}
Heap citireHeapCompFile(const char* fileName) {
	FILE* f = fopen(fileName, "r");
	Heap heap = initHeap(10);
	while (!feof(f)) {
		heap.infoComp[heap.nrComp++] = citireCompFile(f);
	}
	fclose(f);
	for (int i = (heap.nrComp - 2) / 2;i >= 0;i--) {
		filterHeap(heap, i);
	}
	return heap;
}
void showHeap(Heap heap) {
	for (int i = 0;i < heap.nrComp;i++) {
		afisareComp(heap.infoComp[i]);
	}
}
Computer extractComp(Heap* heap) {
	if (heap->nrComp > 0) {
		Computer aux = heap->infoComp[0];
		heap->infoComp[0] = heap->infoComp[heap->nrComp - 1];
		heap->infoComp[heap->nrComp - 1] = aux;
		heap->nrComp--;
		for (int i = (heap->nrComp - 2) / 2;i >= 0;i--) {
			filterHeap(*heap, i);
		}
		return aux;
	}
}
void showHideHeap(Heap heap) {
	for (int i = heap.nrComp;i < heap.lungime;i++) {
		afisareComp(heap.infoComp[i]);
	}
}
void dezalocareHeap(Heap* heap) {
	for (int i = 0;i < heap->lungime;i++) {
		if (heap->infoComp[i].model) {
			free(heap->infoComp[i].model);
		}
	}
	free(heap->infoComp);
	heap->lungime = 0;
	heap->nrComp = 0;
}

int main() {

	Heap heap = citireHeapCompFile("computers_tree.txt");
	showHeap(heap);

	printf("Computers extracted:\n");
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	afisareComp(extractComp(&heap));
	
	printf("Hidden computers:\n");
	showHideHeap(heap);

	dezalocareHeap(&heap);
	return 0;
}
