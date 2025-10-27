/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:43:55 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/23 15:43:56 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int argc, char **argv)
{
	t_shared	*shared;
	pthread_t	*th;
	t_args		*args;
	int			is_miss_init;

	if (input_check(argc, argv))
		return (1);
	is_miss_init = 0;
	shared = shared_init(argc, argv, &is_miss_init);
	th = th_init(shared, &is_miss_init);
	args = args_init(shared, &is_miss_init);
	if (!args || is_miss_init)
	{
		destory(args, th);
		return (1);
	}
	make_thread(args, th, shared);
	destory(args, th);
	return (0);
}
