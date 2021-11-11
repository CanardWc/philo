/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrea <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 09:39:42 by fgrea             #+#    #+#             */
/*   Updated: 2021/11/11 11:31:53 by fgrea            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	ph_gettime(t_sets *data)
{
	timeval	time_dif;
	int		die_time;

	if (gettimeofday(time_dif, NULL))
			return (-1);
	die_time = (data->start_time - (time_diff.usec / 1000)) - data->time;
	if (die_time >= data->t_die)
		return (-1);
	data->time = (data->start_time - (time_diff.usec / 1000));
	return (0);
}

int	ph_eating(t_sets *data)
{
	while (pthread_mutex_trylock(&data->fork[data->nbr]))
		if (ph_gettime(data))
			return (-1);
	printf("%d %d has taken a fork\n", data->time, data->nbr);
	if (data->nbr + 1 > data->n_philo)
	{
		while (pthread_mutex_trylock(&data->fork[0]))
			if (ph_gettime(data))
				return (-1);
	}
	else
	{
		while (pthread_mutex_trylock(&data->fork[data->nbr + 1]))
			if (ph_gettime(data))
				return (-1);
	}
	printf("%d %d has taken a fork\n", data->time, data->nbr);
	printf("%d %d is eating\n", data->time, data->nbr);
	usleep(data->t_eat);
	return (0);
}
