all: lenovo_fan_deamon

lenovo_fan_deamon: ec.c fans.c temp.c lenovo_fan_daemon.c
	gcc ec.c fans.c temp.c lenovo_fan_daemon.c -O2 -o lenovo_fan_daemon

clean:
	rm lenovo_fan_daemon
