
Debian
====================
This directory contains files used to package epgcd/epgc-qt
for Debian-based Linux systems. If you compile epgcd/epgc-qt yourself, there are some useful files here.

## epgc: URI support ##


epgc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install epgc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your epgc-qt binary to `/usr/bin`
and the `../../share/pixmaps/epgc128.png` to `/usr/share/pixmaps`

epgc-qt.protocol (KDE)

