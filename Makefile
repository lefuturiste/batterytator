.SILENT:
.PHONY: install

base:
	@echo "Compiling batterytator..."
	gcc -o ./batterytator ./batterytator.c
	whoami > ./tmp_install_user.txt

install:
	sh ./install.sh

uninstall:
	(killall /usr/bin/batterytator) || true
	rm -f /usr/bin/batterytator
	rm -f /usr/lib/systemd/system/batterytator.service
	@echo "Uninstallation completed."