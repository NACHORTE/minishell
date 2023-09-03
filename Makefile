# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/03 15:02:39 by iortega-          #+#    #+#              #
#    Updated: 2023/09/03 18:32:36 by orudek           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := gcc
STANDARD_FLAGS := -Wall -Wextra -Werror#-fsanitize=leak
RM := rm -f

SRC_DIR := src
INCLUDE_DIR := include
OBJ_DIR := obj

LIBFT_DIR := libft
LIBFT_NAME := libft.a

LIB := readline

SRC_FILES := main.c

SRC := $(addprefix $(SRC_DIR)/,$(SRC_FILES))
LIB_FLAG := $(addprefix -l,$(LIB))
INCLUDE_PATH := -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME)
	$(CC) $(STANDARD_FLAGS)  $(INCLUDE_PATH) $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME) -lreadline -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(STANDARD_FLAGS) $(INCLUDE_PATH) -c -o $@ $<

$(LIBFT_DIR)/$(LIBFT_NAME):
	@make -C $(LIBFT_DIR) all

clean:
	@$(RM) -r $(OBJ_DIR)
	@make -C $(LIBFT_DIR) fclean
	@echo "Deleted obj files:\n\t$(OBJ)"
	@echo "Deleted $(LIBFT_NAME)"

fclean: clean
	@$(RM) $(NAME)
	
re: fclean all

.PHONY: clean fclean all re