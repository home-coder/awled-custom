export ARCH=arm
export CROSS_COMPILE=/home/java/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-
LINUX_PATH :=/home/java/opt/hhzn/kernel_imx

obj-m += mechanism.o

all:
	make -C $(LINUX_PATH) M=`pwd` modules
clean:
	make -C $(LINUX_PATH) M=`pwd` modules clean
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order *.o.cmd *.swp *.c~
