/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_and_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 13:20:15 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/10/31 13:26:54 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_cleanup_philosophers(t_philo *philos, int n)
{
	int	i;

	if (!philos)
		return ;
	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i].last_eat_mutex);
		i++;
	}
	free(philos);
}

void	ft_cleanup_table(t_table *table, int number_of_philos)
{
	int	i;

	if (!table)
		return ;
	if (table->id_fork)
	{
		i = 0;
		while (i < number_of_philos)
		{
			pthread_mutex_destroy(&table->id_fork[i]);
			i++;
		}
		free(table->id_fork);
		table->id_fork = NULL;
	}
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->dep_mutex);
}

void	ft_destroy_table_forks(t_table *table, int forks_initialized)
{
	int	i;

	if (table->id_fork)
	{
		i = 0;
		while (i < forks_initialized)
		{
			pthread_mutex_destroy(&table->id_fork[i]);
			i++;
		}
		free(table->id_fork);
		table->id_fork = NULL;
	}
}

int	ft_init_table(t_table *table, t_parameters *params)
{
	int	i;

	i = 0;
	if (!table || !params)
		return (1);
	table->id_fork = malloc(sizeof(pthread_mutex_t) * params->number_of_philos);
	if (!table->id_fork)
		return (1);
	while (i < params->number_of_philos)
	{
		if (pthread_mutex_init(&table->id_fork[i], NULL) != 0)
			return (ft_destroy_table_forks(table, i), 1);
		i++;
	}
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (ft_destroy_table_forks(table, params->number_of_philos), 1);
	if (pthread_mutex_init(&table->dep_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_mutex);
		ft_destroy_table_forks(table, params->number_of_philos);
		return (1);
	}
	table->someone_died = 0;
	table->start_time_ms = ft_time_now_ms();
	return (0);
}
