/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_logic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrea <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 11:32:10 by fgrea             #+#    #+#             */
/*   Updated: 2021/11/16 18:27:42 by fgrea            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	death = 0;

int	ph_check_death(t_sets data, int death)
{
	if (data.n_philo == 1)
	{
		ph_talking(data, "has taken a fork\n", death);
		usleep(data.t_die * 1000);
		ph_talking(data, "died\n", death);
		death = 1;
		return (-1);
	}
	if (death > 0)
		return (-1);
	printf("check death : %d\n", (ph_get_time() - data.start_time) - data.time);
	if (((ph_get_time() - data.start_time) - data.time) > data.t_die)
	{
		if (!death)
			ph_talking(data, "died\n", death);
		death = 1;
		return (-1);
	}
	return (0);
}

int	ph_eating(t_sets *data)
{
	pthread_mutex_lock(&data->forks[data->nbr]);
	if (ph_check_death(*data, death))
		return (-1);
	ph_talking(*data, "has taken a fork\n", death);
	if (data->nbr + 1 >= data->n_philo)
		pthread_mutex_lock(&data->forks[0]);
	else
		pthread_mutex_lock(&data->forks[data->nbr + 1]);
	if (ph_check_death(*data, death))
		return (-1);
	ph_talking(*data, "has taken a fork\n", death);
	ph_talking(*data, "is eating\n", death);
	usleep(data->t_eat * 1000);
	pthread_mutex_unlock(&data->forks[data->nbr]);
	if (data->nbr + 1 >= data->n_philo)
		pthread_mutex_unlock(&data->forks[0]);
	else
		pthread_mutex_unlock(&data->forks[data->nbr + 1]);
	if (data->must_eat > 0)
		data->must_eat--;
	return (0);
}

void	*ph_alive(void *arg)
{
	t_sets	data;

	data = *(t_sets *)(arg);
	data.time = 0;
	while (data.n_philo != 1 && data.must_eat != 0)
	{
		if (ph_check_death(data, death))
			break;
		if (ph_eating(&data))
			break;
		ph_update_time(&data);
		ph_talking(data, "is sleeping\n", death);
		usleep(data.t_sleep * 1000);
		if (ph_check_death(data, death))
			break;
		ph_talking(data, "is thinking\n", death);
	}
	return (NULL);
}

void	ph_logic(t_sets *data)
{
	pthread_t	*thread_ids;
	int		i;

	thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * data->n_philo);
	if (thread_ids == NULL)
		ph_error();
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&thread_ids[i], NULL, &ph_alive, &data[i]))
			ph_error();
		if (!(i % 2))
			usleep(20);
	}
	i = 0;
	while (i < data->n_philo)
		if (pthread_join(thread_ids[i++], NULL))
			ph_error();
	free(thread_ids);
	free(data->forks);
	free(data);
}
