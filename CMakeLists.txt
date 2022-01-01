include(CheckCCompilerFlag)
cmake_minimum_required(VERSION 3.15)

project(device_model
        LANGUAGES C CXX)


add_executable(device_model
        main.c
        device.h
        device.c
        i2c_mcu_impl.c
        i2c.h
        icm4000.c
        icm4000.h
        accel.h
        )


target_compile_options(device_model PRIVATE
        -Wall
        -Wextra
        -Wshift-count-overflow
        -g2
        )


add_custom_command(TARGET device_model POST_BUILD
        COMMAND size device_model)
