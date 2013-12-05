@echo off

CL /EHsc /c /TC daemon.c
CL /EHsc /c /TC main_console.c
CL /EHsc /c /TC main_win32.c
LINK /OUT:console.exe daemon main_console
LINK /OUT:service.exe daemon main_win32 advapi32.lib
