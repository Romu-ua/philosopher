#include "philo.h"

void	odd_eat(void *args)
{
	struct timeval	timestamp;
	int				ms;
	t_args			*a;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx[a->tid]);
	pthread_mutex_lock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
	pthread_mutex_lock(&a->shared->mtx_die);
	gettimeofday(&timestamp, NULL);
	*(a->last_eat_ts) = timestamp;
	pthread_mutex_unlock(&a->shared->mtx_die);
	gettimeofday(&timestamp, NULL);
	ms = timestamp_ms(a->shared->start, timestamp);
	if (stop(args))
		return ;
	printf("%d %d is eating\n", ms, a->tid);
	ft_msleep(a->shared->time_to_eat);
	pthread_mutex_unlock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
	pthread_mutex_unlock(&a->shared->mtx[a->tid]);
}

void	even_eat(void *args)
{
	struct timeval	timestamp;
	int				ms;
	t_args			*a;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
	pthread_mutex_lock(&a->shared->mtx[a->tid]);
	pthread_mutex_lock(&a->shared->mtx_die);
	gettimeofday(&timestamp, NULL);
	*(a->last_eat_ts) = timestamp;
	pthread_mutex_unlock(&a->shared->mtx_die);
	gettimeofday(&timestamp, NULL);
	ms = timestamp_ms(a->shared->start, timestamp);
	if (stop(args))
		return ;
	printf("%d %d is eating\n", ms, a->tid);
	// lwrite(ms, a->tid, 0);
	ft_msleep(a->shared->time_to_eat);
	pthread_mutex_unlock(&a->shared->mtx[a->tid]);
	pthread_mutex_unlock(&a->shared->mtx[(a->tid + 1) % a->shared->number_of_philosophers]);
}

void	eat(void *args)
{
	t_args			*a;

	a = (t_args *)args;
	if (a->tid % 2)
		odd_eat(args);
	else
		even_eat(args);
}
