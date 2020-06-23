#!/bin/sh
# !!!!!! Add this script into your build dir !!!!!!
trap ctrl_c INT

if [ $# -eq 1 ]
then
  NBPROC=$1
else
  NBPROC=1
fi

kill_process() {
  kill -s 15 $client1
  kill -s 15 $client2
  kill -s 15 $client3
  kill -s 15 $client4
  #kill -9 $client5
  #kill -9 $client6
  #kill -9 $client7
  #kill -9 $client8
  sleep 1
  kill -s 2 $server
}

ctrl_c() {
  echo "** Trapped CTRL-C"
  echo "killing Server and client ..."
  kill_process
  exit 0
}

while true
do
  fuser 2020/tcp
  echo "Compiling ..."
  make -j $NBPROC || exit 1
  echo "Running Server..."
  ./pax_et_mors_server > server.log 2>&1 &
  server=$!

  echo "and x clients..."
  ./pax_et_mors --debug client1 > client1.log 2>&1 &
  client1=$!
  sleep 1
  ./pax_et_mors --debug client2 > client2.log 2>&1 &
  client2=$!
  sleep 1
  ./pax_et_mors --debug client3 > client3.log 2>&1 &
  client3=$!
  sleep 1
  ./pax_et_mors --debug client4 > client4.log 2>&1 &
  client4=$!
  sleep 1
  #./pax_et_mors &
  #client5=$!
  #./pax_et_mors &
  #client6=$!
  #./pax_et_mors &
  #client7=$!
  #./pax_et_mors &
  #client8=$!

  echo -e "\e[5mPress Enter to make and start the server and clients again\e[25m"
  read var1
  echo "killing Server and client ..."
  kill_process
done
