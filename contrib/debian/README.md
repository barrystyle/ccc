
Debian
====================
This directory contains files used to package cccd/ccc-qt
for Debian-based Linux systems. If you compile cccd/ccc-qt yourself, there are some useful files here.

## ccc: URI support ##


ccc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install ccc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your ccc-qt binary to `/usr/bin`
and the `../../share/pixmaps/ccc128.png` to `/usr/share/pixmaps`

ccc-qt.protocol (KDE)

