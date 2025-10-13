#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

// mtxのindexがforkに対応
// 実行する時間を差分で保持しておくために、startのmsを構造体に入れる

// startの時間最初に確定させたいのでshareを作ってそこに入れる。
typedef struct s_shared {
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_mutex_t	*mtx;
	struct timeval	start;
}	t_shared;

typedef struct s_args {
	int				tid;
	t_shared		*shared;
}	t_args;

void	*worker(void *args)
{
	t_args *a = (t_args *)args;

	if (a->tid % 2 && a->tid != a->shared->number_of_philosophers)
	{
		// oddグループ
		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		printf("philo %d has token rigth forks\n", a->tid);

		pthread_mutex_lock(&(a->shared->mtx[a->tid + 1]));
		printf("philo %d has token left forks\n", a->tid);

		printf("philo %d is eating\n", a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[a->tid + 1]));
		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));

		printf("philo %d is sleeping\n", a->tid);
	}
	else if (!(a->tid % 2) && a->tid != a->shared->number_of_philosophers)
	{
		// evenグループ
		pthread_mutex_lock(&(a->shared->mtx[a->tid + 1]));
		printf("philo %d has token left forks\n", a->tid);

		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		printf("philo %d has token rigth forks\n", a->tid);

		printf("philo %d is eating\n", a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
		pthread_mutex_unlock(&(a->shared->mtx[a->tid + 1]));

		printf("philo %d is sleeping\n", a->tid);
	}
	else if (a->shared->number_of_philosophers % 2)
	{
		// oddグループ 最後の一人
		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		printf("philo %d has token rigth forks\n", a->tid);

		pthread_mutex_lock(&(a->shared->mtx[0]));
		printf("philo %d has token left forks\n", a->tid);

		printf("philo %d is eating\n", a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[0]));
		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));

		printf("philo %d is sleeping\n", a->tid);
	}
	else
	{
		// evenグループ 最後の一人
		pthread_mutex_lock(&(a->shared->mtx[0]));
		printf("philo %d has token left forks\n", a->tid);

		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		printf("philo %d has token rigth forks\n", a->tid);

		printf("philo %d is eating\n", a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
		pthread_mutex_unlock(&(a->shared->mtx[0]));

		printf("philo %d is sleeping\n", a->tid);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_shared	*shared;
	pthread_t	*th;
	t_args		*args;

	if (argc > 6 || argc < 5)
	{
		printf("%s <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
			 [optina]<number_of_times_each_philosopher_must_eat>",argv[0]);
			 return (1);
	}
	shared = (t_shared *)malloc(sizeof(t_shared));
	if (!shared)
		return (1);
	shared->number_of_philosophers = atoi(argv[1]);
	shared->time_to_die = atoi(argv[2]);
	shared->time_to_eat = atoi(argv[3]);
	shared->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		shared->number_of_philosophers = atoi(argv[5]);
	shared->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * \
				shared->number_of_philosophers);

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_mutex_init(&(shared->mtx[i]), NULL);
	}
	
	gettimeofday(&shared->start, NULL);
	th = (pthread_t *)malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	args = (t_args *)malloc(sizeof(t_args) * shared->number_of_philosophers);

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		args[i].tid = i;
		args[i].shared = shared;
		if (pthread_create(&th[i], NULL, worker, &args[i]))
		{
			perror("pthread_create");
			// TODO freeの処理入れる
			return (1);
		}
	}

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_join(th[i], NULL);
	}

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_mutex_destroy(&(shared->mtx[i]));
	}
	free(shared->mtx);
	free(th);
	free(args);
	return (0);
}
