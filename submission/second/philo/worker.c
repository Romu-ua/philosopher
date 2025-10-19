/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:26:06 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:26:06 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	process(void *args)
{
	t_args	*a;

	a = (t_args *)args;
	think(args);
	if (stop(args))
		return (1);
	if (a->shared->number_of_philosophers == 1)
	{
		while (1)
		{
			if (stop(args))
				return (1);
		}
	}
	eat(args);
	if (stop(args))
		return (1);
	philo_sleep(args);
	if (stop(args))
		return (1);
	return (0);
}

void	fin_flags_up(t_args *a)
{
	pthread_mutex_lock(&a->shared->mtx_fin_flags);
	a->shared->fin_flags[a->tid] = 1;
	pthread_mutex_unlock(&a->shared->mtx_fin_flags);
}

void	*worker(void *args)
{
	t_args	*a;
	int		i;

	a = (t_args *)args;
	i = 0;
	if (a->shared->number_of_times_each_philosopher_must_eat < 0)
	{
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
		if (i >= a->shared->number_of_times_each_philosopher_must_eat)
			fin_flags_up(a);
	}
	return (NULL);
}
