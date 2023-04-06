# Chat App
 Chat app based on socket tcp communication from client to server and vice versa.


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