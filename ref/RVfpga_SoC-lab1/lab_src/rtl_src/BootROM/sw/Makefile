TOOLCHAIN_PREFIX ?= ${HOME}/.platformio/packages/toolchain-riscv/bin/riscv64-unknown-elf-
RESET_VECTOR ?= 0
FLASH_ADDR ?= 0x0

%.elf: %.S
	$(TOOLCHAIN_PREFIX)gcc -nostartfiles -march=rv32i -mabi=ilp32 -Tlink.ld -o$@ $<
%.vh: %.bin
	python3 makehex.py $< > $@
%.bin: %.elf
	$(TOOLCHAIN_PREFIX)objcopy -O binary $< $@
%.dis: %.elf
	$(TOOLCHAIN_PREFIX)objdump -d $< > $@
%.ihex: %.elf
	$(TOOLCHAIN_PREFIX)objcopy -O ihex $< $@
clean:
	rm -f *.elf *.bin *.vh *.dis *.mem *.vcd
