/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:39:53 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/13 19:39:55 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*util_return(int *is_miss_init, t_shared *shared)
{
	*is_miss_init = 1;
	shared_cleanup(shared);
	return (NULL);
}

void	*cleanup_return(t_shared *shared)
{
	shared_cleanup(shared);
	return (NULL);
}

void	th_init(t_shared *shared, pthread_t **th, int *is_miss_init)
{
	int	n;

	n = shared->number_of_philosophers;
	*th = (pthread_t *)malloc(sizeof(pthread_t) * n);
	if (!*th)
		*is_miss_init = 1;
}
