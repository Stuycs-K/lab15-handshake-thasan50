compile runme: basic_client.o forking_server.o pipe_networking.o
	@gcc -o b_client basic_client.o pipe_networking.o -lm
	@gcc -o f_server forking_server.o pipe_networking.o -lm
basic_client.o: basic_client.c
	@gcc -c basic_client.c -lm
forking_server.o: forking_server.c
	@gcc -c forking_server.c -lm
pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c -lm
client: b_client
	@./b_client
server: f_server
	@./f_server
clean:
	@rm *.o
	@rm b_client
	@rm f_server
