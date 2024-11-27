cmake_minimum_required(VERSION 3.8.0)

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

add_library(${OUTPUT_LIB_NAME} STATIC ${SQLCIPHER_SRC})

target_include_directories(${OUTPUT_LIB_NAME}  PUBLIC
    ${SQLCIPHER_DIR}
    ${OpenSSL_DIR}/include
)

target_link_libraries(${OUTPUT_LIB_NAME}  PRIVATE ${SSL_LIB_DIR}/libssl.lib ${SSL_LIB_DIR}/libcrypto.lib)

target_compile_definitions(
    ${OUTPUT_LIB_NAME} 
    PRIVATE
        $<$<CONFIG:Debug>:SQLITE_DEBUG>
        ${API}
        -DSQLITE_ENABLE_RTREE
        -DSQLITE_ENABLE_UNLOCK_NOTIFY
        -DSQLITE_ENABLE_COLUMN_METADATA
		-DSQLITE_HAS_CODEC
		-DSQLITE_TEMP_STORE=2
)
target_compile_options(${OUTPUT_LIB_NAME}  PRIVATE -Wall)


add_executable(${OUTPUT_TEST} ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
target_link_libraries(${OUTPUT_TEST} PRIVATE ${OUTPUT_LIB_NAME})
