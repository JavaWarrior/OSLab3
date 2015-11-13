#include <pthread.h>
#include "caltrain.h"
#include <stdio.h>
//----
#ifndef MIN
#define MIN(_x,_y) ((_x) < (_y)) ? (_x) : (_y)
#endif
//----
void
station_init(struct station *station)
{
	// FILL ME IN
	station->passenger_count=0;//total passenger on the station
	station->ava_seats=0;//available seats on the current train
	station->boarding=0;//intermediate between on train and waiting
	pthread_mutex_init(&station->global_mutex,NULL);
	pthread_cond_init(&station->passenger_cond,NULL);
	pthread_cond_init(&station->train_cond,NULL);
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
    // pthread_mutex_lock(&station->train_mutex);
	//lock so that no other trains comes here (boarding one train at a time).
	pthread_mutex_lock(&station->global_mutex);
    //lock so that no conflict happenes when checking and changing values of station elements.
	if(station->passenger_count > 0 && count > 0){
        station->ava_seats=count;
        while((station->ava_seats > 0 && station->passenger_count > 0) || station->boarding != 0){
            pthread_cond_signal(&station->passenger_cond);
            pthread_cond_wait(&station->train_cond,&station->global_mutex);
        }
	}
	station->ava_seats=0;
	//unlock
    pthread_mutex_unlock(&station->global_mutex);
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
    //not more than one passenger at a time because of counter of passengers and trains
    pthread_mutex_lock(&station->global_mutex);
    station->passenger_count++;
    while(station->ava_seats < 1){
//        printf("waiting train ava_seats= %d, passenger_count= %d\n",station->ava_seats,station->passenger_count);
        pthread_cond_wait(&station->passenger_cond,&station->global_mutex);
    }
    station->boarding++;
    station->ava_seats--;
    pthread_mutex_unlock(&station->global_mutex);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
    pthread_mutex_lock(&station->global_mutex);
    station->passenger_count--;
    station->boarding--;
//    printf("passenger boarded ava_seats= %d, passenger_count= %d\n",station->ava_seats,station->passenger_count);
    pthread_cond_signal(&station->train_cond);
    pthread_mutex_unlock(&station->global_mutex);

}

//ava seats is because when there's no enough slots in the train.
//passenger_count is when there's no enough people on the station.
//train finished but waiting for another passenger
