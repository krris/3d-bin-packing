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


test_source = ['src/Guillotine2d.cpp', 'src/test.cpp', 'src/cuboid.cpp', 'src/vertex.cpp', 'src/shelf_algorithm.cpp']
env.Program('test', test_source)

source = ['src/3dBinPacking.cpp', 'src/cuboid.cpp', 'src/vertex.cpp']
env.Program('3dBinPacking', source)

