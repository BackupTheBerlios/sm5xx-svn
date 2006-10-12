# Makefile for the Silicon Motion SM5xx core drivers

obj-$(CONFIG_SM5XX) += sm5xx-core.o
sm5xx-core-objs	    := sm5xx_bus.o

ifeq ($(CONFIG_SM5XX_PCI),y)
sm5xx-core-objs += sm5xx_pci.o
else
sm5xx-core-objs += sm5xx_platform.o
endif

