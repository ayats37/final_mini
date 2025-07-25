# SRCS = main.c

# CC = cc
# CFLAGS = -I/home/linuxbrew/.linuxbrew/opt/readline/include  # -Wall -Wextra -Werror
# LDFLAGS = -L/home/linuxbrew/.linuxbrew/opt/readline/lib -lreadline

# NAME = minishell
# OBJS = $(SRCS:.c=.o)

# LIBFT_DIR = ./libft
# LIBFT = $(LIBFT_DIR)/libft.a

# all = $(NAME)

# $(NAME): $(OBJS) $(LIBFT)
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

# $(LIBFT):
# 	$(MAKE) -C $(LIBFT_DIR)
	
# %.o: %.C
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS)
# 	$(MAKE) clean -C $(LIBFT_DIR)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all



# SRCS =  main.c handler.c tokenizer.c handle_word.c checker.c cmd_redir.c join.c expand.c builtin.c pipe2.c cd.c check_path.c cmd.c echo.c env_list.c env_list2.c env.c exec.c exit.c export.c free.c free2.c heredoc.c pipe.c pwd.c redir.c unset.c utils.c utils2.c expand_heredoc.c expand2.c split_expand.c
# CC = cc
# CFLAGS = -Wall -Wextra -Werror
# NAME = minishell

# OBJS = $(SRCS:.c=.o)

# LIBFT_DIR = ./libft
# LIBFT = $(LIBFT_DIR)/libft.a

# all: $(NAME)

# $(NAME): $(OBJS) $(LIBFT)
# 	$(CC) $(CFLAGS) -L/Users/taya/goinfre/homebrew/opt/readline/lib -lreadline $(OBJS) $(LIBFT) -o $(NAME)

# $(LIBFT):
# 	$(MAKE) -C $(LIBFT_DIR)

# %.o: %.c 
# 	$(CC) $(CFLAGS) -I/Users/taya/goinfre/homebrew/opt/readline/include -c $< -o $@

# clean:
# 	rm -f $(OBJS) 
# 	$(MAKE) clean -C $(LIBFT_DIR)

# fclean: clean
# 	rm -f $(NAME)
# 	$(MAKE) fclean -C $(LIBFT_DIR)

# re: fclean all

# .PHONY: all bonus clean fclean re



SRCS =  main.c handler.c tokenizer.c handle_word.c checker.c cmd_redir.c join.c expand.c builtin.c pipe2.c cd.c check_path.c cmd.c echo.c env_list.c env_list2.c env.c exec.c exit.c export.c free.c free2.c heredoc.c pipe.c pwd.c redir.c unset.c utils.c utils2.c expand_heredoc.c expand2.c split_expand.c
CC = cc
CFLAGS = -Wall -Wextra -Werror 
NAME = minishell

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME):	$(OBJS)	$(LIBFT)
	$(CC)	$(CFLAGS) -lreadline -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c 
	$(CC) $(CFLAGS) -I/opt/homebrew/opt/readline/include -c $< -o $@

clean:
	rm -f $(OBJS) 
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all bonus clean fclean re
