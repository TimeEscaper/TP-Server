cmake_minimum_required(VERSION 3.6)
project(TP-Server)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)

set(SOURCE_FILES src/main.cpp src/server/ClientHandler.cpp include/server/ClientHandler.h src/server/Server.cpp include/server/Server.h src/server/WorkerThread.cpp include/server/WorkerThread.h src/thread/BaseThreadPool.cpp include/thread/BaseThreadPool.h src/thread/AbstractThreadHandler.cpp include/thread/AbstractThreadHandler.h include/helpers/utils.h src/helpers/utils.cpp src/http/http.cpp include/http/http.h src/server/WorkerThreadPool.cpp include/server/WorkerThreadPool.h src/helpers/MimeUtil.cpp include/helpers/MimeUtil.h)
add_executable(httpd ${SOURCE_FILES})

target_link_libraries (httpd ${CMAKE_THREAD_LIBS_INIT})