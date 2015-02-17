#GENERIC RULES FOR BUILDING A PROJECT FOR TI TM4C123G
#May Be buggy
#Losely based on TI StellarisWare examples

include Makefile.common

#Define Compiler, linker, archiver, etc.
COMPILER=gcc
CC=${PREFIX}${COMPILER}


#
# The flags passed to the compiler.
#
CFLAGS=-mthumb             \
       ${CPU}              \
       ${FPU}              \
       -ffunction-sections \
       -fdata-sections     \
       -MD                 \
       -std=c99            \
       -Wall               \
       -Wextra             \
       -pedantic           \
       -DPART_${PART}      \


#Locate gcc libraries
LIBGCC=${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC=${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM=${shell ${CC} ${CFLAGS} -print-file-name=libm.a}
