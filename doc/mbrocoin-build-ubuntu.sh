#!/bin/sh

swapoff -a
fallocate -l 3G /swapfile  
chown root:root /swapfile  
chmod 0600 /swapfile  
sudo bash -c "echo 'vm.swappiness = 10' >> /etc/sysctl.conf"  
mkswap /swapfile  
swapon /swapfile    
echo '/swapfile none swap sw 0 0' >> /etc/fstab
free -m 
df -h

cd ~ && sudo apt-get update && sudo apt-get upgrade -y &&
sudo apt-get install git curl cmake automake python3 bsdmainutils libtool autotools-dev libboost-all-dev libssl-dev libevent-dev libdb++-dev libminiupnpc-dev libprotobuf-dev protobuf-compiler pkg-config net-tools build-essential -y &&

# Install the repository ppa:bitcoin/bitcoin
sudo apt-get install software-properties-common -y &&
sudo add-apt-repository ppa:bitcoin/bitcoin -y &&
sudo apt-get update -y &&

cd ~ 

git clone -b master https://github.com/mbrocoin/mbrocoin.git

cd mbrocoin

#use Berkeley DB 4.8
MBROCOIN_ROOT=$(pwd)
BDB_PREFIX="${MBROCOIN_ROOT}/db4"
mkdir -p $BDB_PREFIX
wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
tar -xzvf db-4.8.30.NC.tar.gz
cd db-4.8.30.NC/build_unix/
../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX
make install

cd $MBROCOIN_ROOT

./autogen.sh
 
./configure --with-incompatible-bdb --enable-upnp-default --without-gui LDFLAGS="-L${BDB_PREFIX}/lib/" CXXFLAGS="-O2" CPPFLAGS="-I${BDB_PREFIX}/include/" CXXFLAGS="--param ggc-min-expand=1 --param ggc-min-heapsize=32768" --enable-hardening

make

make install

sudo ufw enable -y 
sudo ufw allow 14141/tcp
sudo ufw allow 14142/tcp
sudo ufw allow 22/tcp

sudo mkdir ~/.mbrocoin

cat << "CONFIG" >> ~/.mbrocoin/mbrocoin.conf
daemon=1
listen=1
server=1
staking=0
rpcport=14141
port=14142
rpcuser=MMyw8qFmVbsttGuPD787oLvmb4kPr796Yx
rpcpassword=c=MBRO
rpcconnect=127.0.0.1
rpcallowip=127.0.0.1
addnode=51.195.249.132
addnode=158.69.130.185
CONFIG

chmod 700 ~/.mbrocoin/mbrocoin.conf
chmod 700 ~/.mbrocoin
ls -la ~/.mbrocoin 
cd ~
cd /usr/local/bin 

mbrocoind -daemon -txindex
