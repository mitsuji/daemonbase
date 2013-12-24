@echo off

if "%1" == "c" (

DEL main_console.obj
DEL main_win32.obj

DEL daemon_1.obj console1.exe service1.exe
DEL daemon_2.obj console2.exe service2.exe

DEL daemon_module_1.obj libmodule1.dll libmodule1.exp libmodule1.lib
DEL daemon_module_2.obj libmodule2.dll libmodule2.exp libmodule2.lib

) else (

CL /nologo /EHsc /c /TC main_console.c
CL /nologo /EHsc /c /TC main_win32.c

CL /nologo /EHsc /c /TC daemon_1.c
LINK /NOLOGO /OUT:console1.exe daemon_1 main_console
LINK /NOLOGO /OUT:service1.exe daemon_1 main_win32 advapi32.lib

CL /nologo /EHsc /c /TP daemon_2.cpp
LINK /NOLOGO /OUT:console2.exe daemon_2 main_console
LINK /NOLOGO /OUT:service2.exe daemon_2 main_win32 advapi32.lib

CL /nologo /EHsc /c /TC daemon_module_1.c
LINK /NOLOGO /DLL /DEF:daemon_module.def daemon_module_1 /OUT:libmodule1.dll

CL /nologo /EHsc /c /TP daemon_module_2.cpp
LINK /NOLOGO /DLL /DEF:daemon_module.def daemon_module_2 /OUT:libmodule2.dll

)