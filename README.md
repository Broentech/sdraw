# SDRAW

SDRAW - A drawing application for Desktop, written in C++ Qt

by Stian Broen, 2016
(C) Broentech Solutions AS
stian@broentech.no

## Installation

How do I build it?
	
First you need (for Windows) :

1. Qt 5.X
3. (Optional) Visual Studio
4. Git

	
Open a shell according to your setup. I use Qt 5.7 64-bit for Desktop (MSVC 2013)

Go to /scripts
run 
setEnv.cmd

go to /source
run
qmake -r
make

(if you have Visual Studio) :
run
qmake -r -tp vc
cmake

You can open the project in Visual Studio now
go to /scripts
run
vcstart.cmd
browse to your sln file and open it


## Usage

Read here for an article, including video, that explains it all :
https://broentech.no/#!/nativeapps/579f5394bd1dc493c687a676


## License

LGPL v3, see separate file

