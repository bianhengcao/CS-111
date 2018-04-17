#NAME: Bianheng Cao
#EMAIL: bianhengcao@gmail.com

default:
	@gcc -w -g -Wall -Wextra lab0.c -o lab0 -o lab0
check:
	@echo "" | ./lab0;\
	if [ $$? -ne 0 ];\
	then\
		echo "Wrong return value for standard input!";\
		exit 1;\
	fi
	@echo "" > stdin.txt | ./lab0 --input stdin.txt;\
        if [ $$? -ne 0 ];\
        then\
                echo "Wrong return value for valid --input argument!";\
		rm stdin.txt;\
                exit 1;\
        fi
	@rm stdin.txt
	@./lab0 --bullshit;\
        if [ $$? -ne 1 ];\
        then\
                echo "Wrong return value for invalid option!";\
                exit 1;\
        fi
	@./lab0 --input stdin.txt;\
        if [ $$? -ne 2 ];\
        then\
                echo "Wrong return value for invalid --input argument!";\
                exit 1;\
        fi
	@echo "" | ./lab0 --output stdout.txt;\
        if [ $$? -ne 0 ];\
        then\
                echo "Wrong return value for valid --output argument!";\
		rm stdout.txt;\
                exit 1;\
        fi
	@./lab0 --output stdout.txt;\
	if [ $$? -ne 3 ];\
	then\
		echo "Wrong return value for invalid --output argument!";\
		exit 1;\
	fi
	@rm stdout.txt
	@./lab0 --catch --segfault;\
	if [ $$? -ne 4 ];\
	then\
		echo "Wrong return value for --catch and --segfault!";\
		exit 1;\
	fi
	@echo "All tests passed"
clean:
	@rm -f lab0
	@rm -f lab0.tar.gz
dist:
	@tar -czf lab0.tar.gz lab0.c Makefile backtrace.png breakpoint.png README
