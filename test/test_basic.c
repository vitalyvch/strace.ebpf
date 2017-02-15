/*
 * Copyright 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * test_basic.c -- unit tests for strace.eBPF
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define PATTERN_START	0x12345678
#define PATTERN_END	0x87654321
#define BUF_LEN		0x100

#define MARK_START()	close(PATTERN_START)
#define MARK_END()	close(PATTERN_END)

#define FILE_EXIST	"/etc/fstab"
#define FILE_CREATE	"/tmp/tmp-strace.ebpf"

static void
run_basic_tests(void)
{
	char buffer[BUF_LEN];
	struct utsname name;
	struct stat buf;
	int fd;

	/* PART #1 - real arguments */

	fd = open(FILE_EXIST, O_RDONLY);
	close(fd);

	fd = open(FILE_CREATE, O_RDWR | O_CREAT);
	write(fd, buffer, BUF_LEN);
	lseek(fd, 0, SEEK_SET);
	read(fd, buffer, BUF_LEN);
	fstat(fd, &buf);
	close(fd);
	unlink(FILE_CREATE);

	execve(FILE_CREATE, (char * const*)0x123456, (char * const*)0x654321);

	stat(FILE_EXIST, &buf);
	lstat(FILE_EXIST, &buf);

	uname(&name);

	syscall(SYS_getpid); /* getpid */
	syscall(SYS_gettid); /* gettid */

	/* PART #2 - test arguments */

	write(0x1001, buffer, 1);
	read (0x1002, buffer, 2);
	lseek(0x1003, 3, SEEK_END);
	fstat(0x1004, &buf);
	syscall(SYS_futex, 1, 2, 3, 4, 5, 6); /* futex */
	ioctl(7, 8, 9);
}

static void test_0(void)
{
	MARK_START();
	run_basic_tests();
	MARK_END();
}

static void test_1(void)
{
	syscall(SYS_fork);
	test_0();
}

static void (*run_test[])(void) = {
	test_0,
	test_1
};

int
main(int argc, char *argv[])
{
	int max = sizeof(run_test) / sizeof(run_test[0]) - 1;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <test-number: 0..%i>\n", argv[0], max);
		return -1;
	}

	int n = atoi(argv[1]);
	if (n > max) {
		fprintf(stderr, "Error: test number can take only following values:"
			" 0..%i (%i is not allowed)\n", max, n);
		return -1;
	}

	printf("Starting: test_%i ...\n", n);
	run_test[n]();
}
