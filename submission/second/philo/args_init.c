/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:20:32 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/13 19:20:32 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	args_cleanup(t_args *args, int n)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < n)
	{
		if (args[i].last_eat_ts)
			free(args[i].last_eat_ts);
		i++;
	}
	free(args);
}

t_args	*args_init(t_shared *shared, int *is_miss_init)
{
	t_args	*args;
	int		i;

	if (!shared)
		return (*is_miss_init = 1, NULL);
	args = (t_args *)malloc(sizeof(t_args) * shared->number_of_philosophers);
	if (!args)
		return (*is_miss_init = 1, NULL);
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		args[i].last_eat_ts
			= (struct timeval *)malloc(sizeof(struct timeval));
		if (!args[i].last_eat_ts)
			return (*is_miss_init = 1, args_cleanup(args, i), NULL);
		shared->fin_flags[i] = 0;
		if (pthread_mutex_init(&args[i].mtx_last_eat_ts, NULL) != 0)
			return (*is_miss_init = 1, args_cleanup(args, i + 1), NULL);
		i++;
	}
	return (args);
}
