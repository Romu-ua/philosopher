#include "philo.h"

int	is_all_one(int *arr, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (!arr[i])
			return (0);
		i++;
	}
	return (1);
}

void	*die_worker(void *args)
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
		pthread_mutex_lock(&a->shared->mtx_fin_flags);
		if (is_all_one(a->shared->fin_flags, a->shared->number_of_philosophers))
		{
			pthread_mutex_unlock(&a->shared->mtx_fin_flags);
			break ;
		}
		if (a->shared->fin_flags[i % n])
		{
			i++;
			pthread_mutex_unlock(&a->shared->mtx_fin_flags);
			ft_msleep(1);
			continue;
		}
		pthread_mutex_unlock(&a->shared->mtx_fin_flags);
		// printf("is not all one\n");
		pthread_mutex_lock(&a->shared->mtx_die);
		gettimeofday(&curr, NULL);
		ms = timestamp_ms(*(a[i % n].last_eat_ts), curr);
		// if (a->tid == 0)
			// printf("[DEBUG] last_eat_time of %d philo is %d\n", a->tid, timestamp_ms(*(a->last_eat_ts), curr));
		pthread_mutex_unlock(&a->shared->mtx_die);
		// printf("[DEBUG] ms: %d | philo: %lld \n", ms, i % n);
		if (ms > a[i % n].shared->time_to_die)
		{
			pthread_mutex_lock(&a->shared->mtx_die);
			a[i % n].shared->is_die = 1;
			ms = timestamp_ms(a->shared->start, curr);
			printf("%d %d died\n", ms, a[i % n].tid);
			pthread_mutex_unlock(&a->shared->mtx_die);
			break ;
		}
		i++;
		if (i == LLONG_MAX)
			i = 0;
		ft_msleep(1);
	}
	return (NULL);
}

int	process(void *args)
{
	if (stop(args))
		return (1);
	think(args);
	waiting_in_line(args);
	enter(args);
	if (stop(args))
		return (1);
	eat(args);
	leave(args);
	if (stop(args))
		return (1);
	philo_sleep(args);
	return (0);
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
			if (process(args))
				break ;
		}
	}
	else
	{
		while (i < a->shared->number_of_times_each_philosopher_must_eat)
		{
			if (process(args))
				break ;
			i++;
		}
	}
	pthread_mutex_lock(&a->shared->mtx_fin_flags);
	a->shared->fin_flags[a->tid] = 1;
	// printf("[DEBUG] philo %d fin flag 1\n", a->tid);
	pthread_mutex_unlock(&a->shared->mtx_fin_flags);
	return (NULL);
}
