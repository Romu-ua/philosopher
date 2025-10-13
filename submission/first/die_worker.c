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

int	is_all_one(t_args *a, int n)
{
	int	i;
	int	*arr;

	i = 0;
	arr = a->shared->fin_flags;
	pthread_mutex_lock(&a->shared->mtx_fin_flags);
	while (i < n)
	{
		if (!arr[i])
		{
			pthread_mutex_unlock(&a->shared->mtx_fin_flags);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&a->shared->mtx_fin_flags);
	return (1);
}

int	should_skip(t_args *a, int n, int i)
{
	int	result;

	pthread_mutex_lock(&a->shared->mtx_fin_flags);
	result = a->shared->fin_flags[i % n];
	pthread_mutex_unlock(&a->shared->mtx_fin_flags);
	if (result)
		ft_msleep(1);
	return (result);
}

int	check_death(t_args *a, int n, int i)
{
	int				ms;
	struct timeval	tv;

	pthread_mutex_lock(&a[i % n].mtx_last_eat_ts);
	gettimeofday(&tv, NULL);
	ms = timestamp_ms(*(a[i % n].last_eat_ts), tv);
	if (ms > a->shared->time_to_die)
	{
		a->shared->is_die = 1;
		ms = timestamp_ms(a->shared->start, tv);
		printf("%d %d died\n", ms, a[i % n].tid);
		pthread_mutex_unlock(&a[i % n].mtx_last_eat_ts);
		return (1);
	}
	pthread_mutex_unlock(&a[i % n].mtx_last_eat_ts);
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
		if (is_all_one(a, n))
			break ;
		if (should_skip(a, n, i))
		{
			i++;
			continue ;
		}
		if (check_death(a, n, i))
			break ;
		if (i++ == INT_MAX)
			i = 0;
		ft_msleep(1);
	}
	return (NULL);
}
