#!/bin/sh
# !!!!!! Add this script into your build dir !!!!!!
while true
do
  echo "Building ..."
  make
  echo "Running Server..."lutr
  ./tmpgame-server &
  server=$!

  echo "and x clients..."
  ./tmpgame &
  client1=$!
  ./tmpgame &
  client2=$!
  ./tmpgame &
  client3=$!
  ./tmpgame &
  client4=$!
  ./tmpgame &
  client5=$!
  ./tmpgame &
  client6=$!
  ./tmpgame &
  client7=$!
  ./tmpgame &
  client8=$!
  while true
  do
    echo "\e[5mPress Enter to make and start the server and clients again\e[25m"
    read var1
    echo "killing Server and client ..."
    kill -9 $server
    kill -9 $client1
    kill -9 $client2
    kill -9 $client3
    kill -9 $client4
    kill -9 $client5
    kill -9 $client6
    kill -9 $client7
    kill -9 $client8
    break
  done
done