/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urgent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:39 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:42 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_tmp(t_args *a, t_urgent *tmp)
{
	tmp->n = a->shared->number_of_philosophers;
	gettimeofday(&tmp->now, NULL);
	tmp->i = 0;
	tmp->max_ms = -1;
	tmp->min_ms = INT_MAX;
	tmp->urgent = -1;
	tmp->sated = -1;
}

void	urgent_mutex(t_args *a, t_urgent *tmp)
{
	pthread_mutex_lock(&a->shared->mtx_urgent);
	a->shared->urgent_tid = tmp->urgent;
	pthread_mutex_unlock(&a->shared->mtx_urgent);
	if (tmp->n % 2 == 1 && tmp->sated != -1)
	{
		pthread_mutex_lock(&a->shared->mtx_blocked);
		a->shared->blocked_tid = tmp->sated;
		pthread_mutex_unlock(&a->shared->mtx_blocked);
	}
}

void	update_priority(t_args *a)
{
	t_urgent	tmp;

	init_tmp(a, &tmp);
	while (tmp.i < tmp.n)
	{
		pthread_mutex_lock(&a[tmp.i].mtx_last_eat_ts);
		tmp.ms = timestamp_ms(*(a[tmp.i].last_eat_ts), tmp.now);
		pthread_mutex_unlock(&a[tmp.i].mtx_last_eat_ts);
		if (tmp.ms > tmp.max_ms)
		{
			tmp.max_ms = tmp.ms;
			tmp.urgent = tmp.i;
		}
		if (tmp.ms < tmp.min_ms)
		{
			tmp.min_ms = tmp.ms;
			tmp.sated = tmp.i;
		}
		tmp.i++;
	}
	urgent_mutex(a, &tmp);
}
