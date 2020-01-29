#!/bin/sh
# !!!!!! Add this script into your build dir !!!!!!
while true
do
  echo "Building ..."
  make
  echo "Running Server and 2 clients..."
  ./tmpgame-server &
  server=$!
  ./tmpgame &
  client1=$!
  ./tmpgame &
  client2=$!
  while true
  do
    echo "\e[5mPress Enter to make and start the server and clients again\e[25m"
    read var1
    echo "killing Server and client ..."
    kill -9 $server
    kill -9 $client1
    kill -9 $client2
    break
  done
done
