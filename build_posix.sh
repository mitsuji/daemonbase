#!/bin/sh

if [ "$1" = "c" ]
then

rm main_console.o
rm main_posix.o

rm daemon_1.o console1 daemon1
rm daemon_2.o console2 daemon2

rm daemon_module_1.o libmodule1.so
rm daemon_module_2.o libmodule2.so

else

gcc -fPIC -c -xc main_console.c
gcc -fPIC -c -xc main_posix.c

gcc -fPIC -c -xc daemon_1.c
gcc -fPIC -o console1 daemon_1.o main_console.o -lpthread -ldl
gcc -fPIC -o daemon1 daemon_1.o main_posix.o -lpthread -ldl

gcc -fPIC -c -xc++ daemon_2.cpp
gcc -fPIC -o console2 daemon_2.o main_console.o -lpthread -ldl -lstdc++
gcc -fPIC -o daemon2 daemon_2.o main_posix.o -lpthread -ldl -lstdc++

gcc -fPIC -c -xc daemon_module_1.c
gcc -fPIC -shared daemon_module_1.o -o libmodule1.so

gcc -fPIC -c -xc++ daemon_module_2.cpp
gcc -fPIC -shared daemon_module_2.o -o libmodule2.so -lstdc++

fi
