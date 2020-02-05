# Pax et Mors
 ## A Quest for Pandalium
Game for GGJ2020

### To install :

#### Required libraries

- GF (https://github.com/GamedevFramework/gf)
Use develop branch
```
git checkout develop
```

- SFML-Audio
```
apt install libsfml-audio2.4
```

#### Download & Build
```
git clone https://github.com/Hatunruna/ggj2020.git
cd ggj2020/
git submodule update --init
mkdir build
cd build
cmake ..
make
make install (May need elevated privileges)
```

### To Run :

One server must be initialize
```
./pem-server
```
To run the game (n times for n players):
```
./pem
```
And connect to the server IP or hostname :

![alt text](Misc/LoginServer.png "Connexion window")
