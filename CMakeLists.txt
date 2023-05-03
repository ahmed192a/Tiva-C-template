cmake_minimum_required(VERSION 3.5)
project(myproject)

# export compile comaand
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -g -Wall -fdata-sections -ffunction-sections")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/tm4c123g.ld -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/tm4c123g.ld -specs=${PROJECT_SOURCE_DIR}/tiva.specs")

# include(tm4c123g.cmake)

include_directories(
    # ${CMAKE_SOURCE_DIR}/CMSIS
    ${CMAKE_SOURCE_DIR}/inc
)



add_library(libusb STATIC IMPORTED)
set_target_properties(libusb PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libusb.a)


add_executable(${PROJECT_NAME}.elf src/led.c startup.c)


target_link_libraries(${PROJECT_NAME}.elf
    libusb
    -static
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
) # add_custom_command to generate .hex and .bin files from .elf file 

set(FLASH_EXECUTABLE "lm4flash")
ADD_CUSTOM_TARGET("flash" DEPENDS ${CMAKE_PROJECT_NAME}.elf 
  COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin 
  COMMAND ${FLASH_EXECUTABLE} ${CMAKE_PROJECT_NAME}.bin
)


# # add a custom command to debug the .elf file with gdb
# add_custom_target(debug
#     COMMAND stlink-gdb ${PROJECT_NAME}.elf -x ${CMAKE_SOURCE_DIR}/gdbinit
#     DEPENDS ${PROJECT_NAME}.elf
# )


# to install arm-none-eabi-gdb on ubuntu 20.04 run the following command
# sudo apt install gdb-arm-none-eabi