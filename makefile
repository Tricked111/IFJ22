CC=gcc
CFLAGS= -std=c99 -Wall -Wextra 

SCANNER=src/scanner/scanner
PARSER=src/parser/parser
STRING=src/str/ifj_string

OBJFILES = $(SCANNER).o $(PARSER).o $(STRING).o main.o
NAME=IFJ22


main: $(NAME)
	
$(NAME) : $(OBJFILES)	
	$(CC) $(CFLAGS) -o $(NAME) $(OBJFILES) 

clean:
	rm -f $(OBJFILES) $(NAME) *~

