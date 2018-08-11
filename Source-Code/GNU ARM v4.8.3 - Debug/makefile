################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/efm32tg/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BLDC_Controller.axf

# Tool invocations
BLDC_Controller.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "BLDC_Controller.ld" -Xlinker --gc-sections -Xlinker -Map="BLDC_Controller.map" --specs=nano.specs -o BLDC_Controller.axf "./src/BLDC_Main.o" "./src/em_acmp.o" "./src/em_adc.o" "./src/em_aes.o" "./src/em_assert.o" "./src/em_burtc.o" "./src/em_cmu.o" "./src/em_cryotimer.o" "./src/em_crypto.o" "./src/em_dac.o" "./src/em_dbg.o" "./src/em_dma.o" "./src/em_ebi.o" "./src/em_emu.o" "./src/em_gpcrc.o" "./src/em_gpio.o" "./src/em_i2c.o" "./src/em_idac.o" "./src/em_int.o" "./src/em_lcd.o" "./src/em_ldma.o" "./src/em_lesense.o" "./src/em_letimer.o" "./src/em_leuart.o" "./src/em_mpu.o" "./src/em_msc.o" "./src/em_opamp.o" "./src/em_pcnt.o" "./src/em_prs.o" "./src/em_rmu.o" "./src/em_rtc.o" "./src/em_rtcc.o" "./src/em_timer.o" "./src/em_usart.o" "./src/em_vcmp.o" "./src/em_wdog.o" "./emlib/em_system.o" "./CMSIS/efm32tg/startup_gcc_efm32tg.o" "./CMSIS/efm32tg/system_efm32tg.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: BLDC_Controller.hex'
	arm-none-eabi-objcopy -O ihex "BLDC_Controller.axf" "BLDC_Controller.hex"
	@echo ' '

	@echo 'Building bin file: BLDC_Controller.bin'
	arm-none-eabi-objcopy -O binary "BLDC_Controller.axf" "BLDC_Controller.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "BLDC_Controller.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) BLDC_Controller.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
