LIBFT_DIR = libft

SRCS = main.c parse_file.c symbols.c

SRC_DIR = srcs/
OBJS = $(SRCS:.c=.o)
OBJ_DIR = objs/

CC = gcc -Ilibft
CFLAGS = -Wextra -Wall -Werror -Ilibft

NAME = ft_nm
INCLUDES = -Ilibft -Iincludes

LIBS = -L$(LIBFT_DIR) -lft

$(OBJ_DIR)%o: $(SRC_DIR)%c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -fPIC $< -o $@

all: $(NAME)

$(NAME): $(addprefix $(OBJ_DIR), $(OBJS))
	$(MAKE) -C ./libft
	$(CC) $(addprefix $(OBJ_DIR), $(OBJS)) $(INCLUDES) $(LIBS) -o $(NAME)

clean:
	$(MAKE) clean -C ./libft
	rm -rf $(addprefix $(OBJ_DIR), $(OBJS))

fclean: clean
	$(MAKE) fclean -C ./libft
	rm -rf $(NAME)

re: fclean all
