// 出力するtimestampをmsにする
// 処理を1msごとにするためにusleepとgettimeofdayの関数の使い方を複数する。
// 一度のループ

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

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
	int				tid;
	t_shared		*shared;
	struct timeval	last_eat_ts;
}	t_args;

int timestamp_ms(struct timeval start, struct timeval curr)
{
	int	diff;
	int	curr_ms;
	int	start_ms;

	start_ms = start.tv_sec * 1000000 + start.tv_usec;
	curr_ms = curr.tv_sec * 1000000 + curr.tv_usec;
	diff = curr_ms - start_ms;
	// printf("diff : %d, curr : %d, start : %d\n", diff, curr_ms, start_ms);
	return ((int)(diff / 1000));
}

void	die_check(t_args *args)
{
	struct timeval	curr_ts;

	gettimeofday(&curr_ts, NULL);
	if (curr_ts.tv_usec - args->last_eat_ts.tv_usec >= args->shared->time_to_die)
	{
		printf("%d philo %d die\n", curr_ts.tv_usec, args->tid);
	}
}

void	*worker(void *args)
{
	struct timeval	timestamp;
	int				ms;

	t_args *a = (t_args *)args;

	while (1)
	{
	if (a->tid % 2 && a->tid == a->shared->number_of_philosophers - 1)
		{
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[a->tid]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[0]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d is eating\n", ms, a->tid);
			usleep(a->shared->time_to_eat * 1000);
			
			gettimeofday(&timestamp, NULL);
			a->last_eat_ts = timestamp;
			
			pthread_mutex_unlock(&(a->shared->mtx[0]));
			pthread_mutex_unlock(&(a->shared->mtx[a->tid]));

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has is sleep\n", ms, a->tid);
			usleep(a->shared->time_to_sleep * 1000);
		}
		else if (!(a->tid % 2) && a->tid == a->shared->number_of_philosophers - 1)
		{
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[0]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);
			
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[a->tid]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d is eating\n", ms, a->tid);
			usleep(a->shared->time_to_eat * 1000);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			a->last_eat_ts = timestamp;

			pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
			pthread_mutex_unlock(&(a->shared->mtx[0]));

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has is sleep\n", ms, a->tid);
			usleep(a->shared->time_to_sleep * 1000);
		}
		else if (a->tid % 2)
		{
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[a->tid]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[a->tid + 1]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d is eating\n", ms, a->tid);
			usleep(a->shared->time_to_eat * 1000);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			a->last_eat_ts = timestamp;

			pthread_mutex_unlock(&(a->shared->mtx[a->tid + 1]));
			pthread_mutex_unlock(&(a->shared->mtx[a->tid]));

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has is sleep\n", ms, a->tid);
			usleep(a->shared->time_to_sleep * 1000);
		}
		else 
		{
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[a->tid + 1]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			// printf("%d %d has is thinking\n", ms, a->tid);

			pthread_mutex_lock(&(a->shared->mtx[a->tid]));
			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has token a fork\n", ms, a->tid);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d is eating\n", ms, a->tid);
			usleep(a->shared->time_to_eat * 1000);

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			a->last_eat_ts = timestamp;

			pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
			pthread_mutex_unlock(&(a->shared->mtx[a->tid + 1]));

			gettimeofday(&timestamp, NULL);
			ms = timestamp_ms(a->shared->start, timestamp);
			printf("%d %d has is sleep\n", ms, a->tid);
			usleep(a->shared->time_to_sleep * 1000);
		}
			
		gettimeofday(&timestamp, NULL);
		ms = timestamp_ms(a->shared->start, timestamp);
		if (timestamp.tv_usec - a->shared->start.tv_usec > a->shared->number_of_times_each_philosopher_must_eat)
			break ;
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
		printf("%s <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [optina]<number_of_times_each_philosopher_must_eat>",argv[0]);
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
	else
		shared->number_of_times_each_philosopher_must_eat = INT_MAX; // 暫定
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
