################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432E401Y_TIRTOS.cmd 

SYSCFG_SRCS += \
../udpecho.syscfg 

C_SRCS += \
../commands.c \
../main_tirtos.c \
../parser.c \
../tasks.c \
../udpEcho.c \
../udpEchoHooks.c \
./syscfg/ti_net_config.c \
./syscfg/ti_ndk_config.c \
./syscfg/ti_drivers_config.c 

GEN_FILES += \
./syscfg/ti_net_config.c \
./syscfg/ti_ndk_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./commands.d \
./main_tirtos.d \
./parser.d \
./tasks.d \
./udpEcho.d \
./udpEchoHooks.d \
./syscfg/ti_net_config.d \
./syscfg/ti_ndk_config.d \
./syscfg/ti_drivers_config.d 

OBJS += \
./commands.obj \
./main_tirtos.obj \
./parser.obj \
./tasks.obj \
./udpEcho.obj \
./udpEchoHooks.obj \
./syscfg/ti_net_config.obj \
./syscfg/ti_ndk_config.obj \
./syscfg/ti_drivers_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.exp \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"commands.obj" \
"main_tirtos.obj" \
"parser.obj" \
"tasks.obj" \
"udpEcho.obj" \
"udpEchoHooks.obj" \
"syscfg\ti_net_config.obj" \
"syscfg\ti_ndk_config.obj" \
"syscfg\ti_drivers_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.exp" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"commands.d" \
"main_tirtos.d" \
"parser.d" \
"tasks.d" \
"udpEcho.d" \
"udpEchoHooks.d" \
"syscfg\ti_net_config.d" \
"syscfg\ti_ndk_config.d" \
"syscfg\ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_net_config.c" \
"syscfg\ti_ndk_config.c" \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../commands.c" \
"../main_tirtos.c" \
"../parser.c" \
"../tasks.c" \
"../udpEcho.c" \
"../udpEchoHooks.c" \
"./syscfg/ti_net_config.c" \
"./syscfg/ti_ndk_config.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../udpecho.syscfg" 


