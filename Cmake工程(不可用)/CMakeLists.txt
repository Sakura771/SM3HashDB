# cmake最小版本要求
cmake_minimum_required(VERSION 3.5.1)  
# 设置C++编译标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_ASM_COMPILER gcc)
#set(CMAKE_MAKE_PROGRAM, mingw32-make)
# 项目名称
project(HashDB) 
#主要配置
include_directories(include)  #include文件夹位置
link_directories(lib) #lib文件夹位置
link_libraries(libmysql.dll)  #lib下dll文件名

 #与本次配置无关，一般由项目自动生成（注意要在引入文件夹的下面）
add_executable(HashDB   src/SM3HashDB.cpp) 

#主要配置第一个参数为项目名
target_link_libraries(HashDB libmysql)

#设置静态编译
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

#链接静态库
#链接boost静态库(project_Name为你的项目名，*分别代表库名、mingw版本号、boost库版本号，如：libboost_system-mgw72-mt-s-1_65_1.a)
#target_link_libraries(HashDB libboost_*-mgw*-mt-s-*.a)
#链接gcc-g++静态库
# target_link_libraries(HashDB libgcc.a)
# target_link_libraries(HashDB libstdc++.a)
#链接网络库（如需要）
#target_link_libraries(project_Name ws2_32)
#链接线程库（必须放到最后）
#target_link_libraries(project_Name libpthread.a)
