/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destory.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:37:09 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/13 18:37:09 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_destroy(pthread_mutex_t *mtx)
{
	if (mtx)
		pthread_mutex_destroy(mtx);
}

void	safe_args_free(t_args *args, int n)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < n)
	{
		if (args[i].last_eat_ts)
			free(args[i].last_eat_ts);
		safe_destroy(&args[i].mtx_last_eat_ts);
		i++;
	}
}

void	safe_shared_free(t_shared *shared)
{
	int	i;
	int	n;

	if (!shared)
		return ;
	safe_destroy(&shared->mtx_printf);
	safe_destroy(&shared->mtx_fin_flags);
	safe_destroy(&shared->mtx_waiting);
	if (shared->mtx)
	{
		n = shared->number_of_philosophers;
		i = 0;
		while (i < n)
			safe_destroy(&shared->mtx[i++]);
		free(shared->mtx);
	}
	if (shared->fin_flags)
		free(shared->fin_flags);
	free(shared);
}

void	destory(t_args *args, pthread_t *th)
{
	t_shared	*shared;
	int			n;

	if (!args)
	{
		if (th)
			free(th);
		return ;
	}
	shared = args->shared;
	if (shared)
		n = shared->number_of_philosophers;
	else
		n = 0;
	safe_args_free(args, n);
	free(args);
	safe_shared_free(shared);
	if (th)
		free(th);
}
