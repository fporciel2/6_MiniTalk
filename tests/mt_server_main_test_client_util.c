/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server_main_test_client_util.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 15:01:19 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/27 15:51:39 by fporciel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* This util is meant to test the main function of MiniTalk's server by sending
 * signals to the program provided by the source file mt_server_main_test.c. Its
 * purpose is to send a SIGUSR1 signal to the testing server. If the signal is
 * successfully sent and received, then the server prints on the standard output
 * the util's PID and the SIGUSR1's value on the standard output. If it
 * successfully sends SIGUSR1 to the util "client" program, then this program
 * sends a SIGUSR2 signal. If the signal is successfully sent and received, then
 * the server prints again the util's process ID and the value of SIGUSR2. If
 * the server correctly sends SIGUSR2 to the client, then the client stops
 * execution. The PID of the server must be inserted manually, i.e. you must
 * previously compile the server's main tester and launch its binary. Then, you
 * must put in the pid's assignment the literal value of the server pid, compile
 * this file, and then launch its binary. */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

static void	fp_writenbr(char c, int fd)
{
	write(fd, &c, 1);
}

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

static void	fp_putstr_fd(char *s, int fd)
{
	size_t	strlen;

	if (s == NULL)
		return ;
	strlen = fp_strlen(s);
	write(fd, s, strlen);
}

static int	test_exit(void)
{
	exit(EXIT_FAILURE);
	return (0);
}

static void	test_signals(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGUSR1)
	{
		printf("\nThe signal is: %d\n", signum);
		printf("\nThe server's PID is: %d\n", info->si_pid);
		kill(info->si_pid, SIGUSR2);
		return ;
	}
	if (signum == SIGUSR2)
	{
		printf("\nThe signal is: %d\n", signum);
		printf("\nThe server's PID is: %d\n", info->si_pid);
		test_exit();
	}
}

int	main(void)
{
	struct sigaction	sig_data;
	int					server_pid;
	int					this_pid;

	server_pid = 18085;
	this_pid = getpid();
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (test_exit());
	sig_data.sa_sigaction = test_signals;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
			|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (test_exit());
	fp_putstr_fd("\nCLIENT PID: ", 1);
	fp_putnbr_fd(this_pid, 1);
	write(1, "\n", 1);
	if (kill(server_pid, SIGUSR1) == -1)
		return (test_exit());
	while (1)
		pause();
	return (0);
}
