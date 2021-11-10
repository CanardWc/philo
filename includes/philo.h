#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <libft.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_sets
{
	int	n_philo;
	int	t_die;
	int	t_eat;
	int	t_sleep;
	int	must_eat;

	int	*forks;
	int	nbr;
}		t_sets;

void	ph_error();

void	ph_logic();

#endif
