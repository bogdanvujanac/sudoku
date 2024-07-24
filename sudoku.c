#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define NUM 9
#define NICK 51

//definicije funkcija
int** alociraj();
void ucitaj(int **matrica, int** resena_matrica, int tezina);
FILE* otvori(int tezina);
int ucitaj_tezinu();
void resavanje(int** postavka, int** resen, int* greska);
void dealociraj(int** matrica);
void ispis(int** matrica);
int broj_praznih_polja(int** matrica);
void igranje(int** postavka, int** resen);

typedef struct{
	char nickname[NICK];
	long vreme;
}Igraci;


int main(){
	int **postavka;
	int **resen;
	int tezina;
	struct timeval start, end;
	Igraci igrac;
	
	
	tezina=ucitaj_tezinu(); //ucitavam u funkciji tezinu 
	
	
	postavka=alociraj();
	resen=alociraj();
	
	
	ucitaj(postavka, resen, tezina);
	//pocetak merenja vremena
	gettimeofday(&start, NULL);
	igranje(postavka, resen);
	//kraj merenja vremena
	gettimeofday(&end, NULL);
	
	//razlika izmedju kraja i pocetka je vreme provedeno igrajuci
	igrac.vreme=end.tv_sec-start.tv_sec;
	
	printf("Unesite svoj nickname: ");
	scanf("%s", igrac.nickname);
	
	printf("Igac: %s \n Vreme: %ld \n", igrac.nickname, igrac.vreme);
	
	//dealociranje matrica
	dealociraj(postavka);
	dealociraj(resen);
	
	return 0;
}


int ucitaj_tezinu(){
	int tezina;
	do{
		printf("Unesite tezinu(1 za lak, 2 za srednji, 3 za tezak): ");
		scanf("%d", &tezina);
	}
	while(tezina<1 || tezina>3);
	
	return tezina;
}

FILE* otvori(int tezina){
	char naziv_fajla[21];
	if(tezina==1){
		strcpy(naziv_fajla, "lak.txt");
	}
	else if(tezina==2){
		strcpy(naziv_fajla, "srednji.txt");
	}
	else{
		strcpy(naziv_fajla, "tezak.txt");
	}
	
	
	FILE* datoteka = fopen(naziv_fajla, "r");

   	if (datoteka == NULL) {
	   	printf("Greska pri otvaranju datoteke");
    	exit(EXIT_FAILURE);
   	}
	
	return datoteka;
}

int** alociraj(){
	int** matrica;
	
	matrica=(int**)malloc(NUM*sizeof(int*));
	if(matrica==NULL){
		printf("Nije dobro alocirana.\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=0; i<NUM; i++){
		matrica[i]=(int*)malloc(NUM*sizeof(int));
		if(matrica[i]==NULL){
			printf("Nije dobro alocirana.\n");
			exit(EXIT_FAILURE);	
		}
	}
	return matrica;
}

void ucitaj(int** matrica, int** resena_matrica, int tezina){
	
	FILE *in;
	in=otvori(tezina);
	
	for(int i=0; i<NUM; i++)
		for(int j=0; j<NUM; j++)
			fscanf(in, "%d", &matrica[i][j]);
			
	for(int i=0; i<NUM; i++)
		for(int j=0; j<NUM; j++)
			fscanf(in, "%d", &resena_matrica[i][j]);
			
	fclose(in);		
	
}

void dealociraj(int** matrica){
	
	for(int i=0; i<9; i++)
		free(matrica[i]);
	
	free(matrica);
	
}

void ispis(int** matrica){

	for(int i=0; i<NUM; i++){
		if(i%3==0)
			printf("\n");
		for(int j=0; j<NUM; j++){
			if(j%3==0)
				printf(" ");
			if(matrica[i][j]==0)
				printf("  ");
			else	
				printf("%d ", matrica[i][j]);
		}	
		
		printf("\n");
	}
	
	
}

int broj_praznih_polja(int** matrica){
	
	int ind=0;
	
	for(int i=0; i<NUM; i++){
		for(int j=0; j<NUM; j++){
			if(matrica[i][j]==0)
				ind++;
		}
	}
	
	return ind;
}

void igranje(int** postavka, int** resen){
	int greska=0;	
	int ind=0;

	printf("Postavka sudoku nivoa: \n");	
	ispis(postavka);
	printf("#########################################################");
	printf("\n");
	printf("\n");
	
	
	
	do{
		if(greska==3){
			printf("Imate maksimalan broj gresaka. Izgubili ste!\n");
			
			break;	
		}
		
		
		ind=broj_praznih_polja(postavka);
		
		if(ind!=0)
			resavanje(postavka, resen, &greska);
		ispis(postavka);
		printf("#########################################################\n\n");
	}while(ind!=0);
	
	if(ind==0){
		printf("BRAVO! POBEDILI STE!\n");	
		
	}
	
}

void resavanje(int** postavka, int** resen, int* greska){
	
	int vrsta, kolona, broj;
	
	do{
		printf("U koju vrstu i kolonu zelite da unesete broj (1-9)?\n");
		scanf("%d %d", &vrsta, &kolona);
	
		if(vrsta<1 || vrsta>9 || kolona<0 || kolona>9)
			printf("Unesite parametre od 1 do 9!");
	}while(vrsta<1 || vrsta>9 || kolona<0 || kolona>9);
	
	if(postavka[vrsta-1][kolona-1]!=0){
		printf("Vec postoji broj na zadatoj poziciji, probajte ponovo.\n");
		printf("#########################################################\n");
		return;
	}
	
	do{
		printf("Unesite broj (1-9): \n");
		scanf("%d", &broj);
	}while(broj<1 || broj>9);
	
	
	
	if(broj==resen[vrsta-1][kolona-1]){
		postavka[vrsta-1][kolona-1]=broj;
		
	}
	else{
		(*greska)++;
		if((*greska)==1)
			printf("\nNIJE DOBAR BROJ! IMATE %d GRESKU! PROBAJTE PONOVO!\n", *greska);
		else if((*greska)==2)
			printf("\nNIJE DOBAR BROJ! IMATE %d GRESKE! IMATE JOS JEDAN POKUSAJ!\n", *greska);
		else
			printf("\nNIJE DOBAR BROJ! NEMATE VISE POKUSAJA!\n");
		printf("#########################################################");
	}
		
	
	
}

