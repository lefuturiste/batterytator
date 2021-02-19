.SILENT:
.PHONY: install

base:
	gcc -o ./batterytator ./batterytator.c

install:
	cp ./batterytator /usr/bin/batterytator
	cp ./batterytator.service /usr/lib/systemd/system/
	systemctl enable batterytator
