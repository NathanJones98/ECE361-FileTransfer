
# Makefile You should also prepare a makefile that generates the executable file deliver from
# deliver.c and the executable file server from server.c.

default: server deliver
# compile the client program
deliver: deliver.c
	gcc deliver.c -o deliver
# compile server program
server: server.c
	gcc server.c -o server
clean:
	rm deliver server