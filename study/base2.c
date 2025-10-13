// baseからmsを表示させる機能をつける

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct s_shared {
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*mtx;
	struct timeval	start;
}	t_shared;

typedef struct s_args {
	int			tid;
	t_shared	*shared;
}	t_args;

void	*worker(void *args)
{
	struct timeval	timestamp;

	t_args *a = (t_args *)args;

	if (a->tid % 2 && a->tid == a->shared->number_of_philosophers - 1)
	{
		// 全体が奇数かつ最後の人
		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		pthread_mutex_lock(&(a->shared->mtx[0]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		gettimeofday(&timestamp, NULL);
		printf("%d philo %d is eating\n", timestamp.tv_usec, a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[0]));
		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
	}
	else if (!(a->tid % 2) && a->tid == a->shared->number_of_philosophers - 1)
	{
		// 全体が偶数かつ最後の人
		pthread_mutex_lock(&(a->shared->mtx[0]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		gettimeofday(&timestamp, NULL);
		printf("%d philo %d is eating\n", timestamp.tv_usec, a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
		pthread_mutex_unlock(&(a->shared->mtx[0]));
	}
	else if (a->tid % 2)
	{
		// 奇数グループ
		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		pthread_mutex_lock(&(a->shared->mtx[a->tid + 1]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		gettimeofday(&timestamp, NULL);
		printf("%d philo %d is eating\n", timestamp.tv_usec, a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[a->tid + 1]));
		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
	}
	else 
	{
		// 偶数グループ
		pthread_mutex_lock(&(a->shared->mtx[a->tid + 1]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		gettimeofday(&timestamp, NULL);
		printf("%d philo %d has token a fork\n", timestamp.tv_usec, a->tid);

		gettimeofday(&timestamp, NULL);
		printf("%d philo %d is eating\n", timestamp.tv_usec, a->tid);

		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
		pthread_mutex_unlock(&(a->shared->mtx[a->tid + 1]));
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
		shared->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	shared->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * shared->number_of_philosophers);

	// mutexの初期化
	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_mutex_init(&(shared->mtx[i]), NULL);
	}

	// スタートの時間を取得
	gettimeofday(&shared->start, NULL);

	// スレッドの作成
	th = (pthread_t *)malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	args = (t_args *)malloc(sizeof(t_args) * shared->number_of_philosophers);

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		args[i].tid = i;
		args[i].shared = shared;
		if (pthread_create(&th[i], NULL, worker, &args[i]))
		{
			perror("pthread_create");
			return (1);
		}
	}

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_join(th[i], NULL);
	}

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_mutex_destroy(&shared->mtx[i]);
	}
	
	free(shared->mtx);
	free(shared);
	free(th);
	free(args);
}
