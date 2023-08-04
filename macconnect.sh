#!/bin/bash

#https://www.linuxquestions.org/questions/programming-9/dynamically-supply-input-to-netcat-in-a-script-793526/

# Function to generate a random 4-letter alphabetic name
generate_random_name() {
  cat /dev/urandom | LC_CTYPE=C tr -dc 'a-zA-Z' | fold -w 4 | head -n 1
}

SERVER='localhost'
PORT='6667'
INTERVAL='1'
NAME=$(generate_random_name)
REAL_NAME=$NAME
CHANNEL='#wez'
PASSWORD='1234'


input() {
	#Connect to the server, Identify, and join channel
  echo -e "CAP LS";
  echo -e "PASS $PASSWORD";
	echo -e "NICK $NAME";
	echo -e "USER $NAME 0 * :$REAL_NAME";
  echo -e "CAP END";
	echo -e "JOIN $CHANNEL";

  # Read user input and pass it to the server
  while IFS= read -r irc_command; do
    echo -e "$irc_command"
  done
  }
#Prevents a disconnect from a ping timeout
#	while true; do
#		pong
#	done;
#	}

#-i $INTERVAL 

function my_irc {
  input | /usr/bin/nc -C -N $SERVER $PORT
}

my_irc