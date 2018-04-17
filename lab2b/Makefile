#NAME: Bianheng Cao
#EMAIL: bianhengcao@gmail.com
default:
	@gcc -w -g -Wall -Wextra -lpthread -lprofiler lab2_list.c SortedList.c -o lab2_list
tests:clean default
	@./lab2_list --threads 1 --iterations 1000 --sync=s> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 1000 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 24 --iterations 1000 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 1000 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 24 --iterations 1000 --sync=m>> lab2b_list.csv
	@-./lab2_list --threads 1 --iterations 1 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 1 --iterations 2 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 1 --iterations 4 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 1 --iterations 8 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 1 --iterations 16 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 4 --iterations 1 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 4 --iterations 2 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 4 --iterations 4 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 4 --iterations 8 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 4 --iterations 16 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 8 --iterations 1 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 8 --iterations 2 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 8 --iterations 4 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 8 --iterations 8 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 8 --iterations 16 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 12 --iterations 1 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 12 --iterations 2 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 12 --iterations 4 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 12 --iterations 8 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 12 --iterations 16 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 16 --iterations 1 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 16 --iterations 2 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 16 --iterations 4 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 16 --iterations 8 --lists=4 --yield=id>> lab2b_list.csv
	@-./lab2_list --threads 16 --iterations 16 --lists=4 --yield=id>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 10 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 20 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 40 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 80 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 10 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 20 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 40 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 80 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 10 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 20 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 40 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 80 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 10 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 20 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 40 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 80 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 10 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 20 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 40 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 80 --lists=4 --yield=id --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 10 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 20 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 40 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 80 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 10 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 20 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 40 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 80 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 10 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 20 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 40 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 80 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 10 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 20 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 40 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 80 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 10 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 20 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 40 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 16 --iterations 80 --lists=4 --yield=id --sync=m>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --lists=4 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --lists=8 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --lists=16 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --lists=4 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --lists=8 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --lists=16 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --lists=4 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --lists=8 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --lists=16 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --lists=4 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --lists=8 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --lists=16 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --lists=4 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --lists=8 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --lists=16 --sync=s>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --lists=4 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --lists=8 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 1 --iterations 1000 --lists=16 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --lists=4 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --lists=8 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 2 --iterations 1000 --lists=16 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --lists=4 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --lists=8 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 4 --iterations 1000 --lists=16 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --lists=4 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --lists=8 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 8 --iterations 1000 --lists=16 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --lists=4 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --lists=8 --sync=m>> lab2b_list.csv
	@./lab2_list --threads 12 --iterations 1000 --lists=16 --sync=m>> lab2b_list.csv
graphs:tests
	@./lab2_list.gp
clean:
	@rm -f lab2_list lab2b.tar.gz
profile:
	@LD_PRELOAD=/usr/lib64/libprofiler.so
	@CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	@pprof --text ./lab2_list ./raw.gperf > profile.out
	@pprof --list=add ./lab2_list ./raw.gperf >> profile.out
	@rm -f ./raw.gperf
dist:clean default graphs profile
	@tar -czf lab2b.tar.gz SortedList.h SortedList.c lab2_list.c lab2b_list.csv profile.out lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png Makefile lab2_list.gp README
