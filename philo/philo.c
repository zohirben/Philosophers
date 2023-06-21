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

int check_data(int ac, char **av, t_data *data)
{
    data->philo_num = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);
    data->time_to_eat = ft_atoi(av[3]);
    data->time_to_sleep = ft_atoi(av[4]);
    data->dead = 0;
    if (ac == 6)
    {
        data->eat_rounds = ft_atoi(av[5]) * data->philo_num;
        data->infinity = 0;
        if (data->eat_rounds < 0)
            return (1);
    }
    else
    {
        data->eat_rounds = -1;
        data->infinity = 1;
    }
    if (data->philo_num < 0 || data->time_to_die < 0 || data->time_to_eat < 0 || data->time_to_sleep < 0 || data->philo_num > 200)
        return (1);
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
        philo->last_meal_time = gettime();
        philo->l_fork = &(data->forks[i]);
        philo->r_fork = &(data->forks[(i + 1) % data->philo_num]);
    }
    return (0);
}

void ft_thinking(t_philos *philo)
{
    pthread_mutex_lock(&(philo->data->lock));
    if (philo->data->dead != 1)
    {
        printf("%lli %i is thinking\n", (gettime() - philo->start_time), philo->id);
        pthread_mutex_unlock(&(philo->data->lock));
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->lock));
        return;
    }
}

void ft_eating(t_philos *philo)
{
    pthread_mutex_lock(&(philo->data->lock));
    if (philo->data->dead != 1)
    {
        printf("%lli %i is eating\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_unlock(&(philo->data->lock));
        pthread_mutex_lock(&(philo->data->meal_lock));
        philo->last_meal_time = gettime();
        pthread_mutex_unlock(&(philo->data->meal_lock));
        pthread_mutex_lock(&(philo->data->meal_lock));
        philo->data->eat_rounds--;
        pthread_mutex_unlock(&(philo->data->meal_lock));
        ft_sleep(philo->eat_time);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->lock));
        return;
    }
}

void ft_sleeping(t_philos *philo)
{

    pthread_mutex_lock(&(philo->data->lock));
    if (philo->data->dead != 1)
    {
        printf("%lli %i is sleeping\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_unlock(&(philo->data->lock));
        ft_sleep(philo->sleep_time);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->lock));
        return;
    }
}

void ft_forks(t_philos *philo)
{
    pthread_mutex_lock(&(philo->data->lock));
    if (philo->data->dead != 1)
    {
        pthread_mutex_unlock(&(philo->data->lock));
        pthread_mutex_lock(philo->l_fork);
        printf("%lli %i has taken a fork\n", gettime() - philo->start_time, philo->id);
        pthread_mutex_lock(philo->r_fork);
        printf("%lli %i has taken a fork\n", gettime() - philo->start_time, philo->id);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->lock));
        return;
    }
}

void ft_dropf(t_philos *philo)
{
    pthread_mutex_lock(&(philo->data->lock));
    if (philo->data->dead != 1)
    {
        pthread_mutex_unlock(&(philo->data->lock));
        pthread_mutex_unlock(philo->l_fork);
        pthread_mutex_unlock(philo->r_fork);
    }
    else
    {
        pthread_mutex_unlock(&(philo->data->lock));
        return;
    }
}

void *routine(void *philos)
{
    t_philos *philo;

    philo = (t_philos *)philos;
    while (1)
    {
        pthread_mutex_lock(&(philo->data->lock));
        if (philo->data->dead == 1)
        {
            pthread_mutex_unlock(&(philo->data->lock));
            pthread_detach(philo->tphilo);
            return (NULL);
        }
        else
        {
            pthread_mutex_unlock(&(philo->data->lock));
            ft_thinking(philo);
            ft_forks(philo);
            ft_eating(philo);
            ft_dropf(philo);
            ft_sleeping(philo);
        }
    }
    return (NULL);
}

int ft_isdying(t_philos *philo)
{
    int time;

    pthread_mutex_lock(&(philo->data->meal_lock));
    time = gettime() - philo->last_meal_time;
    pthread_mutex_unlock(&(philo->data->meal_lock));
    if (time >= philo->die_time)
    {
        pthread_mutex_lock(&(philo->data->lock));
        philo->data->dead = 1;
        pthread_mutex_unlock(&(philo->data->lock));
        printf("%lli %i died\n", gettime() - philo->start_time, philo->id);
        return (1);
    }
    pthread_mutex_unlock(&(philo->data->lock));
    return (0);
}

int philo_status(t_data *data)
{
    int i = 0;
    while (1)
    {
        if (ft_isdying(&data->philos[i]) == 1)
            break;
        pthread_mutex_lock(&data->meal_lock);
        if (data->eat_rounds <= 0 && data->infinity == 0)
        {
            pthread_mutex_unlock(&data->meal_lock);
            break;
        }
        pthread_mutex_unlock(&data->meal_lock);
        i++;
        if (i == data->philo_num - 1)
        {
            usleep(200);
            i = 0;
        }
    }
    return (1);
}

int init_threads(t_data *data)
{
    int i;

    i = -1;

    if (pthread_mutex_init(&(data->lock), NULL) != 0)
        return (1);
    if (pthread_mutex_init(&(data->meal_lock), NULL) != 0)
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
        pthread_create(&(data->philos[i].tphilo), NULL, routine, (void *)&data->philos[i]);
        usleep(300);
    }
    if (philo_status(data) == 1)
        return (1);
    i = -1;
    while (++i < data->philo_num)
    {
        if (pthread_join(data->philos[i].tphilo, NULL))
            return (1);
    }
    return (1);
}

void free_data(t_data *data)
{
    int i = 0;

    while (i < data->philo_num)
    {
        pthread_mutex_destroy(&data->forks[i++]);
    }
    pthread_mutex_destroy(&data->lock);
    pthread_mutex_destroy(&data->meal_lock);
    free(data->forks);
    free(data->philos);
}

int main(int ac, char **av)
{
    t_data data;

    (void)data;
    if (arg_checker(ac, av) == 1)
        return (1);
    if (check_data(ac, av, &data) == 1)
        return (1);
    if (init_data(&data) == 1)
        return (1);
    if (init_philosophers(&data) == 1)
        return (1);
    if (init_threads(&data) == 1)
        free_data(&data);
    return (0);
}