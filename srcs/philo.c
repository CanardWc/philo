#include <philo.h>

void	ph_error()
{
	if (errno == 0)
		printf("Wrong arguments\n");
	else
		perror(strerror(errno));
	exit(0);
}

void	ph_check_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6)
		ph_error();
	i = 0;
	while (++i < ac)
	{
		j = 0;
		while (av[i][j])
			if (av[i][j] < '0' || av[i][j++] > '9')
				ph_error();
	}
}

t_sets	ph_get_static_data(int ac, char **av)
{
	t_sets			data;
	struct timeval	time;
	int				i;

	if (ft_atoi(av[1]) <= 0)
		ph_error();
	if (ac < 6)
		data.must_eat = -1;
	else
		data.must_eat = ft_atoi(av[5]);
	data.t_sleep = ft_atoi(av[4]);
	data.t_eat = ft_atoi(av[3]);
	data.t_die = ft_atoi(av[2]);
	data.n_philo = ft_atoi(av[1]);
	if (gettimeofday(&time, NULL))
		ph_error();
	data.start_time = time.tv_usec / 1000;
	data.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data.n_philo);
	if (!data.forks)
		ph_error();
	i = 0;
	while (i < data.n_philo)
		if (pthread_mutex_init(&data.forks[i++], NULL))
			ph_error();
	return (data);
}

t_sets	*ph_get_data(int ac, char **av)
{
	t_sets			*ret;
	t_sets			data;
	int				i;

	data = ph_get_static_data(ac, av);
	data.life = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data.n_philo);
	if (!data.life)
		ph_error();
	i = 0;
	while (i < data.n_philo)
		if (pthread_mutex_init(&data.life[i++], NULL))
			ph_error();
	ret = (t_sets *)malloc(sizeof(t_sets) * data.n_philo);
	if (!ret)
		ph_error();
	i = -1;
	while (++i < data.n_philo)
	{
		ret[i] = data;
		ret[i].nbr = i + 1;
	}
	return (ret);
}

int	main(int ac, char **av)
{
	t_sets	*data;

	//check args
	ph_check_args(ac, av);

	//get data
	data = ph_get_data(ac, av);
	ft_printf("n_philo = %d\n", data->n_philo);
	ft_printf("t_die = %d\n", data->t_die);
	ft_printf("t_eat = %d\n", data->t_eat);
	ft_printf("t_sleep = %d\n", data->t_sleep);
	ft_printf("must_eat = %d\n", data->must_eat);

	//launching logic
	ph_logic(data);
	return (0);
}
