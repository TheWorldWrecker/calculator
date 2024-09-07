@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist SNAKE.G1A  del SNAKE.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\Users\hojooni\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\Users\hojooni\Tools\MakeAddinHeader363.exe" "C:\Users\hojooni\OneDrive\¹®¼­\CASIO\fx-9860G SDK\SNAKE"
if not exist SNAKE.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

