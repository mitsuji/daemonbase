[daemonbase]

a template of the cross-platform daemon implementation.



[Windows Console]
config: daemon.conf.console

>console_win32.cmd



[Posix Console]
config: daemon.conf.console

>./console_posix.sh



[Windows Service]
config: daemon.conf.win32

>SC create daemonbase1 binPath= "C:\Users\mitsuji\Documents\dev\daemonbase\service1.exe ""C:\Users\mitsuji\Documents\dev\daemonbase\daemon.conf.win32"""
>SC delete daemonbase1

>net start daemonbase1
>net stop daemonbase1


>SC create daemonbase2 binPath= "C:\Users\mitsuji\Documents\dev\daemonbase\service2.exe ""C:\Users\mitsuji\Documents\dev\daemonbase\daemon.conf.win32"""
>SC delete daemonbase2

>net start daemonbase2
>net stop daemonbase2



[Posix Daemon Linux]
config: daemon.conf.posix.linux

>./daemon_ctrl.linux.sh start
>./daemon_ctrl.linux.sh restart
>./daemon_ctrl.linux.sh start



[Posix Daemon OSX]
config: daemon.conf.posix.osx

>./daemon_ctrl.osx.sh start
>./daemon_ctrl.osx.sh restart
>./daemon_ctrl.osx.sh start

