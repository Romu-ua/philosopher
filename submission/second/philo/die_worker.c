/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die_worker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:25:27 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:25:28 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_finished(t_shared *shared)
{
	int	i;
	int	n;
	int	all;

	if (shared->number_of_times_each_philosopher_must_eat < 0)
		return (0);
	n = shared->number_of_philosophers;
	i = 0;
	all = 1;
	pthread_mutex_lock(&shared->mtx_fin_flags);
	while (i < n)
	{
		if (shared->fin_flags[i] == 0)
		{
			all = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&shared->mtx_fin_flags);
	return (all);
}

int	someone_die(t_shared *shared, int i, int n)
{
	int	finished;

	pthread_mutex_lock(&shared->mtx_fin_flags);
	finished = shared->fin_flags[i % n];
	pthread_mutex_unlock(&shared->mtx_fin_flags);
	return (finished);
}

int	check_death(t_args *a, int n, int i)
{
	int				ms;
	struct timeval	tv;

	if (all_finished(a->shared))
		return (1);
	if (someone_die(a->shared, i, n))
		return (0);
	pthread_mutex_lock(&a->shared->mtx_printf);
	pthread_mutex_lock(&a[i % n].mtx_last_eat_ts);
	gettimeofday(&tv, NULL);
	ms = timestamp_ms(*(a[i % n].last_eat_ts), tv);
	if (ms > a->shared->time_to_die)
	{
		a->shared->is_die = 1;
		ms = timestamp_ms(a->shared->start, tv);
		printf("%d %d died\n", ms, a[i % n].tid + 1);
		pthread_mutex_unlock(&a[i % n].mtx_last_eat_ts);
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return (1);
	}
	pthread_mutex_unlock(&a[i % n].mtx_last_eat_ts);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	return (0);
}

void	*die_worker(void *args)
{
	t_args		*a;
	int			i;
	int			n;

	a = (t_args *)args;
	i = 0;
	n = a->shared->number_of_philosophers;
	while (1)
	{
		if (check_death(a, n, i))
			break ;
		if (i++ == INT_MAX)
			i = 0;
		ft_msleep(1);
	}
	return (NULL);
}
