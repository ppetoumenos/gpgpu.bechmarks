include ../../../common/make.config

ifdef OUTPUT
override OUTPUT = -DOUTPUT
endif

# OpenCL SDK installation path
#AMD_OPENCL_DIR = /home/mwb7w/AMD-APP-SDK-v2.5-TC1-lnx64/

# OpenCL include and library paths
#OPENCL_INC = $(AMD_OPENCL_DIR)/include
#OPENCL_LIB = $(AMD_OPENCL_DIR)/lib/x86_64

# Matrix library path
MATRIX_DIR = ../meschach_lib

# C compiler
CC = gcc
CC_FLAGS = -g -O3 -Wall -I$(MATRIX_DIR) $(CFLAGS) $(EXTRA_CFLAGS)


leukocyte: detect_main.o avilib.o find_ellipse.o find_ellipse_opencl.o track_ellipse.o track_ellipse_opencl.o misc_math.o OpenCL_helper_library.o $(MATRIX_DIR)/meschach.a
	$(CC) $(CC_FLAGS) -lm avilib.o find_ellipse.o find_ellipse_opencl.o track_ellipse.o track_ellipse_opencl.o misc_math.o OpenCL_helper_library.o detect_main.o -o leukocyte $(MATRIX_DIR)/meschach.a -lm  $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) $(OUTPUT) $(CC_FLAGS) $< -c

detect_main.o: detect_main.c find_ellipse.h track_ellipse.h avilib.h

find_ellipse.o: avilib.h

track_ellipse.o: find_ellipse.h avilib.h

$(MATRIX_DIR)/meschach.a:
	cd $(MATRIX_DIR); make all; make clean
	#cd $(MATRIX_DIR); ./configure --with-all; make all; make clean

clean:
	rm -f *.o *~ leukocyte
