import os, shutil, platform, re
import SCons.Builder

env = Environment()

env.Append( ENV = {'PATH' : os.environ['PATH'] })

if(platform.system() == "Linux"):
   

   env.Append( CPPFLAGS = '-Wall -pedantic -std=c++0x -g' )
   env.Append( LINKFLAGS = '-Wall' )
   env.Append( LIBS = ['boost_serialization'] )

elif(platform.system() == "Windows"):

   env.Append( CPPFLAGS = ' /EHsc /MD /D "WIN32" /D "_CONSOLE" /W4' )
   env.Append( LINKFLAGS = ' /SUBSYSTEM:WINDOWS ' )
else:
   print platform.system() + " not supported"


source = ['src/3dBinPacking.cpp', 'src/guillotine2d.cpp', 
			   'src/shelf_algorithm.cpp', 'src/guillotine3d.cpp']
env.Program('3dBinPacking', source)

