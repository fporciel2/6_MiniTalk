/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server_main_test.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 14:13:18 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/27 15:47:56 by fporciel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* This function calculates the length of a given null-terminated string. */
static size_t	fp_strlen(const char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

/* A function that prints a single character to a file provided by a given 
 * file descriptor. It is a tool for the fp_putnbr_fd function below. */
static void	fp_writenbr(char c, int fd)
{
	write(fd, &c, 1);
}

/* This function prints the ascii representation of an integer number to a file
 * provided by a given file descriptor. */
static void	fp_putnbr_fd(int n, int fd)
{
	long int	i;

	i = n;
	if (i < 0)
	{
		i = -i;
		write(fd, "-", 1);
	}
	if (i > 9)
	{
		fp_putnbr_fd((i / 10), fd);
		fp_writenbr(((i % 10) + 48), fd);
	}
	else
		fp_writenbr((i + 48), fd);
}

/* This function prints a string to a file provided by a given file descriptor.
 * */
static void	fp_putstr_fd(char *s, int fd)
{
	size_t	strlen;

	if (s == NULL)
		return ;
	strlen = fp_strlen(s);
	write(fd, s, strlen);
}

/* This function handles some errors in the main file.*/
static int	test_exit(void)
{
	exit(EXIT_FAILURE);
	return (0);
}

/*This function is meant to verify if the sig_data structure and the sigaction
 * function in the main file are, respectively, well setted up and well
 * registering data. It also verifies the signal handling and sending. */
static void	test_signals(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGUSR1)
	{
		write(1, "\nThe signal is: ", 1);
		fp_putnbr_fd(signum, 1);
		write(1, "\nThe process ID is: ", 1);
		fp_putnbr_fd(info->si_pid, 1);
		write(1, "\n", 1);
		kill(info->si_pid, signum);
		return ;
	}
	if (signum == SIGUSR2)
	{
		write(1, "\nThe signal is: ", 1);
		fp_putnbr_fd(signum, 1);
		write(1, "\nThe process ID is: ", 1);
		fp_putnbr_fd(info->si_pid, 1);
		write(1, "\n", 1);
		kill(info->si_pid, signum);
		return ;
	}
}

/* This is the main function to test. It simply set up the handling signals'
 * features and then stops, waiting for a signal. It needs a client program that
 * sends signals SIGUSR1 and SIGUSR2 in order to be properly tested. */
int	main(void)
{
	struct sigaction	sig_data;
	int					pid;

	pid = getpid();
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (test_exit());
	sig_data.sa_sigaction = test_signals;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
			|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (test_exit());
	fp_putstr_fd("\nSERVER PID: ", 1);
	fp_putnbr_fd(pid, 1);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
