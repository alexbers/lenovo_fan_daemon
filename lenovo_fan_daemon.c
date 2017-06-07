#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "temp.h"
#include "fans.h"

#define HISTORY_LEN 6
#define MAX_SENSORS 10
#define SLEEP_TIME 1

#define FAN_ON_TEMPERATURE 75
#define FAN_OFF_TEMPERATURE 65

volatile int need_exit = 0;

void restore_fans(int signo) {
    need_exit = 1;
}

void install_signal_handler(int signo) {
    if (signal(signo, restore_fans) == SIG_ERR) {
        fprintf(stderr, "Failed to set signal handler\n");
        exit(1);
    }
}

void install_signal_handlers() {
    install_signal_handler(SIGHUP);
    install_signal_handler(SIGINT);
    install_signal_handler(SIGQUIT);
    install_signal_handler(SIGILL);
    install_signal_handler(SIGTRAP);
    install_signal_handler(SIGABRT);
    install_signal_handler(SIGFPE);
    install_signal_handler(SIGBUS);
    install_signal_handler(SIGSEGV);
    install_signal_handler(SIGSYS);
    install_signal_handler(SIGPIPE);
    install_signal_handler(SIGTERM);
    install_signal_handler(SIGUSR1);
    install_signal_handler(SIGUSR2);
    install_signal_handler(SIGTTOU);
    install_signal_handler(SIGPROF);
    install_signal_handler(SIGXCPU);
    install_signal_handler(SIGXFSZ);
}

int main() {
    int temp_fds[MAX_SENSORS];
    int temp_fds_len;
    int temps[HISTORY_LEN] = {};

    int temp_offset = 0;
    int temp_offset_max = 0;

    temp_fds_len = init_temp(temp_fds, MAX_SENSORS);
    if (temp_fds_len == 0) {
        fprintf(stderr, "Failed to find temperature sensors\n");
        return 1;
    }

    if (!init_fans()) {
        fprintf(stderr, "Failed to init fan control. Make sure that you have root rights\n");
        return 1;
    }

    install_signal_handlers();

    printf("Started, found %d sensors\n", temp_fds_len);

    while(!need_exit){
        int temp;
        int avg;
        int i;
        temp = get_temp(temp_fds, temp_fds_len);
        if (temp == -1) {
            fprintf(stderr, "Failed to get temperature\n");
        }
        temps[temp_offset] = temp;

        avg = 0;
        for (i = 0; i <= temp_offset_max; i += 1) {
            avg += temps[i];
        }
        avg /= (temp_offset_max + 1);

        // printf("Last temp: %d, avg: %d\n", temp, avg);

        if(!both_fans_running()) {
            if(avg >= FAN_ON_TEMPERATURE) {
                printf("The temperature is %d, turning on fans\n", avg);
                turn_on_fans();
            }
        } else {
                if (avg <= FAN_OFF_TEMPERATURE) {
                printf("The temperature is %d, turning off fans\n", avg);
                turn_off_fans();
            }
        }
        
        temp_offset = (temp_offset + 1) % HISTORY_LEN;
        if (temp_offset > temp_offset_max) {
            temp_offset_max = temp_offset;
        }

        sleep(SLEEP_TIME);
    }

    turn_on_fans();

    exit(0);
}
