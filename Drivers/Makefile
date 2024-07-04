
obj-m += GlassMotor/GlassMotor.o
obj-m += ShieldMotor/ShieldMotor.o
obj-m += TiltMotor/TiltMotor.o


KDIR := /lib/modules/$(shell uname -r)/build


PWD := $(shell pwd)

G_DEV_FILE := /dev/GlassMotorDriver
S_DEV_FILE := /dev/ShieldMotorDriver
T_DEV_FILE := /dev/TiltMotorDriver


default:

	$(MAKE) -C $(KDIR) M=$(PWD) modules

	@sudo insmod GlassMotor/GlassMotor.ko
	@sudo insmod ShieldMotor/ShieldMotor.ko
	@sudo insmod TiltMotor/TiltMotor.ko

	@sudo chmod 666 $(G_DEV_FILE)
	@sudo chmod 666 $(S_DEV_FILE)
	@sudo chmod 666 $(T_DEV_FILE)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

	@rm -f GlassMotor/*.ko
	@rm -f ShieldMotor/*.ko
	@rm -f TiltMotor/*.ko

	@sudo rmmod GlassMotor
	@sudo rmmod ShieldMotor
	@sudo rmmod TiltMotor
