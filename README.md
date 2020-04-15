**lib-deamon-cpp**

**Version:** 1.0.1

**Release Date:** 2020-04-15

Library to make daemons in c++ projects.

**Build**
```
git clone https://github.com/PawelJanik/lib-daemon-cpp.git
cd lib-daemon-cpp
mkdir build
cd build
cmake ..
make
```

**Install**
```
make install (as root user)
ldconfig (as root user)
```

**Example**

CPUtempLogger - simple example of a daemon that logging processor temperature

**Build example**
```
cd to example/CPUtempLogger
mkdir build
cd build
cmake ..
make
```

**Install example**

```
make install (as root user)
```

**Run example daemon**

```
systemctl daemon-reload
systemctl start cputemplogger
```
To see logging data

```
cat /var/log/cputemplogger.log
```
