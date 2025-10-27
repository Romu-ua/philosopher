/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:56 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:57 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	process(void *args)
{
	if (ph_think(args))
		return (1);
	if (odd_case(args))
		return (1);
	if (ph_eat(args))
		return (1);
	if (ph_sleep(args))
		return (1);
	return (0);
}

void	run_unlimited(void *args)
{
	while (1)
	{
		if (process(args))
			break ;
	}
}

void	run_limited(void *args)
{
	t_args	*a;
	int		i;

	a = (t_args *)args;
	i = 0;
	while (i < a->shared->number_of_times_each_philosopher_must_eat)
	{
		if (process(args))
			break ;
		i++;
	}
	if (i >= a->shared->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&a->shared->mtx_fin_flags);
		a->shared->fin_flags[a->tid] = 1;
		pthread_mutex_unlock(&a->shared->mtx_fin_flags);
	}
}

void	*worker(void *args)
{
	t_args	*a;

	a = (t_args *)args;
	if (a->tid % 2)
		usleep(100);
	if (a->shared->number_of_times_each_philosopher_must_eat < 0)
		run_unlimited(args);
	else
		run_limited(args);
	return (NULL);
}
