/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server_handler_logic.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 16:05:38 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/27 18:23:34 by fporciel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* This file is meant to test the logic of the server's handler function. It
 * simulates the behavior of the interaction between the client and the server,
 * in a modular way. It is based on the already tested function mt_addchar.*/

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

static char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*str;
	size_t	slen;

	i = 0;
	slen = ft_strlen(s);
	str = (char *)malloc(slen + 1);
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

static char	*mt_addchar(char *message, char character, int client_pid)
{
	char	*new_message;
	int		messagelen;
	int		count;

	(void)client_pid;
	messagelen = (int)ft_strlen(message);
	count = 0;
	new_message = malloc((2 + messagelen) * sizeof(*new_message));
	if (new_message == NULL)
		return (free(message), NULL);
	if (message)
	{
		while (count < messagelen)
		{
			new_message[count] = message[count];
			count++;
		}
		free(message);
	}
	new_message[count] = character;
	new_message[count + 1] = 0;
	return (new_message);
}

static void	mt_server_handler(int signum, int *checker)
{
	static char	character = 0;
	static char	*message = NULL;
	static int	bitIndex = 7;

	if (signum == 1)
		character |= (1 << bitIndex);
	bitIndex--;
	if (bitIndex < 0)
	{
		message = mt_addchar(message, character, 0);
		if (message == NULL) // remove this check late
		{
			(*checker)++;
			return ;
		}
		bitIndex = 7;
		if (character == 0)
		{
			(*checker)++;
			printf("\n%s\n", message);
			free(message);
			message = NULL;
		}
		else
			character = 0;
	}
	// Add a sending instruction here... Maybe a flag to pass to
	// 'mt_putmessage'. 
}

int	main(void)
{
	static char	*message = NULL;
	static int	bitIndex = 7;
	static int	checker = 0;
	static int	counter = 0;

	message = ft_strdup("The assassin was murdered along a path of stones and brushwood.");
	if (message == NULL)
		return (0);
	while (checker == 0)
	{
		if (bitIndex < 0)
		{
			counter++;
			bitIndex = 7;
		}
		if ((message[counter] >> bitIndex) & 1)
			mt_server_handler(1, &checker);
		else
			mt_server_handler(0, &checker);
		bitIndex--;
	}
	free(message);
	return (0);
}
