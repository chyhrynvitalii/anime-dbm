add_library(file STATIC file.c file.h)
target_include_directories(file PUBLIC .)

add_library(get STATIC get.c get.h)
target_include_directories(get PUBLIC .)

add_library(maths STATIC maths.c maths.h)
target_include_directories(maths PUBLIC .)

add_library(str STATIC str.c str.h)
target_include_directories(str PUBLIC .)