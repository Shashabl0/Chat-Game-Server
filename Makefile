server:
	wsl gcc server.c -o server -lpthread

client:
	wsl gcc client.c -o client -lpthread

clean:
	rm -f server client

update: clean server client