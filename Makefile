all: lenovo_fan_daemon

lenovo_fan_daemon: ec.c fans.c temp.c lenovo_fan_daemon.c
	gcc ec.c fans.c temp.c lenovo_fan_daemon.c -O2 -o lenovo_fan_daemon

install: lenovo_fan_daemon
	cp lenovo_fan_daemon /usr/sbin/lenovo_fan_daemon
	cp lenovo_fan_daemon.service /usr/lib/systemd/system/lenovo_fan_daemon.service

clean:
	rm lenovo_fan_daemon
