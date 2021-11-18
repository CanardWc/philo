/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_needs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrea <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:55:29 by fgrea             #+#    #+#             */
/*   Updated: 2021/11/18 17:01:37 by fgrea            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	death = 0;

/*
 *	Handle talking (obviously) but also dying gestion. A mutex is used for 
 *		avoiding a scramble view on the STDOUT.
 */

int	ph_talking(t_sets data, char *str)
{
	pthread_mutex_lock(data.talk);
	if (!death)
	{
		if (!ft_strncmp("died\n", str, 5))
		{
			while ((ph_get_time() - data.start_time - data.time) < data.t_die)
				;
			death = 1;
		}
		printf("%d %d %s", ph_get_time() - data.start_time, data.nbr + 1, str);
	}
	pthread_mutex_unlock(data.talk);
	if (death)
		return (-1);
	return (0);
}

/*
 *	Handle the picking of the right forks for the right philosopher, it also
 *		avoid having all philosophers taking one fork at the same time and
 *		blocking the logic.
 */

void	ph_take_fork(t_sets data)
{
	if ((data.nbr % 2))
	{
		if (data.nbr - 1 < 0)
		{
			pthread_mutex_lock(&(data.forks[data.n_philo - 1]));
			//printf("%d %d took fork %d\n", (ph_get_time() - data.start_time - data.time), data.nbr + 1, data.n_philo - 1);
		}
		else
		{
			pthread_mutex_lock(&(data.forks[data.nbr - 1]));
			//printf("%d %d took fork %d\n", (ph_get_time() - data.start_time - data.time), data.nbr + 1, data.nbr - 1);
		}
		ph_talking(data, "has taken a fork\n");
	}
	pthread_mutex_lock(&data.forks[data.nbr]);
	//printf("%d %d took fork %d\n", (ph_get_time() - data.start_time - data.time), data.nbr + 1, data.nbr);
	ph_talking(data, "has taken a fork\n");
	if (!(data.nbr % 2))
	{
		if (data.nbr - 1 < 0)
		{
			pthread_mutex_lock(&(data.forks[data.n_philo - 1]));
			//printf("%d %d took fork %d\n", (ph_get_time() - data.start_time - data.time), data.nbr + 1, data.n_philo - 1);
		}
		else
		{
			pthread_mutex_lock(&(data.forks[data.nbr - 1]));
			//printf("%d %d took fork %d\n", (ph_get_time() - data.start_time - data.time), data.nbr + 1, data.nbr - 1);
		}
		ph_talking(data, "has taken a fork\n");
	}
}

/*
 *	Handle the sleeping and thinking part (what a surprise).
 */

int	ph_sleeping_and_thinking(t_sets data)
{
	int	time;

	if (death)
		return(-1);
	if ((ph_get_time() - data.start_time - data.time) >= data.t_die)
		return (ph_talking(data, "died\n"));
	//printf("b sleep = %d\n", (ph_get_time() - data.start_time - data.time));
	ph_talking(data, "is sleeping\n");
	time = ph_get_time();
	while ((ph_get_time() - time) < data.t_sleep)
		if ((ph_get_time() - data.start_time - data.time) >= data.t_die)
			return (ph_talking(data, "died\n"));
	ph_talking(data, "is_thinking\n");
	return (0);
}

/*
 *	Handle eating part, which is the true challenge of the project.
 */

int	ph_eating(t_sets data)
{
	int	time;

	if (death)
		return (-1);
	if ((ph_get_time() - data.start_time - data.time) /*+ (data.t_fork)*/ > data.t_die)
	{
		printf("died before forks\n");
		return (ph_talking(data, "died\n"));
	}
	ph_take_fork(data);
	ph_talking(data, "is eating\n");
	data.time = ph_get_time() - data.start_time;
	time = ph_get_time();
	while ((ph_get_time() - time) < data.t_eat)
		if ((ph_get_time() - data.start_time - data.time) >= data.t_die)
			data.time = ph_talking(data, "died\n");
	pthread_mutex_unlock(&(data.forks[data.nbr]));
	if (data.nbr - 1 < 0)
		pthread_mutex_unlock(&(data.forks[data.n_philo - 1]));
	else
		pthread_mutex_unlock(&(data.forks[data.nbr - 1]));
	return (data.time);
}
