CC=$(CROSS_COMPILE)gcc

hwprobe: hwprobe.c
	$(CC) $^ -O0 -g -o $@
