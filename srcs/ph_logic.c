#include <philo.h>

printf("%d %d died\n", data.nbr);

void	eating(t_sets *data)
{
	pthread_mutex_lock(&data.forks[data->nbr]);
	printf("%d %d has taken a fork\n", time, data->nbr);
	if (data->nbr + 1 > data->n_philo)
		pthread_mutex_lock(&data->forks[0]);
	else
		pthread_mutex_lock(&data.forks[data->nbr + 1]);
	printf("%d %d has taken a fork\n", time, data->nbr);
	printf("%d %d is eating\n", time, data->nbr);
	usleep(data->t_eat * 1000);
	pthread_mutex_unlock(&data->forks[data->nbr]);
	if (data->nbr + 1 > data->n_philo)
		pthread_mutex_unlock(&data->forks[0]);
	else
		pthread_mutex_unlock(&data->forks[data->nbr + 1]);
	if (data->must_eat > 0)
		data->must_eat--;
}

void	*ph_alive(void *arg)
{
	t_sets	data;
	int		time;

	data = *(t_sets *)(arg);
	data.t_sleep = 3;
	ft_printf("coucou\n");
	while (data.must_eat != 0)
	{
		ph_eating(&data);
		if (gettimeofday(&time, NULL))
			ph_error();
		printf("%d %d is sleeping\n", time, data.nbr);
		usleep(data.t_sleep * 1000);
		printf("%d %d is thinking\n", time, data.nbr);
	}
	return (arg);
}

void	ph_logic(t_sets data)
{
	pthread_t	*thread_ids;
	int		err;
	int		i;

	thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * data.n_philo);
	if (thread_ids == NULL)
		ph_error();
	data.forks = (int *)malloc(sizeof(int), data.n_philo);
	if (data.forks == NULL)
		ph_error();
	i = 0;
	while (i < data.n_philo)
	{
		data.nbr = i;
		if (pthread_create(&thread_ids[i++], NULL, &ph_alive, &data))
			ph_error();
	}
	i = 0;
	while (i < data.n_philo)
		if (pthread_join(thread_ids[i++], NULL))
			ph_error();
	free(thread_ids);
	free(data.forks);
}
