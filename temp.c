#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_HW_MONS 32
#define MAX_SENSORS 32

#define MAX_PATH_LEN 256
#define MAX_FILEBUF_LEN 256

#define TEMP_SCALE_COEF 1000

int init_temp(int* fds, int fds_len) {
    char buf[MAX_PATH_LEN] = {0};
    char buf_file[MAX_FILEBUF_LEN] = {0};

    int i,j;
    int fd;
    int ret;
    struct stat sb;

    int fd_ptr = 0;

    if (fds_len == 0) {
        return 0;
    }

    for (i = 0; i < MAX_HW_MONS; i += 1) {
        snprintf(buf, MAX_PATH_LEN, "/sys/class/hwmon/hwmon%d", i);

        if (stat(buf, &sb) != 0 || !S_ISDIR(sb.st_mode)) {
            continue;
        }

        for (j = 0; j < MAX_SENSORS; j += 1) {
            snprintf(buf, MAX_PATH_LEN, "/sys/class/hwmon/hwmon%d/temp%d_label", i, j);
            fd = open(buf, O_RDONLY);
            if (fd == -1) {
                continue;
            }

            ret = read(fd, buf_file, MAX_FILEBUF_LEN);
            if (ret == -1) {
                continue;
            }


            snprintf(buf, MAX_PATH_LEN, "/sys/class/hwmon/hwmon%d/temp%d_input", i, j);
            fd = open(buf, O_RDONLY);
            if (fd == -1) {
                continue;
            }

            if (fd_ptr < fds_len) {
                fds[fd_ptr] = fd;
                fd_ptr += 1;
            } else {
                return fd_ptr;
            }
        }
    }
    return fd_ptr;
}

static int get_temp_by_fd(int fd) {
    char buf_file[MAX_FILEBUF_LEN];
    int ret;
    if (pread(fd, buf_file, MAX_FILEBUF_LEN, 0) == -1) {
        return -1;
    }
    return atoi(buf_file) / TEMP_SCALE_COEF;
}

int get_temp(int* fds, int fds_len) {
    int i;
    int temp_cnt = 0;
    int avg = 0;
    for (i = 0; i < fds_len; i+= 1) {
        int temp = get_temp_by_fd(fds[i]);
        if(temp == -1) {
            continue;
        }
        avg += temp;
        temp_cnt += 1;
    }
    return avg / temp_cnt;
}
