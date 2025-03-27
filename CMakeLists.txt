# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(qing LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# 设置编译选项
set(CMAKE_C_FLAGS "-Wall -g")

# 指定项目的头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# 设置源代码文件目录
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# 设置测试目录
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)


# 添加核心源文件
set(SOURCE_FILES
    ${SRC_DIR}/qing_tensor.c
    ${SRC_DIR}/qing_ops.c
    ${SRC_DIR}/qing_model.c
    ${SRC_DIR}/qing_backend.c
    ${SRC_DIR}/qing_utils.c
)

# 添加测试源文件
set(TEST_FILES
    ${TEST_DIR}/test_qing_tensor.c
    ${TEST_DIR}/test_qing_ops.c
    ${TEST_DIR}/test_qing_model.c
    ${TEST_DIR}/test_qing_backend.c
    ${TEST_DIR}/test_qing_utils.c
)

# 设置可执行文件
# add_executable(qing ${SOURCE_FILES})

# 设置测试可执行文件
add_executable(test_qing_tensor ${TEST_FILES} ${SRC_DIR}/qing_tensor.c ${SRC_DIR}/qing_utils.c)
target_include_directories(test_qing_tensor PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 目标链接库
# 如果有依赖库，可以在这里链接
# target_link_libraries(qing <LIBRARIES>)


# 添加 CTest 支持
enable_testing()

# 添加测试
add_test(NAME test_qing_tensor COMMAND test_qing_tensor)
add_test(NAME test_qing_ops COMMAND test_qing_ops)
add_test(NAME test_qing_model COMMAND test_qing_model)
add_test(NAME test_qing_backend COMMAND test_qing_backend)
add_test(NAME test_qing_utils COMMAND test_qing_utils)

# 为 CMake 提供一些输出信息
message(STATUS "Source files: ${SOURCE_FILES}")
message(STATUS "Test files: ${TEST_FILES}")
