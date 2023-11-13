@echo off

cls

@REM AES ECB Windows

cd "./bin/"

cl.exe /c /EHsc /std:c++20 "../src/Common/Helper.cpp" "../src/CPU/AES-CPU.cpp" "../src/Common/SHA256.cpp"  "../test/Main.cpp"

link.exe Main.obj Helper.obj SHA256.obj AES-CPU.obj

@copy Main.exe "../" > nul

cd ../

Main.exe data/Novel.txt Novel.txt.enc
