@echo off
rem ~ Copyright: (c) 2014. Turtsevich Alexander
rem ~
rem ~ Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html

rem creates mingw compatible library from FlashRuntimeExtensions.lib

set air_def_name=Adobe AIR.def
set air_lib=%AIR_SDK_HOME%\lib\win\FlashRuntimeExtensions.lib
set air_a=FlashRuntimeExtensions.dll.a

cd windows

if "%1"=="clean" goto clean
echo convert "%air_lib%" to "%air_a%"
reimp -d "%air_lib%"
dlltool -d "%air_def_name%" -l "%air_a%"
goto quit

:clean
echo clean
if exist "%air_a%" echo delete "%air_a%" && del "%air_a%"
if exist "%air_def_name%" echo delete "%air_def_name%" && del "%air_def_name%"
goto quit

:quit
