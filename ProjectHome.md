# A Simulator of P-Waves Using Seismic Unix. #

This project was last compiled with Qt-4.4.3

To compile in linux use
```
    qmake -project
    qmake
    make
```

To compile in mac-os use
```
    qmake -project
    qmake -spec macx-g++ <name>.pro
    make
```


I have updated to Qt-4.7, and have tested the build on windows
just to see how it works. This is best built with QtCreator or
setting the path to use the mingw provided by the Qt installation.
The following are the commands to build on windows.

\# These are the same commands used by Qt Creator

```
    mingw32-make.exe clean -w
    
    qmake.exe Viper.pro -r -spec win32-g++
    mingw32-make.exe -w
```
If you wish to collaborate in this project contact edvelez.g@gmail.com