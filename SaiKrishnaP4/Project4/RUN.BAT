:run.bat

cd CppCli-WPF-App/Debug/ 


@echo OFF
@echo:

@echo:
echo Input is given in a .txt file that is read as a string
START Server.exe localhost:9000 ../../TestDir/Server1/
START Server.exe localhost:9010 ../../TestDir/Server2/
START CppCli-WPF-App.exe localhost:8090 ../../TestDir/Application/

@echo:

cd ../..