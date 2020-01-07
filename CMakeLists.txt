# cmake 最低版本要求
cmake_minimum_required(VERSION 2.8)

#项目
project(Cpp_template)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

set(LIBRARY_GLAG -lpthread)

#递归枚举对应目录下的的指定后缀文件
#若是不用递归寻找 GLOB_RECURSE ==> GLOB
file(GLOB_RECURSE SRC 
    ${PROJECT_SOURCE_DIR}/src/*.cc 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
message("Network_src ${SRC}")

#生成指定目标
add_executable(test.out 
    #main.cc
    ${SRC}
)

TARGET_LINK_LIBRARIES(test.out
    ${LIBRARY_GLAG}
)
