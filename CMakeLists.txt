cmake_minimum_required(VERSION 3.20)

project(dej_os C ASM_NASM)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(KERNEL_DIR ${CMAKE_SOURCE_DIR}/src/kernel)
set(BUILD_DIR ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS
    ${KERNEL_DIR}/*.c
)

file(GLOB_RECURSE ASM_SOURCES CONFIGURE_DEPENDS
    ${KERNEL_DIR}/*.asm
)

set(CMAKE_C_FLAGS
    "-ffreestanding -fno-stack-protector -fno-pie -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-builtin -fno-omit-frame-pointer -mno-red-zone -m64 -mcmodel=kernel -std=c11 -Wall -Werror -g -mrdrnd"
)

set(CMAKE_ASM_NASM_FLAGS "-f elf64")

add_executable(kernel.elf
    ${C_SOURCES}
    ${ASM_SOURCES}
)

set_target_properties(kernel.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
    LINKER_LANGUAGE C
)

target_link_options(kernel.elf PRIVATE
    -T ${KERNEL_DIR}/linker.ld
)

add_custom_target(limine_build
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Limine
)

add_dependencies(kernel.elf limine_build)

add_custom_command(
    OUTPUT ${BUILD_DIR}/dej-os.img
    COMMAND rm -f ${BUILD_DIR}/dej-os.img
    COMMAND dd if=/dev/zero of=${BUILD_DIR}/dej-os.img bs=1M count=64
    COMMAND sgdisk -Z ${BUILD_DIR}/dej-os.img
    COMMAND sgdisk -n 1:2048:4095 -t 1:ef02 ${BUILD_DIR}/dej-os.img
    COMMAND sgdisk -n 2:4096:0 -t 2:ef00 ${BUILD_DIR}/dej-os.img
    COMMAND sudo losetup --find --partscan --show ${BUILD_DIR}/dej-os.img > ${BUILD_DIR}/loopdev
    COMMAND sudo partprobe $$(cat ${BUILD_DIR}/loopdev)
    COMMAND sudo mkfs.fat -F 32 $$(cat ${BUILD_DIR}/loopdev)p2
    COMMAND mkdir -p ${BUILD_DIR}/mnt
    COMMAND sudo mount $$(cat ${BUILD_DIR}/loopdev)p2 ${BUILD_DIR}/mnt
    COMMAND sudo mkdir -p ${BUILD_DIR}/mnt/boot/limine
    COMMAND sudo cp ${BUILD_DIR}/kernel.elf ${BUILD_DIR}/mnt/boot/kernel.elf
    COMMAND sudo cp ${CMAKE_SOURCE_DIR}/limine.conf ${BUILD_DIR}/mnt/limine.conf
    COMMAND sudo cp ${CMAKE_SOURCE_DIR}/Limine/bin/limine-bios.sys ${BUILD_DIR}/mnt/boot/limine/limine-bios.sys
    COMMAND sudo umount ${BUILD_DIR}/mnt
    COMMAND sudo losetup -d $$(cat ${BUILD_DIR}/loopdev)
    COMMAND rm -f ${BUILD_DIR}/loopdev
    COMMAND ${CMAKE_SOURCE_DIR}/Limine/bin/limine bios-install ${BUILD_DIR}/dej-os.img
    DEPENDS kernel.elf ${CMAKE_SOURCE_DIR}/limine.conf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(image ALL
    DEPENDS ${BUILD_DIR}/dej-os.img
)

add_custom_target(run
    COMMAND qemu-system-x86_64 -drive format=raw,file=${BUILD_DIR}/dej-os.img
    DEPENDS image
)
