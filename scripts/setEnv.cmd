cd ..
SET LIB=%cd%\install\lib;%QTDIR%\lib
SET INCLUDE=%cd%\install\include;%QTDIR%\include;%QTDIR%\include\QtCore
SET STARTDIR=%cd%
set MVDIR=%STARTDIR%\build\
SET INSTDIR=%STARTDIR%\install
SET BDIR=%STARTDIR%\scripts
SET BROENDRAWPDIR=%STARTDIR%

set logfile=%STARTDIR%\log.txt

set VCDIR=C:\Program Files (x86)\Microsoft Visual Studio 12.0
call "%VCDIR%\VC\vcvarsall.bat" x64