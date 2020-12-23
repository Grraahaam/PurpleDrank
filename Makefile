
# Build game binaries
all:
	@make -C src

# Adding binary file and desktop related icons/launcher
install:
	cp PurpleDrank /usr/games/purpledrank
	
	cp -r res/ /usr/share/PurpleDrank
	
	cp conf/purpledrank.desktop ~/.local/share/applications
	
	update-desktop-database ~/.local/share/applications

# Removing binary file and desktop related icons/launcher
uninstall:
	rm -rf /usr/games/purpledrank
	
	rm -rf /usr/share/PurpleDrank
	
	rm -rf ~/.local/share/applications/purpledrank.desktop
	update-desktop-database ~/.local/share/applications

# Clean up *.o files
clean:
	@make -C src clean

# Clean up *.o files and game binary
purge:
	@make -C src purge
