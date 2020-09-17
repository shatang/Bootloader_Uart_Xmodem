SET PATH=d:\Keil_v5\ARM\ARMCC\Bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;D:\Program Files (x86)\IDM Computer Solutions\UltraEdit;D:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin;D:\Git\cmd;D:\TortoisGit\bin;C:\Users\bzhang\AppData\Local\Microsoft\WindowsApps;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F407ZG
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Bootloader
SET CPU_CLOCK=0x0A037A00
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\main.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_it.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\system_stm32f4xx.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\obj\startup_stm32f40_41xxx._ia"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\misc.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_gpio.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_rcc.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_syscfg.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_usart.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_flash.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_tim.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\timer_driver.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\rs232_uart2_driver.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\rs485_uart4_driver.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\common.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\download.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\flash_if.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\xmodem.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\crc16.__i"
"d:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\OBJ\Shatang.lnp"
"d:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\OBJ\Shatang.axf" --i32combined --output "..\OBJ\Shatang.hex"
d:\Keil_v5\\ARM\ARMCC\bin\fromelf.exe --bin --output=Shatang.bin ..\OBJ\Shatang.axf
