/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   th_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:33 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:33 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

pthread_t	*th_init(t_shared *shared, int *is_miss_init)
{
	pthread_t	*th;
	int			n;

	th = NULL;
	n = shared->number_of_philosophers;
	th = (pthread_t *)malloc(sizeof(pthread_t) * n);
	if (!th)
		*is_miss_init = 1;
	return (th);
}
