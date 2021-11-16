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
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				must_eat;
	int				start_time;
	
	int				nbr;
	int				time;

	pthread_mutex_t	life;	
	pthread_mutex_t	*forks;
}	t_sets;

void	ph_error();

int		ph_get_time();
void	ph_update_time(t_sets *data);
int		ph_check_death(t_sets data, int death);
void	ph_talking(t_sets data, char *str, int death);

void	ph_logic(t_sets *data);

#endif
