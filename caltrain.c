#include <pthread.h>
#include "caltrain.h"
//----
pthread_mutex_t train_mutex,passenger_mutex,seated_mutex;
pthread_cond_t passenger_cond,train_cond;
//----
void
station_init(struct station *station)
{
	// FILL ME IN
	station->passenger_count=0;station->trains_count=0;
	pthread_mutex_init(&train_mutex,NULL);
	pthread_mutex_init(&passenger_mutex,NULL);
	pthread_cond_init(&passenger_cond,NULL);
	pthread_cond_init(&train_cond,NULL);
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	pthread_mutex_lock(&train_mutex);
	//lock so that no other trains comes here
	station->trains_count++;
	if(station->passenger_count > 0 && count > 0){
        int estimated_num;
        if(station->passenger_count > count){
            estimated_num = station->passenger_count-count;
        }else{
            estimated_num = 0;
        }
        pthread_cond_broadcast(&passenger_cond);
        while(station->passenger_count > estimated_num){
            pthread_cond_wait(&train_cond,&train_mutex);
        }
	}
	station->trains_count--;
	//unlock
    pthread_mutex_unlock(&train_mutex);
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
    //not more than one passenger at a time because of counter of passengers and trains
    pthread_mutex_lock(&passenger_mutex);
    station->passenger_count++;
    while(station->trains_count < 1){
        pthread_cond_wait(&passenger_cond,&passenger_mutex);
    }
    pthread_mutex_unlock(&passenger_mutex);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
    pthread_mutex_lock(&seated_mutex);
    station->passenger_count--;
    pthread_cond_signal(&train_cond);
    pthread_mutex_unlock(&seated_mutex);

}
