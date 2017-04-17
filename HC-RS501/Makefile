#ubuntu的内核源码树，如果要编译在ubuntu中安装的模块就打开这2个
#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build	

		
# 开发板的linux内核的源码树目录
KERN_DIR = /root/linux-3.0.86

obj-m	+= tiny4412_hc501.o

all:
	make -C $(KERN_DIR) M=`pwd` modules 

cp:
	cp *.ko /root/rootfs -rf

.PHONY: clean	
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	

