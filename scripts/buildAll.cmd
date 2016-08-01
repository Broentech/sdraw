call setEnv.cmd
call installHeaders.cmd
cd %MVDIR%
qmake -r Broen_Draw.pro
qmake -r -tp vc Broen_Draw.pro
nmake install
cd %BDIR%
call vcstart.cmd
