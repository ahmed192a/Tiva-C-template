

# run QEMU using Tiva-C and pass it myproject.bin to run it
qemu-system-arm -M lm3s6965evb -nographic -kernel myproject.elf  -serial tcp::5678,server=on,wait=off

telnet localhost 5678
