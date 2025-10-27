/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 10:56:40 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/23 10:56:40 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_eat(void *args)
{
	int		blocked;
	t_args	*a;

	a = (t_args *)args;
	while (1)
	{
		pthread_mutex_lock(&a->shared->mtx_blocked);
		blocked = a->shared->blocked_tid;
		pthread_mutex_unlock(&a->shared->mtx_blocked);
		if (blocked != a->tid)
			break ;
		if (stop(args))
			return (1);
		usleep(100);
	}
	return (0);
}

int	only_one(void *args)
{
	while (1)
	{
		if (stop(args))
			return (1);
	}
	return (0);
}

int	wait_favor_urgent(void *args)
{
	t_args	*a;
	int		urgent;

	a = (t_args *)args;
	if (a->shared->number_of_philosophers % 2 == 0)
		return (0);
	pthread_mutex_lock(&a->shared->mtx_urgent);
	urgent = a->shared->urgent_tid;
	pthread_mutex_unlock(&a->shared->mtx_urgent);
	if (urgent != -1 && urgent != a->tid)
	{
		if (stop(args))
			return (1);
		usleep(200);
	}
	return (0);
}

void	fin_flags_up(t_args *a)
{
	pthread_mutex_lock(&a->shared->mtx_fin_flags);
	a->shared->fin_flags[a->tid] = 1;
	pthread_mutex_unlock(&a->shared->mtx_fin_flags);
}

int	edge_case(void *args)
{
	t_args	*a;

	a = (t_args *)args;
	if (a->shared->number_of_philosophers == 1)
	{
		if (only_one(args))
			return (1);
	}
	if (a->shared->number_of_philosophers % 2 == 1)
	{
		if (wait_favor_urgent(args))
			return (1);
		if (wait_eat(args))
			return (1);
	}
	return (0);
}
