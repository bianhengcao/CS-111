#NAME: Bianheng Cao
#EMAIL: bianhengcao@gmail.com
default:
	@gcc -w -g -Wall -Wextra -lm -lpthread -lmraa lab4b.c -o lab4b
check: default
	@echo "SCALE=F\nPERIOD=3\nSCALE=C\nSTOP\nSTART\nOFF" > test.txt
	@./lab4b --period=10 --log=log --scale=C <test.txt
clean:
	@rm -f lab4b lab4b.tar.gz
dist:
	@tar -czf lab4b.tar.gz lab4b.c Makefile README