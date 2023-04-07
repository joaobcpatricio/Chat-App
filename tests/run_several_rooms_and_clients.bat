cd ..
cd cmake-build-debug/server

start ServerApp.exe 8585 8888

cd ..
cd client

start ClientApp.exe
start ClientApp.exe Joao 127.0.0.1 8888
start ClientApp.exe Fiona 127.0.0.1 8585
start ClientApp.exe Lara 127.0.0.1 8585

echo Press any key to close programs...
pause >nul

taskkill /f /im ServerApp.exe
taskkill /f /im ClientApp.exe