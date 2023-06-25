/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 17:01:44 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 01:48:03 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philos
{
	struct s_data	*data;
	int				id;
	pthread_t		tphilo;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	long long		start_time;
	long long		last_meal_time;
	int				meals_eaten;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;

}					t_philos;

typedef struct s_data
{
	int				philo_num;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				eat_rounds;
	int				dead;
	int				infinity;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	last_meal_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	*forks;
	t_philos		*philos;
}					t_data;

void				ft_sleep(long long time);
void				free_data(t_data *data);
int					ft_atoi(const char *str);
long long			gettime(void);
int					ft_thinking(t_philos *philo);
int					ft_forks(t_philos *philo);
int					ft_eating(t_philos *philo);
int					ft_sleeping(t_philos *philo);
int					ft_dropf(t_philos *philo);
int					ft_isdying(t_philos *philo);
int					ft_onecase(t_philos *philo);
int					philo_status(t_data *data);
int					ft_food(t_data *data);
int					ft_fpack(int ac, char **av, t_data *data);
int					init_mutex(t_data *data);
int					init_philosophers(t_data *data);
int					arg_checker(int ac, char **av);
int					init_data(t_data *data);
int					init_threads(t_data *data);
int					ft_fpack(int ac, char **av, t_data *data);
void				*routine(void *philos);
int					ft_routine_looper(t_philos *philo);
int					check_data(int ac, char **av, t_data *data);

#endif