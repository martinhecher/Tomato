@echo off

rem Script to start a VS2010 solution with a correctly setup environment in
rem dropping the solution file onto this .bat file.
rem author: Martin Hecher
rem email:  martin.hecher@fraunhofer.at

rem Path to OpenSG2 installation:
set OPENSG_ROOT=D:/Coding/Libs/OpenSG2-Februar2012/OpenSG.install

rem Path to OpenSG2 support libs installation:
set OSG_SUPPORT_ROOT=D:/Coding/Libs/OpenSG2-Februar2012/OpenSG.Support.install

rem ---------------------------------------------------------------------------
rem The next lines should not be changed. At the end of the script you can adjust
rem the application start options.

rem Preload importer libraries. CAUTION: OSGColladaIO has to be loaded BEFORE OSGFileIO,
rem to override the built-in collada loader of OpenSG!
set OSG_LOAD_LIBS=OSGFileIO;OSGImageFileIO;OSGContribPLY

rem Set the system path. DO NOT CHANGE THIS LINE!
rem Do not forget to add %PATH% at the end. The OSGVSLBTFNode seems to need a dll from the
rem default system path. Otherwise very weird segfaults occur when loading a BTF model!
set PATH=%OPENSG_ROOT%\bin\rel;%OPENSG_ROOT%\bin\debug;%OSG_SUPPORT_ROOT%\bin;%OSG_SUPPORT_ROOT%\lib
rem ---------------------------------------------------------------------------

rem Start the solution
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\VCExpress.exe" %1
