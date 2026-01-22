all : sm3.c sm4.c bootstrap.S
	riscv64-unknown-elf-gcc sm3.c bootstrap.S -o sm3 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax
	riscv64-unknown-elf-gcc sm4.c bootstrap.S -o sm4 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sm3-sim: all
	/home/x/riscv-vp-mine/vp/build/bin/tiny32-vp --intercept-syscalls sm3

sm4-sim: all
	/home/x/riscv-vp-mine/vp/build/bin/tiny32-vp --intercept-syscalls sm4

clean:
	rm -f sm3 sm4
