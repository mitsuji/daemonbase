#!/bin/sh

DAEMON_HOME=/home/administrator/dev/daemonbase

case "$1" in
    "start" )
	echo "starting daemonbase.."
	${DAEMON_HOME}/daemon1 ${DAEMON_HOME}/daemonbase.pid ${DAEMON_HOME}/daemon.conf.posix.linux
	;;
    "stop" )
	echo "stopping daemonbase.."
	kill -s TERM `cat ${DAEMON_HOME}/daemonbase.pid`
	;;
    "restart" )
	echo "restarting daemonbase.."
	kill -s HUP `cat ${DAEMON_HOME}/daemonbase.pid`
	;;
    * )
	echo "invalid parameter..."
	;;
esac
