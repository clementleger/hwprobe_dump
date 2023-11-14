CC=$(CROSS_COMPILE)gcc

hwprobe: hwprobe.c hwprobe.h
	$(CC) $^ -O0 -g -o $@
