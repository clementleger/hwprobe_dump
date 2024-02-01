#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/syscall.h>
#include "hwprobe.h"

#define ARRAY_SIZE(__array)	sizeof(__array) / sizeof(__array[0])

#ifndef __NR_riscv_hwprobe
#define __NR_riscv_hwprobe 258
#endif
static int __riscv_hwprobe(struct riscv_hwprobe *pairs, size_t pair_count,
			   size_t cpu_count, unsigned long *cpus,
			   unsigned int flags)
{
	return syscall(__NR_riscv_hwprobe, pairs, pair_count, cpu_count, cpus, flags);
}

static const char* ima_ext0[] = {
	"FD",
	"C",
	"V",
	"Zba",
	"Zbb",
	"Zbs",
	"Zicboz",
	"Zbc",
	"Zbkb",
	"Zbkc",
	"Zbkx",
	"Zknd",
	"Zkne",
	"Zknh",
	"Zksed",
	"Zksh",
	"Zkt",
	"Zvbb",
	"Zvbc",
	"Zvkb",
	"Zvkg",
	"Zvkned",
	"Zvknha",
	"Zvknhb",
	"Zvksed",
	"Zvksh",
	"Zvkt",
	"Zfh",
	"Zfhmin",
	"Zihintntl",
	"Zvfh",
	"Zvfhmin",
	"Zfa",
	"Ztso",
	"Zacas",
	"Zicond",
	"Zihintpause",
	"Zve32x",
	"Zve32f",
	"Zve64x",
	"Zve64f",
	"Zve64d",
	"Zimop",
	"Zca",
	"Zcb",
	"Zcd",
	"Zcf",
	"Zcmop",
	"Zawrs",
};


int check_ima_ext0(void)
{
	unsigned int i;
        struct riscv_hwprobe p;
	uint64_t extensions = 0, ima = 0;

        p.key = RISCV_HWPROBE_KEY_IMA_EXT_0;

        if (__riscv_hwprobe(&p, 1, 0, NULL, 0) < 0) {
            perror("Failed to hwprobe");
            return -1;
        }

        if (p.key < 0) {
                perror("Unknown key!");
                return -1;
        }

	for (i = 0; i < ARRAY_SIZE(ima_ext0); i++) {
		if (p.value & (1ULL << i)) {
			if (ima_ext0[i][0] == 'Z')
				extensions |= (1ULL << i);
			else
				ima |= (1ULL << i);
		}
	}

	printf("Base system ISA:\n");
	for (i = 0; i < ARRAY_SIZE(ima_ext0); i++) {
		if (ima & (1ULL << i))
			printf(" - %s\n", ima_ext0[i]);
	}

	printf("Supported extensions:\n");
	for (i = 0; i < ARRAY_SIZE(ima_ext0); i++) {
		if (extensions & (1ULL << i))
			printf(" - %s\n", ima_ext0[i]);
	}

        return 0;
}

int check_va_bits(void)
{
	int i;
        struct riscv_hwprobe p;

        p.key = RISCV_HWPROBE_KEY_HIGHEST_VIRT_ADDRESS;

        if (__riscv_hwprobe(&p, 1, 0, NULL, 0) < 0) {
            perror("Failed to hwprobe");
            return -1;
        }

	printf("Highest virt address: 0x%llx\n", p.value);

	return 0;
}

int main(int argc, char **argv)
{
	if (check_ima_ext0())
		return 1;

	if (check_va_bits())
		return 1;

	return 0;
}