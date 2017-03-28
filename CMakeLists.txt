cmake_minimum_required(VERSION 3.6)
project(TP-Server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/server/ClientHandler.cpp include/server/ClientHandler.h src/server/Server.cpp include/server/Server.h src/server/WorkerThread.cpp include/server/WorkerThread.h src/thread/ThreadPool.cpp include/thread/ThreadPool.h src/thread/IThreadHandler.cpp include/thread/IThreadHandler.h)
add_executable(httpd ${SOURCE_FILES})