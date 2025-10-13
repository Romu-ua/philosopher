// dieするかどうかを判定するためにスレッドをもう一つ作成してそこで確認する。
// is_dieのフラグが立ったら、die以降のコメントを出さないように、
// dieを頻繁に確認して、die以降にメッセージが出ないようにする.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_node
{
	int				data;
	struct s_node	*next;
} t_node;

typedef struct s_queue
{
	t_node	*front;
	t_node	*rear;
}	t_queue;

typedef struct s_shared
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*mtx;
	
	// 会場の管理
	pthread_mutex_t	is_waiting;
	pthread_mutex_t	adm_enter;
	int				capa;
	int				number_of_inside;

	// dieの確認用スレッド
	pthread_mutex_t	mtx_die;
	int				is_die;

	struct timeval	start;
	t_queue			*q;
}	t_shared;

typedef struct s_args
{
	int				tid;
	t_shared		*shared;
	struct timeval	last_eat_ts;
}	t_args;

int	is_empty(t_queue *q)
{
	if (q->front == NULL)
		return (1);
	return (0);
}

int timestamp_ms(struct timeval start, struct timeval curr)
{
	int		diff;
	long	curr_ms;
	long	start_ms;

	start_ms = start.tv_sec * 1000000 + start.tv_usec;
	curr_ms = curr.tv_sec * 1000000 + curr.tv_usec;
	diff = curr_ms - start_ms;
	return ((int)(diff / 1000));
}

void	msleep(int ms)
{
	if (ms > 0)
		usleep(ms * 1000); // macではuseconds_t、定義はunsgined int
}

int	stop(void *args)
{
	t_args	*a;
	int		flag;

	flag = 0;
	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_die);
	if (a->shared->is_die)
		flag = 1;
	pthread_mutex_unlock(&a->shared->mtx_die);
	return (flag);
}

void	think(void *args)
{
	t_args			*a;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	a = (t_args *)args;
	


	printf("%d %d is thinking\n", timestamp_ms(a->shared->start, curr), a->tid);
}

void	waiting_in_line(void *args)
{
	t_args	*a;
	t_node	*new_node;

	a = (t_args *)args;

	new_node = (t_node *)malloc(sizeof(t_node));
	new_node->data = a->tid;
	new_node->next = NULL;
	pthread_mutex_lock(&a->shared->is_waiting);
	if (!a->shared->q->front)
	{
		a->shared->q->front = new_node;
		a->shared->q->rear = new_node;
	}
	else
	{
		a->shared->q->rear->next = new_node;
		a->shared->q->rear = new_node;
	}
	pthread_mutex_unlock(&a->shared->is_waiting);
}

void	get_out_of_line(void *args)
{
	t_args	*a;
	t_node	*tmp;

	a = (t_args *)args;
	if (is_empty(a->shared->q))
	{
		printf("queue is empty.\n");
		return ;
	}
	tmp = a->shared->q->front;
	a->shared->q->front = tmp->next;
	if (a->shared->q->front == NULL)
		a->shared->q->rear = NULL;
	free(tmp);
}

void	enter(void *args)
{
	t_args	*a;

	a = (t_args *)args;
	while (1)
	{
		if (a->shared->capa > a->shared->number_of_inside)
		{
			pthread_mutex_lock(&a->shared->adm_enter);
			a->shared->number_of_inside++;
			pthread_mutex_lock(&a->shared->is_waiting);
			get_out_of_line(args);

			pthread_mutex_unlock(&a->shared->is_waiting);
			pthread_mutex_unlock(&a->shared->adm_enter);
			break ;
		}
	}
	return ;
}

void	leave(void *args)
{
	t_args	*a;

	a = (t_args *)args;

	pthread_mutex_lock(&a->shared->adm_enter);
	a->shared->number_of_inside--;
	pthread_mutex_unlock(&a->shared->adm_enter);
}

void	philo_sleep(void *args)
{
	t_args			*a;
	struct timeval	curr;

	a = (t_args *)args;
	gettimeofday(&curr, NULL);
	printf("%d %d is sleeping\n", timestamp_ms(a->shared->start, curr), a->tid);
	msleep(a->shared->time_to_sleep);
}

void	eat(void *args)
{
	struct timeval	timestamp;
	int				ms;
	t_args			*a;
	a = (t_args *)args;

	if (a->tid % 2)
	{
		pthread_mutex_lock(&a->shared->mtx[a->tid]);
		pthread_mutex_lock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);

		gettimeofday(&timestamp, NULL);
		ms = timestamp_ms(a->shared->start, timestamp);

		if (stop(args))
			return ;

		printf("%d %d is eating\n", ms, a->tid);
		msleep(a->shared->time_to_eat);

		// last_eat_tsはdieのチェックで使うのでlockする
		pthread_mutex_lock(&a->shared->mtx_die);
		gettimeofday(&timestamp, NULL);
		a->last_eat_ts = timestamp;
		pthread_mutex_unlock(&a->shared->mtx_die);

		pthread_mutex_unlock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
		pthread_mutex_unlock(&a->shared->mtx[a->tid]);
	}
	else
	{
		pthread_mutex_lock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
		pthread_mutex_lock(&a->shared->mtx[a->tid]);

		gettimeofday(&timestamp, NULL);
		ms = timestamp_ms(a->shared->start, timestamp);
		if (stop(args))
			return ;
		printf("%d %d is eating\n", ms, a->tid);
		msleep(a->shared->time_to_eat);

		pthread_mutex_lock(&a->shared->mtx_die);
		gettimeofday(&timestamp, NULL);
		a->last_eat_ts = timestamp;
		pthread_mutex_unlock(&a->shared->mtx_die);

		pthread_mutex_unlock(&a->shared->mtx[a->tid]);
		pthread_mutex_unlock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
	}
}

void	*die_check(void *args)
{
	int				n;
	long long		i;
	t_args			*a;
	struct timeval	curr;
	int				ms;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;	
	i = 0;
	while (1)
	{
		gettimeofday(&curr, NULL);
		ms = timestamp_ms(a[i % n].last_eat_ts, curr);
		if (ms > a[i % n].shared->time_to_die)
		{
			a[i % n].shared->is_die = 1;
			printf("%d %d died\n", ms, a[i % n].tid);
			break ;
		}
		i++;
		if (i == LLONG_MAX)
			i = 0;
	}
	return (NULL);
}


void	*worker(void *args)
{
	t_args	*a;
	int		i;

	a = (t_args *)args;
	i = 0;
	if (a->shared->number_of_times_each_philosopher_must_eat < 0)
	{
		(void)i;
		while (1)
		{
			if (stop(args))
				break ;
			think(args);
			waiting_in_line(args);
			enter(args);
			if (stop(args))
				break ;
			eat(args);
			leave(args);
			if (stop(args))
				break ;
			philo_sleep(args);
		}
	}
	else
	{
		while (i < a->shared->number_of_times_each_philosopher_must_eat)
		{
			if (stop(args))
				break ;
			think(args);
			waiting_in_line(args);
			enter(args);
			if (stop(args))
				break ;
			eat(args);
			leave(args);
			if (stop(args))
				break ;
			philo_sleep(args);
			i++;
		}
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_shared	*shared;
	pthread_t	*th;
	pthread_t	th_die;
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
		shared->number_of_times_each_philosopher_must_eat = -1;
	shared->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * shared->number_of_philosophers);
	
	// 会場の出入り
	pthread_mutex_init(&shared->is_waiting, NULL);
	pthread_mutex_init(&shared->adm_enter, NULL);
	shared->capa = shared->number_of_philosophers - 1;

	shared->q = (t_queue *)malloc(sizeof(t_queue));
	// 待ち行列のqueueをここで初期化
	shared->q->front = NULL;
	shared->q->rear = NULL;

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_mutex_init(&(shared->mtx[i]), NULL);
	}
	gettimeofday(&shared->start, NULL);

	th = (pthread_t *)malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	args = (t_args *)malloc(sizeof(t_args) * shared->number_of_philosophers);

	for (int i = 0; i < shared->number_of_philosophers; i++) {
		args[i].tid = i;
		args[i].shared = shared;
		args[i].last_eat_ts = shared->start;
		if (pthread_create(&th[i], NULL, worker, &args[i]))
		{
			perror("pthread_create");
			return (1);
		}
	}
	
	pthread_mutex_init(&shared->mtx_die, NULL);
	if (pthread_create(&th_die, NULL, die_check, args))
	{
		perror("pthread_create");
		return (1);
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
