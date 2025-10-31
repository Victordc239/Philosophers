/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:11:11 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/10/31 13:28:56 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdint.h>
# include <limits.h>

typedef struct s_check_vars
{
	long long	now;
	long long	last_eat;
	int			trying;
	int			i;
	int			full_philos;
}	t_check_vars;

typedef struct s_table
{
	pthread_mutex_t	*id_fork;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dep_mutex;
	int				someone_died;
	long long		start_time_ms;
}				t_table;

typedef struct s_parameters
{
	int		number_of_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		count_must_eat;
	t_table	table;
}				t_parameters;

typedef struct s_philo
{
	int				id_philo;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_parameters	*parameters;
	t_table			*table;
	long long		last_eat_ms;
	int				eat_count;
	int				trying_to_eat;
	pthread_mutex_t	last_eat_mutex;
}				t_philo;

long long	ft_time_now_ms(void);
int			ft_atoi(const char *str);
int			ft_start_philos_threads(t_philo *philos, int n);
int			ft_is_positive_digits(const char *s);
int			ft_philo_loop(t_philo *philo);
int			ft_parse_arguments(int argc, char **argv, t_parameters *parameters);
int			ft_init_table(t_table *table, t_parameters *params);
int			ft_init_value_philos(t_philo **philos, t_table *table,
				t_parameters *params);
int			ft_parse_and_init(int argc, char **argv, t_parameters *parameters,
				t_philo **philos);
int			ft_check_philos(t_table *table, t_philo *philos,
				t_parameters *params, t_check_vars *vars);
int			check_and_handle_dep(t_table *table, t_philo *philos,
				t_parameters *params, t_check_vars *vars);
void		ft_destroy_table_forks(t_table *table, int forks_initialized);
void		ft_cleanup_table(t_table *table, int n);
void		ft_cleanup_philosophers(t_philo *philos, int n);
void		ft_print_action(t_table *table, int id_philo, const char *action);
void		*ft_philo_routine(void *philosophers);
void		*ft_check_dep(void *arg);
void		ft_swap(pthread_mutex_t **a, pthread_mutex_t **b);
void		ft_actions(t_philo *philo, pthread_mutex_t *first_fork,
				pthread_mutex_t *second_fork);

#endif
