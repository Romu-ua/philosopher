/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:23:05 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:23:06 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_args	*args_init(t_shared *shared, int *is_miss_init)
{
	t_args	*args;
	int		i;
	int		n;

	if (!shared || *is_miss_init)
		return (NULL);
	n = shared->number_of_philosophers;
	args = (t_args *)malloc(sizeof(t_args) * n);
	if (!args)
	{
		*is_miss_init = 1;
		return (args);
	}
	i = 0;
	while (i < n)
	{
		args[i].last_eat_ts
			= (struct timeval *)malloc(sizeof(struct timeval));
		if (!args[i].last_eat_ts)
			*is_miss_init = 1;
		if (pthread_mutex_init(&args[i].mtx_last_eat_ts, NULL) != 0)
			*is_miss_init = 1;
		i++;
	}
	return (args);
}
