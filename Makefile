# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fporciel <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/22 19:16:39 by fporciel          #+#    #+#              #
#    Updated: 2023/08/25 09:19:11 by fporciel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# 
# This software is made available to anyone who wants to retrace the 
# author's learning path through the projects of school 42.
# Copyright (C) 2023  fporciel
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# You can contact the author at: 
#- fporciel@student.42roma.it

.PHONY: all clean fclean re norm_check leaks_check_server leaks_check_client \
	ft_printf_download clean_ft_printf
.DEFAULT_GOAL := $(NAME)
MT_SERVER := server
MT_CLIENT := client
NAME := $(MT_SERVER) $(MT_CLIENT)
MTDIRECTORY := $(shell pwd)
FTPRINTFDIRECTORY := $(MTDIRECTORY)/2_ft_printf
FTDIRECTORY := $(FTPRINTFDIRECTORY)/libft
HEADER := minitalk.h
INCLUDES := /usr/include $(FTPRINTFDIRECTORY) $(FTDIRECTORY) $(MTDIRECTORY)
LIBFTPRINTF := $(FTPRINTFDIRECTORY)/libftprintf.a
LIBFT := $(FTDIRECTORY)/libft.a
LIBRARIES := $(LIBFTPRINTF) $(LIBFT)
LIBRARIES_DIRECTORIES := $(FTLIBRARY) $(FTPRINTFDIRCETORY)
SOURCE_FILES_SERVER := $(wildcard mt_*_server.c)
SOURCE_FILES_CLIENT := $(wildcard mt_*_client.c)
SOURCE_FILES_BOTH := $(SOURCE_FILES_SERVER) $(SOURCE_FILES_CLIENT)
SOURCE_FILES_SHARED := $(filter-out $(SOURCE_FILES_BOTH), $(wildcard mt_*.c))
CC := gcc
COMPILER_FLAGS := -Wall -Wextra -Werror -g -c
HEADERS_FLAGS := $(addprefix -I, $(INCLUDES))
LIBRARIES_FLAGS := $(addprefix -L, $(LIBRARIES_DIRECTORIES))
LINKER_FLAGS := -lft -lftprintf

$(NAME): $(MT_SERVER) $(MT_CLIENT)

all: $(NAME)

$(MT_SERVER): $(LIBRARIES)
	if [ -e $(MT_SERVER) ]; \
		then echo "mt_server already linked"; \
		else $(CC) $(COMPILER_FLAGS) $(HEADERS_FLAGS) $(SOURCE_FILES_SERVER) \
		$(SOURCE_FILES_SHARED) $(LIBRARIES_FLAGS) \
		$(LINKER_FLAGS) -o $(MT_SERVER); fi

$(MT_CLIENT): $(LIBRARIES)
	if [ -e $(MT_CLIENT) ]; \
		then echo "mt_client already linked"; \
		else $(CC) $(COMPILER_FLAGS) $(HEADERS_FLAGS) $(SOURCE_FILES_CLIENT) \
		$(SOURCE_FILES_SHARED) $(LIBRARIES_FLAGS) \
		$(LINKER_FLAGS) -o $(MT_CLIENT); fi

$(LIBRARIES): $(LIBFT) $(LIBFTPRINTF)

$(LIBFT): ft_printf_download
	if [ -e $(LIBFT) ]; \
		then echo "libft detected"; \
		else cd $(FTDIRECTORY) && make && cd .. && cd ..; fi

$(LIBFTPRINTF): ft_printf_download
	if [ -e $(LIBFTPRINTF) ]; \
		then echo "libftprintf detected"; \
		else cd $(FTPRINTFDIRECTORY) && make && cd ..; fi

ft_printf_download:
	if [ -e $(FTPRINTFDIRECTORY) ]; \
		then echo "Libraries' directories detected"; \
		else git clone git@github.com:fporciel2/2_ft_printf.git; fi

clean:
	rm -f *.o $(FTPRINTFDIRECTORY)/*.o $(FTDIRECTORY)/*.o
	rm -f *.h.gch $(FTPRINTFDIRECTORY)/*.h.gch $(FTDIRECTORY)/*.h.gch

fclean: clean
	rm -f $(NAME) $(LIBFT) $(LIBFTPRINTF)

re: fclean all

clean_ft_printf:
	rm -rfd $(FTPRINTFDIRECTORY)

leaks_check_server:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s \
		-v ./mt_server

leaks_check_client:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s \
		-v ./mt_client

norm_check:
	norminette $(SOURCE_FILES_SERVER) $(SOURCE_FILES_CLIENT) $(HEADER) \
		$(FTDIRECTORY)/*.c $(FTDIRECTORY)/*.h $(FTPRINTFDIRECTORY)/*.c \
		$(FTPRINTFDIRECTORY)*.h

