# Chat App
 Chat app based on socket tcp communication from client to server and vice versa.

## How to use

### Server Application

For default settings, just double click the ServerApp.exe, it will run under the port 8888.

To specify the room/port or several rooms, run as:

```ServerApp.exe <port> [<port> ...]```

E.g. execute server with two rooms:

```ServerApp.exe 8888 9898```

### Client Application

For default settings, just double click the ClientApp.exe, it will run under the port 8888 with the username Anonymous.

To specify the room/port or several rooms, run as:

```ClientApp.exe <username> <host> <port>```

E.g.:

```ClientApp.exe Joao 127.0.0.1 9898```


## Dependencies

### Boost
[Boost C++](https://www.boost.org) library is required for this project to work. 

This project was build using the version referred at the CMakeLists file.

Do not forget to add it to your environmental variables as "BOOST_ROOT" with the path to its installation. If you still face any issues, run the "bootstrap.bat" and then the "b2.exe".

Another easier solution is to make sure you have on your CMake files:

```
SET(CMAKE_INCLUDE_PATH${CMAKE_INCLUDE_PATH} "C:/path_to_boost/boost_1_81_0")
SET(CMAKE_LIBRARY_PATH${CMAKE_LIBRARY_PATH} "C:/path_to_boost/boost_1_81_0/libs")
```