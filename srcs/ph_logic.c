#include <philo.h>

void	*ph_alive(void *arg)
{
	t_sets	data;

	data = *(t_sets *)(arg);
	data.t_sleep = 3;
	ft_printf("coucou\n");
	while (data.must_eat != 0)
	{
		
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
	i = 0;
	while (i < data.n_philo)
		if (pthread_create(&thread_ids[i++], NULL, &ph_alive, &data))
			ph_error();
	i = 0;
	while (i < data.n_philo)
		if (pthread_join(thread_ids[i++], NULL))
			ph_error();
}
