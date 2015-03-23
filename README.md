# circle-routes
Circle network routes evaluation utility

## Dependencies

* ```circle_routes``` has no external dependencies
* Http Server: Poco v1.6.0 (```-lPocoFoundation``` ```-lPocoNet``` ```-lPocoUtil``` ```-lPocoJSON```)
* Command line utility: Boost 1.57 (```-lboost_system``` ```-lboost_program_options```)

## Build

In order to build under **Linux** just run the following commands:

```bash
mkdir build && cd build
cmake ..
make
```

To build under **Windows** you will need additional packages. Also Boost and Poco have to be built using mingw32 toolchain to mingw32 folder, fot instance, ```$HOME/mingw-install/```

```bash
apt-get install wine mingw32 mingw32-binutils mingw32-runtime
mkdir buildwin && cd buildwin
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-mingw32.cmake -DCMAKE_INSTALL_PREFIX=$HOME/mingw-install/ ..
```
