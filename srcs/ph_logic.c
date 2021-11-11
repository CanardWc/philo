/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_logic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrea <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 11:32:10 by fgrea             #+#    #+#             */
/*   Updated: 2021/11/11 14:41:28 by fgrea            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	ph_gettime(t_sets *data)
{
	struct timeval	time_dif;
	int				die_time;

	if (gettimeofday(&time_dif, NULL))
			return (-1);
	//printf("start_time = %d", data->start_time);
	die_time = (data->start_time - (time_dif.tv_usec / 1000)) - data->time;
	if (die_time >= data->t_die)
		return (-1);
	data->time = (data->start_time - (time_dif.tv_usec / 1000));
	return (0);
}

int	ph_eating(t_sets *data)
{
	while (pthread_mutex_trylock(&data->forks[data->nbr]))
		if (ph_gettime(data))
			return (-1);
	printf("%d %d has taken a fork\n", data->time, data->nbr);
	if (data->nbr + 1 > data->n_philo)
	{
		while (pthread_mutex_trylock(&data->forks[0]))
			if (ph_gettime(data))
				return (-1);
	}
	else
	{
		while (pthread_mutex_trylock(&data->forks[data->nbr + 1]))
			if (ph_gettime(data))
				return (-1);
	}
	printf("%d %d has taken a fork\n", data->time, data->nbr);
	printf("%d %d is eating\n", data->time, data->nbr);
	usleep(data->t_eat);
	return (0);
}

void	*ph_alive(void *arg)
{
	t_sets	data;

	data = *(t_sets *)(arg);
	data.time = 0;
	while (data.must_eat != 0)
	{
	//ft_printf("bonsoir %d\n", data.nbr);
		if (ph_eating(&data))
			return (&((t_sets *)arg)->nbr);
		else if (data.must_eat > 0)
			data.must_eat--;
		if (data.t_sleep > data.t_die)
			while (!ph_gettime(&data))
				;
		if (ph_gettime(&data))
			return (&((t_sets *)arg)->nbr);
		printf("%d %d is sleeping\n", data.time, data.nbr);
		usleep(data.t_sleep * 1000);
		printf("%d %d is thinking\n", data.time, data.nbr);
	}
	return (NULL);
}

void	ph_logic(t_sets *data)
{
	pthread_t	*thread_ids;
	int		err;
	int		i;

	thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * data->n_philo);
	if (thread_ids == NULL)
		ph_error();
	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_create(&thread_ids[i], NULL, &ph_alive, &data[i]))
			ph_error();
		//if (pthread_detach(thread_ids[i++]))
		//	ph_error();
	}
	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_join(thread_ids[i++], (void *)&err))
			ph_error();
		if (err && err > 0)
		{
			printf("%d %d died", data->time, err);
			exit(0);
		}
	}
	free(thread_ids);
	free(data->forks);
	free(data->life);
	free(data);
}
