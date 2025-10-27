/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:39:53 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/13 19:39:55 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*util_return(int *is_miss_init, t_shared *shared)
{
	*is_miss_init = 1;
	shared_cleanup(shared);
	return (NULL);
}

void	*cleanup_return(t_shared *shared)
{
	shared_cleanup(shared);
	return (NULL);
}

void	th_init(t_shared *shared, pthread_t **th, int *is_miss_init)
{
	int	n;

	n = shared->number_of_philosophers;
	*th = (pthread_t *)malloc(sizeof(pthread_t) * n);
	if (!*th)
		*is_miss_init = 1;
}

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
