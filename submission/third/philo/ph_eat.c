/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_eat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:23:49 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:23:50 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	odd_take_fork(void *args)
{
	t_args	*a;
	int		n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	pthread_mutex_lock(&a->shared->fork[a->tid]);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->fork[a->tid]);
		return (1);
	}
	pthread_mutex_lock(&a->shared->fork[(a->tid + 1) % n]);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->fork[(a->tid + 1) % n]);
		pthread_mutex_unlock(&a->shared->fork[a->tid]);
		return (1);
	}
	return (0);
}

int	odd_eat(void *args)
{
	t_args	*a;
	int		n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	if (odd_take_fork(args))
		return (1);
	if (printf_wrap("is eating", args))
		return (1);
	ft_msleep(a->shared->time_to_eat);
	pthread_mutex_unlock(&a->shared->fork[(a->tid + 1) % n]);
	pthread_mutex_unlock(&a->shared->fork[a->tid]);
	return (0);
}

int	even_take_fork(void *args)
{
	t_args	*a;
	int		n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	pthread_mutex_lock(&a->shared->fork[(a->tid + 1) % n]);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->fork[a->tid]);
		return (1);
	}
	pthread_mutex_lock(&a->shared->fork[a->tid]);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->fork[a->tid]);
		pthread_mutex_unlock(&a->shared->fork[(a->tid + 1) % n]);
		return (1);
	}
	return (0);
}

int	even_eat(void *args)
{
	t_args	*a;
	int		n;

	a = (t_args *)args;
	n = a->shared->number_of_philosophers;
	if (even_take_fork(args))
		return (1);
	if (printf_wrap("is eating", args))
		return (1);
	ft_msleep(a->shared->time_to_eat);
	pthread_mutex_unlock(&a->shared->fork[a->tid]);
	pthread_mutex_unlock(&a->shared->fork[(a->tid + 1) % n]);
	return (0);
}

int	ph_eat(void *args)
{
	t_args	*a;
	int		n;

	a = (t_args *)args;
	if (a->tid % 2)
	{
		if (odd_eat(args))
			return (1);
	}
	else
	{
		if (even_eat(args))
			return (1);
	}
	n = a->shared->number_of_philosophers;
	if (n % 2)
	{
		pthread_mutex_lock(&a->shared->mtx_blocked);
		a->shared->blocked_tid = a->tid;
		pthread_mutex_unlock(&a->shared->mtx_blocked);
	}
	return (0);
}
