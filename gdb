qemu-system-aarch64 -M virt -cpu cortex-a57 -nographic -kernel hypervisor.elf -S -s &
gdb-multiarch -q -ex 'file hypervisor.elf' -ex 'target remote localhost:1234'
kill %1
