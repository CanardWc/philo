#include <philo.h>

int	death = 0;

int	ph_talking(t_sets data, char *str)
{
	char	*s = "died\n";

	pthread_mutex_lock(data.talk);
	if (!death)
	{
		if (!ft_strncmp(s, str, 5))
			death = 1;
		printf("%d %d %s", ph_get_time() - data.start_time, data.nbr + 1, str);
	}
	pthread_mutex_unlock(data.talk);
	if (death)
		return (-1);
	return (0);
}

void	ph_take_fork(t_sets data)
{
	if ((data.nbr % 2))
	{
		pthread_mutex_lock(&data.forks[(data.nbr - 1) % data.n_philo]);
		ph_talking(data, "has taken a fork\n");
	}
	pthread_mutex_lock(&data.forks[data.nbr]);
	ph_talking(data, "has taken a fork\n");
	if (!(data.nbr % 2))
	{
		pthread_mutex_lock(&data.forks[(data.nbr - 1) % data.n_philo]);
		ph_talking(data, "has taken a fork\n");
	}
}

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

int	ph_eating(t_sets data)
{
	int	time;

	if (death)
		return (-1);
	if ((ph_get_time() - data.start_time - data.time) > data.t_die)
		return (ph_talking(data, "died\n"));
	ph_take_fork(data);
	ph_talking(data, "is eating\n");
	data.time = ph_get_time() - data.start_time;
	//printf("d.time = %d\n", data.time);
	time = ph_get_time();
	while ((ph_get_time() - time) < data.t_eat)
		if ((ph_get_time() - data.start_time - data.time) >= data.t_die)
			data.time = ph_talking(data, "died\n");
	//printf("time eat = %d\n", (ph_get_time() - data.start_time - data.time));
	pthread_mutex_unlock(&(data.forks[data.nbr]));
	if (data.nbr - 1 < 0)
		pthread_mutex_unlock(&(data.forks[data.n_philo]));
	else
		pthread_mutex_unlock(&(data.forks[data.nbr - 1]));
	return (data.time);
}
