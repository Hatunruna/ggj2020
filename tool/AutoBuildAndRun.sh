#!/bin/sh
# !!!!!! Add this script into your build dir !!!!!!
trap ctrl_c INT

ctrl_c() {
  echo "** Trapped CTRL-C"
  echo "killing Server and client ..."
  kill -9 $server
  kill -9 $client1
  kill -9 $client2
  kill -9 $client3
  kill -9 $client4
  #kill -9 $client5
  #kill -9 $client6
  #kill -9 $client7
  #kill -9 $client8
  fuser 2020/tcp
  exit 0
}

while true
do
  fuser 2020/tcp
  echo "Compiling ..."
  make -j 8
  echo "Running Server..."
  ./tmpgame-server > server.log 2>&1 &
  server=$!

  echo "and x clients..."
  ./tmpgame --debug client1 > client1.log 2>&1 &
  client1=$!
  sleep 1
  ./tmpgame --debug client2 > client2.log 2>&1 &
  client2=$!
  sleep 1
  ./tmpgame --debug client3 > client3.log 2>&1 &
  client3=$!
  sleep 1
  ./tmpgame --debug client4 > client4.log 2>&1 &
  client4=$!
  sleep 1
  #./tmpgame &
  #client5=$!
  #./tmpgame &
  #client6=$!
  #./tmpgame &
  #client7=$!
  #./tmpgame &
  #client8=$!
  while true
  do
    echo "\e[5mPress Enter to make and start the server and clients again\e[25m"
    read var1
    echo "killing Server and client ..."
    kill -2 $client1
    kill -2 $client2
    kill -2 $client3
    kill -2 $client4
    #kill -9 $client5
    #kill -9 $client6
    #kill -9 $client7
    #kill -9 $client8
    kill -s 2 $server
    break
  done
done


