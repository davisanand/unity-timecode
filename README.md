Unity Timecode Library: Easy-to-use vinyl timecode access
=========================================================
Copyright (c) 2018 Thomas Perl <m@thp.io>

The Unity Timecode library is a simple dynamic-link-library that
exposes a C API (so in practice can be used without Unity3D) and
a small C# Script that loads and communicates with the library.

The default audio recording device of the system will be used.

This library uses the "timecoder" from xwax, which is licensed
under the GNU GPL and parts of the PortAudio library.

The included Visual Studio project can be used to build the DLL,
you need the xwax and PortAudio sources (see referenced files).

This library is used in the vinylOS project, http://vinylos.io

License
-------

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License version 2 for more details.

You should have received a copy of the GNU General Public License
version 2 along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
