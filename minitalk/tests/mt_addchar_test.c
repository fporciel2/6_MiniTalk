/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_addchar_test.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 09:10:32 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/27 10:20:47 by fporciel         ###   ########.fr       */
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
		return (NULL);
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

int	main(void)
{
	static char	*message;
	char		character;
	int			pid;

	pid = 0;
	printf("\n%s\n", message);
	character = 48;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 49;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 50;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 51;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 52;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 53;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 54;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 55;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 56;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 57;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 58;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	character = 59;
	message = mt_addchar(message, character, pid);
	printf("Added %c: \n%s\n", character, message);
	free(message);
	return (0);
}
