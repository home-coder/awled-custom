export ARCH=arm
#export CROSS_COMPILE=/home/java/basic/cross_compile/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-
export CROSS_COMPILE=/home/jiangxj/a810/sdk/lichee/brandy/gcc-linaro/bin/arm-linux-gnueabi-
LINUX_PATH :=/home/java/opt/hhzn/kernel_imx
LINUX_PATH :=~/a810/sdk/lichee/linux-3.4

obj-m += mechanism.o

all:
	make -C $(LINUX_PATH) M=`pwd` modules
clean:
	make -C $(LINUX_PATH) M=`pwd` modules clean
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order *.o.cmd *.swp *.c~
