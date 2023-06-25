/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 01:50:27 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 01:50:30 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_threads(t_data *data)
{
	int	i;

	if (init_mutex(data) == 1)
		return (1);
	i = -1;
	while (++i < data->philo_num)
	{
		if (data->philos[i].id % 2 == 0)
			usleep(200);
		if (pthread_create(&(data->philos[i].tphilo), NULL, routine,
				&data->philos[i]) != 0)
		{
			return (1);
		}
	}
	philo_status(data);
	i = 0;
	while (i < data->philo_num)
	{
		pthread_detach(data->philos[i].tphilo);
		i++;
	}
	return (0);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&(data->print_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(data->last_meal_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(data->meal_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(data->dead_lock), NULL) != 0)
		return (1);
	while (++i < data->philo_num)
		if (pthread_mutex_init(&(data->forks[i]), NULL) != 0)
			return (1);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int			i;
	t_philos	*philo;

	i = -1;
	while (++i < data->philo_num)
	{
		philo = &(data->philos[i]);
		philo->data = data;
		philo->id = i + 1;
		philo->die_time = data->time_to_die;
		philo->eat_time = data->time_to_eat;
		philo->sleep_time = data->time_to_sleep;
		philo->start_time = gettime();
		philo->meals_eaten = 0;
		philo->last_meal_time = gettime();
		philo->l_fork = &(data->forks[i]);
		philo->r_fork = &(data->forks[(i + 1) % data->philo_num]);
	}
	if (ft_onecase(&data->philos[0]) == 1)
		return (1);
	return (0);
}

int	init_data(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (data->forks == NULL)
		return (1);
	data->philos = malloc(sizeof(t_philos) * data->philo_num);
	if (data->philos == NULL)
	{
		free(data->forks);
		return (1);
	}
	return (0);
}

int	arg_checker(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6)
		return (1);
	i = 0;
	while (av[++i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] >= '0' && av[i][j] <= '9')
				j++;
			else
				return (1);
		}
	}
	return (0);
}
