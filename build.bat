@echo off
mkdir build
FOR /R "./src" %%F IN (*.c) DO (
    cl.exe /wd4133 /I"./src" /c "%%F"
)
lib.exe /OUT:clib.lib *.obj
del *.obj