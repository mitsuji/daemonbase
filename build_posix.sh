#!/bin/sh

gcc -c daemon.c
gcc -c main_console.c
gcc -c main_posix.c
gcc -o console daemon.o main_console.o -lpthread
gcc -o daemon daemon.o main_posix.o -lpthread
