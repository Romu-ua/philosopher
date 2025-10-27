/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   odd_case.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:23:39 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:23:41 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	yield_to_urgent(void *args)
{
	t_args	*a;
	int		urgent;
	int		n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	if (n % 2 == 0)
		return ;
	pthread_mutex_lock(&a->shared->mtx_urgent);
	urgent = a->shared->urgent_tid;
	pthread_mutex_unlock(&a->shared->mtx_urgent);
	if (urgent != -1 || urgent != a->tid)
		usleep(200);
	return ;
}

void	wait_eat(void *args)
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
			return ;
		usleep(100);
	}
}

int	odd_case(void *args)
{
	t_args	*a;
	int		n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	if (n == 1)
	{
		while (1)
		{
			if (stop(args))
				return (1);
		}
	}
	if (n % 2)
	{
		yield_to_urgent(args);
		wait_eat(args);
	}
	return (0);
}
