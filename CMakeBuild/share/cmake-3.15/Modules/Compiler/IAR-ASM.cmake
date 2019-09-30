# This file is processed when the IAR compiler is used for an assembler file

include(Compiler/IAR)

if("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "ARM")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> -S <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_ilink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "RX")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> --silent <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_ilink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "RH850")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> --silent <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_ilink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "RL78")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> --silent <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_ilink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "RISC-V")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> --silent <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_ilink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "AVR")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> -S <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_xlink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s90;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "MSP430")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> -S <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_xlink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s43;asm;msa)

elseif("${CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID}" STREQUAL "V850")
  set(CMAKE_ASM_COMPILE_OBJECT  "<CMAKE_ASM_COMPILER> -S <SOURCE> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT>")
  __compiler_iar_xlink(ASM)
  set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS r85;asm;msa)

else()
  message(FATAL_ERROR "CMAKE_ASM${ASM_DIALECT}_COMPILER_ARCHITECTURE_ID not detected. This should be automatic.")
endif()
