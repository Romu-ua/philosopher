/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:23:12 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:23:13 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	args_free(t_args *args, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (args[i].last_eat_ts)
			free(args[i].last_eat_ts);
		pthread_mutex_destroy(&args[i].mtx_last_eat_ts);
		i++;
	}
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
	n = shared->number_of_philosophers;
	args_free(args, n);
}
