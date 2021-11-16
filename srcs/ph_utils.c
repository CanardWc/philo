/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrea <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 09:39:42 by fgrea             #+#    #+#             */
/*   Updated: 2021/11/16 18:27:41 by fgrea            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	ph_get_time()
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
		ph_error();
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ph_update_time(t_sets *data)
{
	int	time;

	time = ph_get_time();
	data->time = time - data->start_time;
}

void	ph_talking(t_sets data, char *str, int death)
{
	if (!death || death == 3)
		printf("%d %d %s", ph_get_time() - data.start_time, data.nbr + 1, str);
	if (death == 3)
		usleep(data.t_die * 1000);
}
