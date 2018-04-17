#NAME: Bianheng Cao
#EMAIL: bianhengcao@gmail.com
default:
	@gcc -w -g -Wall -Wextra lab4c_tcp.c -o lab4c_tcp
	@gcc -w -g -Wall -Wextra -lssl -lcrypto lab4c_tls.c -o lab4c_tls
clean:
	@rm -f lab4c_tcp lab4c_tls lab4c.tar.gz
dist:
	@tar -czf lab4c.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README
