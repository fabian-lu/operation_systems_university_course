all:
	gcc uebung2_10threads.c -g -o uebung2_10threads -Wall -pthread
	gcc uebung2_gauss.c -g -o uebung2_gauss -Wall -pthread
	gcc uebung2_scheduler.c -g -o uebung2_scheduler -Wall

10threads: all
	./uebung2_10threads

gauss: all
	./uebung2_gauss

scheduler: all
	./uebung2_scheduler processes.list FCFS > fcfs.out
	./uebung2_scheduler processes.list SJF > sjf.out
	./uebung2_scheduler processes.list SRTF > srtf.out
	./uebung2_scheduler processes.list RR > rr.out
	./uebung2_scheduler processes.list RRPRIO > rrprio.out