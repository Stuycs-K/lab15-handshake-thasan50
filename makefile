compile runme: basic_client.o persistant_server.o pipe_networking.o
	@gcc -o b_client basic_client.o pipe_networking.o -lm
	@gcc -o p_server persistant_server.o pipe_networking.o -lm
basic_client.o: basic_client.c
	@gcc -c basic_client.c -lm
persistant_server.o: persistant_server.c
	@gcc -c persistant_server.c -lm
pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c -lm
client: b_client
	@./b_client
server: p_server
	@./p_server
clean:
	@rm *.o
	@rm *~
