/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:25:55 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:25:56 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	timestamp_ms(struct timeval start, struct timeval curr)
{
	int		diff;
	long	curr_ms;
	long	start_ms;

	start_ms = start.tv_sec * 1000000 + start.tv_usec;
	curr_ms = curr.tv_sec * 1000000 + curr.tv_usec;
	diff = curr_ms - start_ms;
	return ((int)(diff / 1000));
}

void	register_last_eat_ts(t_args *a)
{
	struct timeval	timestamp;

	pthread_mutex_lock(&a->mtx_last_eat_ts);
	gettimeofday(&timestamp, NULL);
	*(a->last_eat_ts) = timestamp;
	pthread_mutex_unlock(&a->mtx_last_eat_ts);
}

void	odd_eat(void *args)
{
	struct timeval	timestamp;
	int				ms;
	t_args			*a;
	int				n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	pthread_mutex_lock(&a->shared->mtx[a->tid]);
	pthread_mutex_lock(&a->shared->mtx[(a->tid + 1) % n]);
	register_last_eat_ts(a);
	pthread_mutex_lock(&a->shared->mtx_printf);
	gettimeofday(&timestamp, NULL);
	ms = timestamp_ms(a->shared->start, timestamp);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return ;
	}
	printf("%d %d is eating\n", ms, a->tid);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	ft_msleep(a->shared->time_to_eat);
	pthread_mutex_unlock(&a->shared->mtx[(a->tid + 1) % n]);
	pthread_mutex_unlock(&a->shared->mtx[a->tid]);
}

void	even_eat(void *args)
{
	struct timeval	timestamp;
	int				ms;
	t_args			*a;
	int				n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	pthread_mutex_lock(&a->shared->mtx[(a->tid + 1) % n]);
	pthread_mutex_lock(&a->shared->mtx[a->tid]);
	register_last_eat_ts(a);
	pthread_mutex_lock(&a->shared->mtx_printf);
	gettimeofday(&timestamp, NULL);
	ms = timestamp_ms(a->shared->start, timestamp);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return ;
	}
	printf("%d %d is eating\n", ms, a->tid);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	ft_msleep(a->shared->time_to_eat);
	pthread_mutex_unlock(&a->shared->mtx[a->tid]);
	pthread_mutex_unlock(&a->shared->mtx[(a->tid + 1) % n]);
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
