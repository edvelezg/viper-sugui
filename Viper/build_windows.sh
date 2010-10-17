#It's very unlikely that anything built with 4.4.3 will fail to build with
#4.6.3 or 4.7.0.  If it does, it will most likely fail in compilation
#because of API differences, and not at link time.  It looks like the GCC
#C++ support library is not being found or you are linking a library built
#with a different version of GCC and/or Qt .  Are you using the Nokia
#provided SDK or have you built MingW for yourself?  Have your cleaned the
#project and any libraries, rerun qmake, and built from scratch?[/QUOTE]
#
#I have cleaned the project and made a brand new project file.  I am using
#the Nokia provided SDK, as I think it would be built to be compatible with
#the Qt that is being used.  I tried building it with QtCreator and it
#worked.  Seems like QtCreator adds a bit more to the commands.
#
#I have also tried using qmake mingw32-make

Ok I followed as you suggested and decided to use Qt Creator to do this, as 
it has the right commands and it worked.  I was wondering though, I want 
to be able to build other things using my own cygwin make in other projects, 
and at the same time I want to be able to build using the mingw32-make for this 
project without QtCreator.  I am not sure what to do, because if I have 
C:/Cygwin/bin in my path, mingw32-make calls the wrong g++, and I get all those
ugly errors.



# These are the build commands used by QtCreator

mingw32-make.exe clean -w

qmake.exe Viper.pro -r -spec win32-g++
mingw32-make.exe -w

