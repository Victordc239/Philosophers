/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:12:47 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/10/31 13:25:47 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_parse_arguments(int argc, char **argv, t_parameters *parameters)
{
	if (!ft_is_positive_digits(argv[1]) || !ft_is_positive_digits(argv[2])
		|| !ft_is_positive_digits(argv[3]) || !ft_is_positive_digits(argv[4]))
		return (0);
	parameters->number_of_philos = ft_atoi(argv[1]);
	parameters->time_to_die = ft_atoi(argv[2]);
	parameters->time_to_eat = ft_atoi(argv[3]);
	parameters->time_to_sleep = ft_atoi(argv[4]);
	if (parameters->number_of_philos > 200)
		return (-1);
	if (parameters->time_to_die < 60 || parameters->time_to_eat < 60
		|| parameters->time_to_sleep < 60)
		return (-2);
	if (argc == 6)
	{
		if (!ft_is_positive_digits(argv[5]))
			return (0);
		parameters->count_must_eat = ft_atoi(argv[5]);
	}
	else
		parameters->count_must_eat = -1;
	return (1);
}

int	ft_init_value_philos(t_philo **philos, t_table *table, t_parameters *params)
{
	int	i;

	i = 0;
	*philos = malloc(sizeof(t_philo) * params->number_of_philos);
	if (!*philos)
		return (1);
	while (i < params->number_of_philos)
	{
		(*philos)[i].id_philo = i + 1;
		(*philos)[i].parameters = params;
		(*philos)[i].table = table;
		(*philos)[i].left_fork = &table->id_fork[i];
		(*philos)[i].right_fork
			= &table->id_fork[(i + 1) % params->number_of_philos];
		(*philos)[i].last_eat_ms = table->start_time_ms;
		(*philos)[i].eat_count = 0;
		(*philos)[i].trying_to_eat = 0;
		pthread_mutex_init(&(*philos)[i].last_eat_mutex, NULL);
		i++;
	}
	return (0);
}

int	ft_start_philos_threads(t_philo *philos, int number_of_philos)
{
	int	i;
	int	ret;

	i = 0;
	while (i < number_of_philos)
	{
		ret = pthread_create(&philos[i].thread, NULL,
				ft_philo_routine, &philos[i]);
		if (ret != 0)
		{
			while (--i >= 0)
				pthread_join(philos[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_parse_and_init(int argc, char **argv, t_parameters *parameters,
					t_philo **philos)
{
	int		return_parse;

	if (argc != 5 && argc != 6)
		return (write(2, "Arguments Error\n", 16), 1);
	return_parse = ft_parse_arguments(argc, argv, parameters);
	if (return_parse == 0)
		return (write(2, "Arguments Error\n", 16), 1);
	if (return_parse == -1)
		return (write(2, "Error: max philosophers is 200\n", 31), 1);
	if (return_parse == -2)
		return (write(2, "Error: time values must be at least 60\n", 39), 1);
	if (ft_init_table(&parameters->table, parameters) != 0)
		return (write(2, "Error: initializing table\n", 26), 1);
	if (ft_init_value_philos(philos, &parameters->table, parameters) != 0)
	{
		ft_cleanup_table(&parameters->table, parameters->number_of_philos);
		return (write(2, "Error: initializing philosophers\n", 33), 1);
	}
	if (ft_start_philos_threads(*philos, parameters->number_of_philos) != 0)
	{
		ft_cleanup_philosophers(*philos, parameters->number_of_philos);
		ft_cleanup_table(&parameters->table, parameters->number_of_philos);
		return (write(2, "Error: starting threads\n", 24), 1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	t_parameters	parameters;
	pthread_t		thread_detect_dep;
	int				i;

	i = 0;
	if (ft_parse_and_init(argc, argv, &parameters, &philos) != 0)
		return (1);
	if (pthread_create(&thread_detect_dep, NULL, ft_check_dep, philos) != 0)
	{
		write(2, "Error: creating monitor thread\n", 31);
		while (i < parameters.number_of_philos)
			(pthread_join(philos[i].thread, NULL), i++);
		ft_cleanup_philosophers(philos, parameters.number_of_philos);
		ft_cleanup_table(&parameters.table, parameters.number_of_philos);
		return (1);
	}
	pthread_join(thread_detect_dep, NULL);
	while (i < parameters.number_of_philos)
		(pthread_join(philos[i].thread, NULL), i++);
	ft_cleanup_philosophers(philos, parameters.number_of_philos);
	ft_cleanup_table(&parameters.table, parameters.number_of_philos);
	return (0);
}
