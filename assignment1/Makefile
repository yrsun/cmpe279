TARGET = client server
CC = gcc
normal: $(TARGET)
server: server.c
	$(CC) server.c -o server 
client: client.c
	$(CC) client.c -o client 
clean:
	$(RM) $(TARGET)