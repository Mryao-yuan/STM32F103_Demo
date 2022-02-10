startup：STARTUP_STM32f10x_hd.s


cmsis:core_cm3.c \system_stm32f10x.c
 B
user:main.c \ stm32f10x_it.c

fwlib(外设库）：src下的文件(.../.C文件)


在c/c++中加入（USE_STDPERIPH_DRIVER,
===========
#ifdef USE_STDPERIPH_DRIVER
  #include "stm32f10x_conf.h"



hex文件通过串口下载



