#NAME: Bianheng Cao
#EMAIL: bianhengcao@gmail.com

default: 
	@gcc -w -g -Wall -Wextra -lpthread lab2_add.c -o lab2_add
	@gcc -w -g -Wall -Wextra -lpthread lab2_list.c SortedList.c -o lab2_list
tests:
	@./lab2_add --threads 1 --iterations 100 >> lab2_add.csv
	@./lab2_add --threads 1 --iterations 1000 >> lab2_add.csv
	@./lab2_add --threads 1 --iterations 10000 >> lab2_add.csv
	@./lab2_add --threads 1 --iterations 100000 >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100 >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 1000 >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10000 >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100000 >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100 >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 1000 >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100000 >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100 >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 1000 >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100000 >> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100 >> lab2_add.csv
	@./lab2_add --threads 12 --iterations 1000 >> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10000 >> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100000 >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 20 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 40 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 80 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 1000 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10000 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100000 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 20 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 40 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 80 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 1000 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100000 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 20 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 40 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 80 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 1000 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100000 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 20 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 40 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 80 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 1000 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10000 --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100000 --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 20 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 40 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 80 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 1000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 20 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 40 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 80 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 1000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 20 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 40 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 80 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 1000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 20 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 40 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 80 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 1000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10000 --sync=m --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 20 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 40 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 80 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 1000 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 20 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 40 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 80 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 1000 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 20 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 40 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 80 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 1000 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 20 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 40 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 80 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 1000 --sync=s --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 20 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 40 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 80 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 100 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 2 --iterations 1000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 20 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 40 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 80 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 100 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 1000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 20 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 40 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 80 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 100 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 1000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 20 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 40 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 80 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 100 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 1000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10000 --sync=c --yield>> lab2_add.csv
	@./lab2_add --threads 1 --iterations 10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10000 --sync=m >> lab2_add.csv
	@./lab2_add --threads 1 --iterations 10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads 12 --iterations 10000 --sync=s >> lab2_add.csv
	@./lab2_add --threads 1 --iterations 10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads 2 --iterations 10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads 4 --iterations 10000 --sync=c >> lab2_add.csv
	@./lab2_add --threads 8 --iterations 10000 --sync=c >> lab2_add.csv
	@-./lab2_add --threads 12 --iterations 10000 --sync=c >> lab2_add.csv
	@-./lab2_list --threads 1 --iterations 10 >> lab2_list.csv
	@-./lab2_list --threads 1 --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 1 --iterations 1000 >> lab2_list.csv
	@-./lab2_list --threads 1 --iterations 10000 >> lab2_list.csv
	@-./lab2_list --threads 1 --iterations 20000 >> lab2_list.csv
	@-./lab2_list --threads 2 --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 2 --iterations 10 >> lab2_list.csv
	@-./lab2_list --threads 2 --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 2 --iterations 1000 >> lab2_list.csv
	@-./lab2_list --threads 4 --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 4 --iterations 10 >> lab2_list.csv
	@-./lab2_list --threads 4 --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 4 --iterations 1000 >> lab2_list.csv
	@-./lab2_list --threads 8 --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 8 --iterations 10 >> lab2_list.csv
	@-./lab2_list --threads 8 --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 8 --iterations 1000 >> lab2_list.csv
	@-./lab2_list --threads 12 --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 12 --iterations 10 >> lab2_list.csv
	@-./lab2_list --threads 12 --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 12 --iterations 1000 >> lab2_list.csv
	@-./lab2_list --threads 1 --yield=i --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 1 --yield=il --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 1 --yield=idl --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 1 --yield=il --iterations 100 >> lab2_list.csv
	@-./lab2_list --threads 2 --yield=dl --iterations 8 >> lab2_list.csv
	@-./lab2_list --threads 2 --yield=il --iterations 8 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=l --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=l --iterations 8 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=l --iterations 32 >> lab2_list.csv
	@-./lab2_list --threads 12 --yield=l --iterations 32 >> lab2_list.csv
	@-./lab2_list --threads 12 --yield=l --iterations 4 >> lab2_list.csv
	@-./lab2_list --threads 2 --yield=il --iterations 4 >> lab2_list.csv
	@-./lab2_list --threads 4 --yield=il --iterations 4 >> lab2_list.csv
	@-./lab2_list --threads 4 --yield=dl --iterations 32 >> lab2_list.csv
	@-./lab2_list --threads 2 --yield=i --iterations 16 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=il --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=il --iterations 16 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=dl --iterations 16 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=d --iterations 32 >> lab2_list.csv
	@-./lab2_list --threads 4 --yield=dl --iterations 16 >> lab2_list.csv
	@-./lab2_list --threads 2 --yield=i --iterations 16 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=i --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 4 --yield=i --iterations 32 >> lab2_list.csv
	@-./lab2_list --threads 12 --yield=i --iterations 1 >> lab2_list.csv
	@-./lab2_list --threads 4 --yield=i --iterations 16 >> lab2_list.csv
	@-./lab2_list --threads 8 --yield=i --iterations 8 >> lab2_list.csv
	@-./lab2_list --threads 12 --yield=il --iterations 32 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 12 --yield=d --iterations 32 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 2 --yield=dl --iterations 32 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 12 --yield=dl --iterations 16 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 4 --yield=d --iterations 2 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 2 --yield=dl --iterations 16 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 4 --yield=i --iterations 32 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 4 --yield=il --iterations 32 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 8 --yield=i --iterations 32 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 8 --yield=il --iterations 16 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 4 --yield=i --iterations 32 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 8 --yield=il --iterations 1 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 8 --yield=il --iterations 1 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 2 --yield=dl --iterations 32 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 12 --yield=dl --iterations 16 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 8 --yield=d --iterations 2 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 2 --yield=dl --iterations 16 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 2 --yield=d --iterations 32 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 4 --yield=il --iterations 32 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 16 --yield=dl --iterations 32 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 8 --yield=il --iterations 16 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 2 --yield=i --iterations 32 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 16 --yield=dl --iterations 1 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 1 --iterations 1000 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 2 --iterations 1000 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 4 --iterations 1000 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 8 --iterations 1000 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 12 --iterations 1000 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 16 --iterations 1000 --sync=m>> lab2_list.csv
	@-./lab2_list --threads 1 --iterations 1000 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 2 --iterations 1000 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 4 --iterations 1000 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 8 --iterations 1000 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 12 --iterations 1000 --sync=s>> lab2_list.csv
	@-./lab2_list --threads 16 --iterations 1000 --sync=s>> lab2_list.csv
graphs:tests
	@./lab2_add.gp 
	@./lab2_list.gp
clean:
	@rm -f lab2_add lab2_list lab2a.tar.gz
dist:
	@tar --ignore-failed-read -czf lab2a.tar.gz lab2_add.c SortedList.h SortedList.c lab2_add.gp lab2_list.gp lab2_list.c lab2_add.csv lab2_list.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png Makefile README
