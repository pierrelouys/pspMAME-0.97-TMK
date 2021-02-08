
rem この mingw は mame公式の奴と同じです。今回 'm68kmake.exe' は(DJGPP は止めて) mame_mingw でコンパイルしてみました。
rem コンパイルは 「my_ming.bat↓」（このファイル）「gcc -O1 m68kmake.c -o m68kmake↓」 と打ちました。

rem Ｗｉｎ９８の場合winbootdirの標準は、C:\WINDOWSです。(OSを入れたフォルダ名)

rem PATH=%winbootdir%;%winbootdir%\COMMAND;C:\DJGPP\BIN
rem DJGPP=c:\DJGPP\DJGPP.ENV

rem PATH=%winbootdir%;%winbootdir%\COMMAND;C:\MAMEDEV\mingw\BIN;C:\MAMEDEV\mingw\mingw32\bin;C:\DJGPP\BIN
set PATH=%winbootdir%;%winbootdir%\COMMAND;C:\mingw\BIN
set DJGPP=c:\DJGPP\DJGPP.ENV

