#include <pthread.h>

struct station {
	// FILL ME IN
	int passenger_count;
	int ava_seats;
	int boarding;
	pthread_mutex_t global_mutex;
    pthread_cond_t passenger_cond,train_cond;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
