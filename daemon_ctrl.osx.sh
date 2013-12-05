#!/bin/sh

DAEMON_HOME=/Users/mitsuji/Documents/dev/daemonbase

case "$1" in
    "start" )
	echo "starting daemonbase.."
	${DAEMON_HOME}/daemon ${DAEMON_HOME}/daemonbase.pid ${DAEMON_HOME}/daemon.conf.posix.osx
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
