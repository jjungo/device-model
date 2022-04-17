include(CheckCCompilerFlag)
cmake_minimum_required(VERSION 3.15)

project(device-model
        LANGUAGES C CXX)


add_executable(device-model
        main.c
        device.h
        device.c
        i2c_mcu_impl.c
        i2c.h
        spi.h
        spi_mcu_impl.c
        icm4000.c
        icm4000.h
        accel.h
        )


target_compile_options(device-model PRIVATE
        -Wall
        -Wextra
        -Wshift-count-overflow
        -g2
        )


add_custom_command(TARGET device-model POST_BUILD
        COMMAND size device-model)
