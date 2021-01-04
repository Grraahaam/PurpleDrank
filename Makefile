
# Build game binaries
all:
	@make -C src

# Adding binary file and desktop related icons/launcher
install:
	cp -f PurpleDrank /usr/games/purpledrank
	
	mkdir -p /usr/share/PurpleDrank
	mkdir -p ~/.local/share/applications
	
	cp -rf res/* /usr/share/PurpleDrank
	
	cp -f conf/purpledrank.desktop /usr/share/applications
	
	update-desktop-database /usr/share/applications

# Removing binary file and desktop related icons/launcher
uninstall:
	rm -rf /usr/games/purpledrank
	
	rm -rf /usr/share/PurpleDrank
	
	rm -rf /usr/share/applications/purpledrank.desktop
	update-desktop-database /usr/share/applications

# Clean up *.o files
clean:
	@make -C src clean

# Clean up *.o files and game binary
purge:
	@make -C src purge
