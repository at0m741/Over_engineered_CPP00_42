NAME = megaphone
CC = c++
C_FLAGS = -Wall -Wextra -Werror -std=c++98
AVX2_FLAG = -Wall -Wextra -Werror -std=c++98 -O3 -mavx2 -march=native -mtune=native

AVX512_FLAG = -Wall -Wextra -Werror -std=c++98 -O3 -mavx512f

C_FILE = megaphone.cpp

all: $(NAME)

$(NAME):
	$(CC) $(C_FLAGS) $(C_FILE) -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

avx2:
	$(CC) $(AVX2_FLAG) $(C_FILE) -o $(NAME)

avx512:
	$(CC) $(AVX512_FLAG) $(C_FILE) -o $(NAME)

re: fclean all

.PHONY: all clean fclean re
