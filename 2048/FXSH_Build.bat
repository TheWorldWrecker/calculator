@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist 2048.G1A  del 2048.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\Users\hojoo\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\Users\hojoo\Tools\MakeAddinHeader363.exe" "C:\Users\hojoo\OneDrive\¹®¼­\CASIO\fx-9860G SDK\2048"
if not exist 2048.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

