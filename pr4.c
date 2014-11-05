/*
Meno:
Datum: NEODSKUSANE

Simulujte nasledujucu situaciu. Desiati maliari maluju steny.
Maliarovi trva nejaky cas, kym stenu maluje (v simulacii 2s) a nejaky cas, kym si ide nabrat farbu do vedra
(v simulacii 1s). Cela simulacia nech trva nejaky cas (30s).

1. Doplnte do programu pocitadlo celkoveho poctu vedier minutej farby a tiez nech si kazdy maliar pocita,
kolko vedier farby uz minul preniesol, na konci simulacie vypiste hodnoty pocitadiel. [2b]

2. Ked maliar minie 4 vedra, pocka na dvoch svojich kolegov a kazdy si spravi prestavku na nejaky cas (v simulacii 2s). [5b]

3. Osetrite v programe spravne ukoncenie simulacie hned po uplynuti stanoveneho casu (nezacne sa dalsia cinnost). [3b]


Poznamky:
- na synchronizaciu pouzite iba mutexy+podmienene premenne alebo semafory
- nespoliehajte sa na uvedene casy, simulacia by mala fungovat aj s inymi casmi
- build (console): gcc maliari.c -o maliari -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "windows.h"

sem_t mux, inymux, muxEnd;
sem_t turns1, inyturns;
int barCnt = 0, inyCnt = 0;

// signal na zastavenie
int stoj = 0;

// maliar - malovanie steny
void maluj(void) {
    sleep(2);
}

//  maliar - branie farby
void ber(void) {
  sleep(1);
}

void barr_wait(){

	//vchod do bariery
    sem_wait(&mux);
		sem_wait(&inyturns);
		sem_post(&inyturns);
		barCnt++;
		if(barCnt == 3){
			sem_post(&turns);
			barCnt = 0;		
			sem_wait(&inyturns);//minie turniket
			sem_wait(&inyturns);//a zastane...
		}	
    sem_post(&mux);
	
    sem_wait(&turns);
	sem_post(&turns);
	
	//prechod cez barieru
	sem_wait(&inymutex)
		inycnt++;
		if(inycnt == 3){
			sem_post(&inyturns);//...kym predosli 3 nezacnu prestavkovat
			sem_post(&inyturns);
			inycnt = 0;		
		}
	sem_post(&inymutex);
	
	///prestavka
	
	
}


// maliar
void *maliar( void *ptr ) {

	int i;
		
	for(i=1;;i++){
			
		maluj();
			
		sem_wait(&muxEnd);
		if(stoj==1){
			sem_post(&muxEnd);
			return;
			}
		sem_post(&muxEnd);
		
		ber();
		
		sem_wait(&muxEnd);
		if(stoj==1){
			sem_post(&muxEnd);
			return;
			}
		sem_post(&muxEnd);
		
		if(i%4 == 0){
			
			barr_wait();
			
			sem_wait(&muxEnd);
			if(stoj==1){
				sem_post(&muxEnd);
				return;
				}
			sem_post(&muxEnd);
			
			Sleep(200);
			
			sem_wait(&muxEnd);
			if(stoj==1){
				sem_post(&muxEnd);
				return null;
				}
			sem_post(&muxEnd);
		
		}	
	}
}
		

int main(void) {
    int i;

    sem_init(&mux,0,1);
    sem_init(&inymux,0,1);
    sem_init(&muxEnd,0,1);
    sem_init(&turns, 0,0);
    sem_init(&inyturns, 0,1);

    pthread_t maliari[10];

    for (i=0;i<10;i++) pthread_create(&maliari[i], NULL, &maliar, NULL);

    sleep(30);
	
	sem_wait(&muxEnd);
		stoj = 1;
	sem_post(&muxEnd);
	
	sem_post(&turns);
	sem_post(&inyturns);
	

    for (i=0;i<10;i++) pthread_join(maliari[i], NULL);

    exit(EXIT_SUCCESS);
}
