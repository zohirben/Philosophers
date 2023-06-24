/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 00:20:32 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 00:45:35 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int arg_checker(int ac, char **av)
{
    int i;
    int j;

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

int ft_onecase(t_philos *philo)
{
    if (philo->data->philo_num == 1)
    {
        printf("1 1 is thinking\n");
        printf("1 1 has taken a fork\n");
        ft_sleep(philo->die_time);
        printf("%i 1 died\n", philo->die_time);
        return (1);
    }
    return (0);
}

int check_data(int ac, char **av, t_data *data)
{
    data->philo_num = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);
    data->time_to_eat = ft_atoi(av[3]);
    data->time_to_sleep = ft_atoi(av[4]);
    data->dead = 0;
    if (ac == 6)
    {
        data->eat_rounds = ft_atoi(av[5]);
        data->infinity = 0;
        if (data->eat_rounds < 0)
            return (1);
    }
    else
    {
        data->eat_rounds = -1;
        data->infinity = 1;
    }
    if (data->philo_num <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0 ||
        data->time_to_sleep <= 0 || data->philo_num > 200)
    {
        return (1);
    }
    return (0);
}

int init_data(t_data *data)
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

int init_philosophers(t_data *data)
{
    int i;

    i = -1;
    while (++i < data->philo_num)
    {
        t_philos *philo = &(data->philos[i]);

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

int ft_thinking(t_philos *philo)
{
    pthread_mutex_lock(&(philo->data->dead_lock));
    if (philo->data->dead != 1)
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        pthread_mutex_lock(&(philo->data->print_lock));
        printf("%lli %i is thinking\n", (gettime() - philo->start_time), philo->id);
        pthread_mutex_unlock(&(philo->data->print_lock));
        return (0);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        return (1);
    }
}

int ft_eating(t_philos *philo)
{
    pthread_mutex_lock(&(philo->data->dead_lock));
    if (philo->data->dead != 1)
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        pthread_mutex_lock(&(philo->data->print_lock));
        printf("%lli %i is eating\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_unlock(&(philo->data->print_lock));
        pthread_mutex_lock(&(philo->data->last_meal_lock));
        philo->last_meal_time = gettime();
        pthread_mutex_unlock(&(philo->data->last_meal_lock));
        pthread_mutex_lock(&(philo->data->meal_lock));
        philo->meals_eaten++;
        pthread_mutex_unlock(&(philo->data->meal_lock));
        ft_sleep(philo->eat_time);
        return (0);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        return (1);
    }
}

int ft_sleeping(t_philos *philo)
{

    pthread_mutex_lock(&(philo->data->dead_lock));
    if (philo->data->dead != 1)
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        pthread_mutex_lock(&(philo->data->print_lock));
        printf("%lli %i is sleeping\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_unlock(&(philo->data->print_lock));
        ft_sleep(philo->sleep_time);
        return (0);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        return (1);
    }
}

int ft_forks(t_philos *philo)
{
    
    pthread_mutex_lock(&(philo->data->dead_lock));
    if (philo->data->dead != 1)
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        pthread_mutex_lock(philo->l_fork);
        pthread_mutex_lock(&(philo->data->print_lock));
        printf("%lli %i has taken a fork\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_unlock(&(philo->data->print_lock));
        pthread_mutex_lock(philo->r_fork);
        pthread_mutex_lock(&(philo->data->print_lock));
        printf("%lli %i has taken a fork\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_unlock(&(philo->data->print_lock));
        return (0);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->dead_lock));
        return (1);
    }
}

int ft_dropf(t_philos *philo)
{
    pthread_mutex_unlock(philo->l_fork);
    pthread_mutex_unlock(philo->r_fork);
    return (0);
}

void *routine(void *philos)
{
    t_philos *philo;

    philo = (t_philos *)philos;
    while (1)
    {
        pthread_mutex_lock(&(philo->data->dead_lock));
        if (philo->data->dead == 1)
        {
            pthread_mutex_unlock(&(philo->data->dead_lock));
            return (NULL);
        }
        else
        {
            pthread_mutex_unlock(&(philo->data->dead_lock));
            if (ft_thinking(philo) == 1)
                return (NULL);
            if (ft_forks(philo) == 1)
                return (NULL);
            if (ft_eating(philo) == 1)
                return (NULL);
            if (ft_dropf(philo) == 1)
                return (NULL);
            if (ft_sleeping(philo) == 1)
                return (NULL);
        }
    }
    return (NULL);
}

int ft_isdying(t_philos *philo)
{
    int time;

    pthread_mutex_lock(&(philo->data->last_meal_lock));
    time = gettime() - philo->last_meal_time;
    pthread_mutex_unlock(&(philo->data->last_meal_lock));
    if (time >= philo->die_time)
    {
        pthread_mutex_lock(&(philo->data->dead_lock));
        philo->data->dead = 1;
        // pthread_mutex_unlock(&(philo->data->dead_lock));
        pthread_mutex_lock(&(philo->data->print_lock));
        printf("%lli %i died\n", gettime() - philo->start_time, philo->id);
        // pthread_mutex_unlock(&(philo->data->print_lock));
        return (1);
    }
    return (0);
}

int ft_food(t_data *data)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (i < data->philo_num)
    {
        pthread_mutex_lock(&data->meal_lock);
        if (data->philos[i].meals_eaten >= data->eat_rounds)
            count++;
        pthread_mutex_unlock(&data->meal_lock);
        i++;
    }
    if (count == data->philo_num)
    {
        pthread_mutex_lock(&data->dead_lock);
        // printf("\t\tbtata\t\t\n");
        data->dead = 1;
        // pthread_mutex_unlock(&data->dead_lock);
        return (1);
    }
    return (0);
}

int philo_status(t_data *data)
{
    int i = 0;
    while (1)
    {
        if (ft_isdying(&data->philos[i]) == 1)
            break;
        if (data->infinity == 0 && data->eat_rounds != -1)
        {
            if (ft_food(data) == 1)
                break;
        }
        i++;
        if (i == data->philo_num - 1)
            i = 0;
    }
    return (0);
}

int init_threads(t_data *data)
{
    int i;

    i = -1;

    if (pthread_mutex_init(&(data->print_lock), NULL) != 0)
        return (1);
    if (pthread_mutex_init(&(data->last_meal_lock), NULL) != 0)
        return (1);
    if (pthread_mutex_init(&(data->meal_lock), NULL) != 0)
    {
        free_data(data);
        return (1);
    }
    if (pthread_mutex_init(&(data->dead_lock), NULL) != 0)
    {
        free_data(data);
        return (1);
    }
    while (++i < data->philo_num)
        if (pthread_mutex_init(&(data->forks[i]), NULL) != 0)
            return (1);
    i = -1;
    while (++i < data->philo_num)
    {
        if (data->philos[i].id % 2 == 0)
            usleep(200);
        if (pthread_create(&(data->philos[i].tphilo), NULL, routine, &data->philos[i]) != 0)
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

void free_data(t_data *data)
{
    int i = 0;

    while (i < data->philo_num)
        pthread_mutex_destroy(&data->forks[i++]);
    pthread_mutex_destroy(&data->print_lock);
    pthread_mutex_destroy(&data->dead_lock);
    pthread_mutex_destroy(&data->meal_lock);
    pthread_mutex_destroy(&data->last_meal_lock);
    free(data->forks);
    free(data->philos);
    free(data);
}

int main(int ac, char **av)
{
    t_data *data;

    data = malloc(sizeof(*data));
    if (data == NULL)
        return (1);
    if (arg_checker(ac, av) == 1)
    {
        free(data);
        return (1);
    }
    if (check_data(ac, av, data) == 1)
    {
        free(data);
        return (1);
    }
    if (init_data(data) == 1)
    {
        free(data);
        return (1);
    }
    if (init_philosophers(data) == 1)
    {
        free_data(data);
        return (1);
    }
    if (init_threads(data) == 1)
    {
        free_data(data);
        return (1);
    }
    free_data(data);
    return (0);
}