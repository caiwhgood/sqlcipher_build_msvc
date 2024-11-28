cmake_minimum_required(VERSION 3.8.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(OUTPUT_LIB_NAME sqlcipher)
set(OUTPUT_TEST test)
set(SQLCIPHER_DIR ${CMAKE_CURRENT_LIST_DIR}/sqlcipher_410)
set(OpenSSL_DIR ${CMAKE_CURRENT_LIST_DIR}/3rd/OpenSSL)

project(${OUTPUT_LIB_NAME} LANGUAGES C)
project(${OUTPUT_TEST} LANGUAGES CXX)

set(BUILD_MODE $<IF:$<CONFIG:Debug>,Debug,Release>)
message(---BUILD_MODE ${BUILD_MODE})

set(SSL_LIB_DIR ${OpenSSL_DIR}/lib/${_PLATFORM}/${BUILD_MODE})

set(SQLCIPHER_SRC 
    ${SQLCIPHER_DIR}/shell.c
    ${SQLCIPHER_DIR}/sqlite3.c
    ${SQLCIPHER_DIR}/sqlite3.h
    ${SQLCIPHER_DIR}/sqlite3ext.h
)

file(GLOB_RECURSE OpenSSL_DIR_SOURCE 
    ${OpenSSL_DIR}/include/openssl/*.h
    ${OpenSSL_DIR}/include/openssl/*.H
)

message(OpenSSL_DIR_SOURCE :${OpenSSL_DIR_SOURCE})

add_library(${OUTPUT_LIB_NAME} STATIC ${SQLCIPHER_SRC} ${OpenSSL_DIR_SOURCE})


target_link_directories(${OUTPUT_LIB_NAME} PRIVATE ${SSL_LIB_DIR})

target_include_directories(${OUTPUT_LIB_NAME}  PUBLIC
    ${SQLCIPHER_DIR}
    ${OpenSSL_DIR}/include
)

target_link_libraries(${OUTPUT_LIB_NAME} PRIVATE 
    ${SSL_LIB_DIR}/libssl_static.lib
    ${SSL_LIB_DIR}/libcrypto_static.lib
    Ws2_32
    Crypt32
    shlwapi
)

target_compile_definitions(
    ${OUTPUT_LIB_NAME} 
    PRIVATE
        $<$<CONFIG:Debug>:SQLITE_DEBUG>
        -DSQLCIPHER_CRYPTO_OPENSSL
        -DSQLITE_ENABLE_RTREE
        -DSQLITE_ENABLE_UNLOCK_NOTIFY
        -DSQLITE_ENABLE_COLUMN_METADATA
		-DSQLITE_HAS_CODEC
		-DSQLITE_TEMP_STORE=2
        -DSQLITE_4_BYTE_ALIGNED_MALLOC=1 
        -DSQLITE_SYSTEM_MALLOC=1
        -DSQLITE_OS_WIN=1
        -DINCLUDE_MSVC_H=1
        -DWIN32_LEAN_AND_MEAN
        -DOPENSSL_NO_DEPRECATED_1_1_0
)



# add_executable(${OUTPUT_TEST} ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
# target_include_directories(${OUTPUT_TEST}  PUBLIC
#     ${SQLCIPHER_DIR}
# )
# target_link_libraries(${OUTPUT_TEST} PRIVATE ${OUTPUT_LIB_NAME})

