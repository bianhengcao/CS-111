#NAME: Bianheng Cao
#EMAIL: bianhengcao@gmail.com

default: 
	@gcc -w -g -Wall -Wextra -lz lab1b-client.c -o lab1b-client
	@gcc -w -g -Wall -Wextra -lz lab1b-server.c -o lab1b-server
client:
	@gcc -w -g -Wall -Wextra -lz lab1b-client.c -o lab1b-client
server:
	@gcc -w -g -Wall -Wextra -lz lab1b-server.c -o lab1b-server
clean:
	@rm -f lab1b-client
	@rm -f lab1b-server
	@rm -f lab1a.tar.gz
dist:
	@tar -czf lab1b.tar.gz lab1b-client.c lab1b-server.c Makefile README
