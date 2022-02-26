#!/bin/bash


SCRIPT_PATH=`pwd`;


cd $SCRIPT_PATH


echo Press [CTRL+C] to stop mining.


while :


do


./mbrocoin-cli generatetoaddress 1 $(./mbrocoin-cli getnewaddress)



