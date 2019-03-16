#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define FIB_DEV "/dev/fibonacci"

int main()
{
    int fd;
    long long sz;

    char buf[128];
    char write_buf[] = "testing writing";
    int offset = 100;  // TODO: test something bigger than the limit
    int i = 0;

    int fd_log;
    char log[100];
    struct timespec before, after;

    fd = open(FIB_DEV, O_RDWR);
    fd_log = open("client.txt", O_WRONLY | O_CREAT, 0666);

    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (i = 0; i <= offset; i++) {
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
    }

    for (i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_REALTIME, &before);
        sz = read(fd, buf, 128);
        clock_gettime(CLOCK_REALTIME, &after);
        sprintf(log, "%ld\n", after.tv_nsec - before.tv_nsec);
        write(fd_log, log, strlen(log));
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, buf);
    }

    for (i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_REALTIME, &before);
        sz = read(fd, buf, 128);
        clock_gettime(CLOCK_REALTIME, &after);
        sprintf(log, "%ld\n", after.tv_nsec - before.tv_nsec);
        write(fd_log, log, strlen(log));
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.\n",
               i, buf);
    }

    close(fd_log);
    close(fd);
    return 0;
}
