K=kernel
U=user

# Directory structure
BUILD_DIR = build
ASM_DIR = $(BUILD_DIR)/asm
SYM_DIR = $(BUILD_DIR)/sym
OBJ_DIR = $(BUILD_DIR)/obj
KERNEL_OBJ_DIR = $(OBJ_DIR)/kernel
USER_OBJ_DIR = $(OBJ_DIR)/user

# Create necessary directories
$(shell mkdir -p $(ASM_DIR) $(SYM_DIR) $(KERNEL_OBJ_DIR) $(USER_OBJ_DIR))

# Modify kernel object paths
KOBJS = $(addprefix $(KERNEL_OBJ_DIR)/, \
  entry.o \
  start.o \
  console.o \
  printf.o \
  uart.o \
  kalloc.o \
  spinlock.o \
  string.o \
  main.o \
  vm.o \
  proc.o \
  swtch.o \
  trampoline.o \
  trap.o \
  syscall.o \
  sysproc.o \
  bio.o \
  fs.o \
  log.o \
  sleeplock.o \
  file.o \
  pipe.o \
  exec.o \
  sysfile.o \
  kernelvec.o \
  plic.o \
  virtio_disk.o \
  shm.o \
  msgqueue.o \
  semaphore.o \
)

# Host compiler settings
HOST_CC = gcc
HOST_CFLAGS = -Wall -I.

# Tool definitions
TOOLPREFIX ?= $(shell if riscv64-unknown-elf-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-unknown-elf-'; \
	elif riscv64-linux-gnu-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-linux-gnu-'; \
	elif riscv64-unknown-linux-gnu-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv64-unknown-linux-gnu-'; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find a riscv64 version of GCC/binutils." 1>&2; \
	echo "*** To turn off this error, run 'gmake TOOLPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)

QEMU = qemu-system-riscv64
CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

# RISC-V compilation flags
RISCV_CFLAGS = -Wall -O -fno-omit-frame-pointer -ggdb
RISCV_CFLAGS += -MD
RISCV_CFLAGS += -mcmodel=medany
RISCV_CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
RISCV_CFLAGS += -I.
RISCV_CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

# PIE settings
ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]no-pie'),)
RISCV_CFLAGS += -fno-pie -no-pie
endif
ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]nopie'),)
RISCV_CFLAGS += -fno-pie -nopie
endif

LDFLAGS = -z max-page-size=4096

# Core build targets
$K/kernel: $(KOBJS) $K/kernel.ld $U/initcode
	@echo "Building kernel..."
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $K/kernel $(KOBJS)
	$(OBJDUMP) -S $K/kernel > $(ASM_DIR)/kernel.asm
	$(OBJDUMP) -t $K/kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(SYM_DIR)/kernel.sym

# Kernel object compilation
$(KERNEL_OBJ_DIR)/%.o: $K/%.c
	$(CC) $(RISCV_CFLAGS) -c -o $@ $<

$(KERNEL_OBJ_DIR)/%.o: $K/%.S
	$(CC) $(RISCV_CFLAGS) -c -o $@ $<

# Initcode compilation
$U/initcode: $U/initcode.S
	$(CC) $(RISCV_CFLAGS) -march=rv64g -nostdinc -I. -Ikernel -c $U/initcode.S -o $(USER_OBJ_DIR)/initcode.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o $(USER_OBJ_DIR)/initcode.out $(USER_OBJ_DIR)/initcode.o
	$(OBJCOPY) -S -O binary $(USER_OBJ_DIR)/initcode.out $U/initcode
	$(OBJDUMP) -S $(USER_OBJ_DIR)/initcode.o > $(ASM_DIR)/initcode.asm

# User library objects
ULIB = $(addprefix $(USER_OBJ_DIR)/, ulib.o usys.o printf.o umalloc.o)

# User programs compilation
$(USER_OBJ_DIR)/%: $(USER_OBJ_DIR)/%.o $(ULIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ $^
	$(OBJDUMP) -S $@ > $(ASM_DIR)/$*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(SYM_DIR)/$*.sym

# User object compilation
$(USER_OBJ_DIR)/%.o: $U/%.c
	$(CC) $(RISCV_CFLAGS) -c -o $@ $<

$(USER_OBJ_DIR)/%.o: $U/%.S
	$(CC) $(RISCV_CFLAGS) -c -o $@ $<

# System call generation
$U/usys.S: $U/usys.pl
	perl $U/usys.pl > $U/usys.S

# mkfs compilation
mkfs/mkfs: mkfs/mkfs.c $K/fs.h $K/param.h
	$(HOST_CC) $(HOST_CFLAGS) -o $@ $<

# User programs list
UPROGS=\
	$(USER_OBJ_DIR)/cat\
	$(USER_OBJ_DIR)/echo\
	$(USER_OBJ_DIR)/forktest\
	$(USER_OBJ_DIR)/grep\
	$(USER_OBJ_DIR)/init\
	$(USER_OBJ_DIR)/kill\
	$(USER_OBJ_DIR)/ln\
	$(USER_OBJ_DIR)/ls\
	$(USER_OBJ_DIR)/mkdir\
	$(USER_OBJ_DIR)/rm\
	$(USER_OBJ_DIR)/sh\
	$(USER_OBJ_DIR)/stressfs\
	$(USER_OBJ_DIR)/usertests\
	$(USER_OBJ_DIR)/grind\
	$(USER_OBJ_DIR)/wc\
	$(USER_OBJ_DIR)/zombie\
	$(USER_OBJ_DIR)/salaam\
	$(USER_OBJ_DIR)/retint\
	$(USER_OBJ_DIR)/stat\
	$(USER_OBJ_DIR)/dir\
	$(USER_OBJ_DIR)/test\
	$(USER_OBJ_DIR)/testyear\
	$(USER_OBJ_DIR)/getppid\
	$(USER_OBJ_DIR)/killpid\
	$(USER_OBJ_DIR)/msgqueue_test\
	$(USER_OBJ_DIR)/user_writershm\
	$(USER_OBJ_DIR)/user_readershm\
	$(USER_OBJ_DIR)/test1\
	$(USER_OBJ_DIR)/test_readcount\
	$(USER_OBJ_DIR)/sem_test\

# Filesystem image creation
fs.img: mkfs/mkfs $(UPROGS)
	@echo "Creating filesystem image..."
	mkdir -p .tmp
	cp $(UPROGS) .tmp/
	cd .tmp && ../mkfs/mkfs ../fs.img *
	rm -rf .tmp

# Clean rule
clean: 
	rm -rf $(BUILD_DIR) .tmp
	rm -f *.tex *.dvi *.idx *.aux *.log *.ind *.ilg \
	$U/initcode $U/initcode.out $K/kernel fs.img \
	mkfs/mkfs .gdbinit \
	$U/usys.S $(UPROGS)

# QEMU settings
CPUS ?= 3
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

QEMUOPTS = -machine virt -bios none -kernel $K/kernel -m 128M -smp $(CPUS) -nographic
QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0
QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

# QEMU targets with error checking
qemu: $K/kernel fs.img
	@if [ ! -f $K/kernel ]; then \
		echo "Error: kernel file not found"; \
		exit 1; \
	fi
	@if [ ! -f fs.img ]; then \
		echo "Error: fs.img not found"; \
		exit 1; \
	fi
	$(QEMU) $(QEMUOPTS)

.gdbinit: .gdbinit.tmpl-riscv
	sed "s/:1234/:$(GDBPORT)/" < $^ > $@

qemu-gdb: $K/kernel .gdbinit fs.img
	@if [ ! -f $K/kernel ]; then \
		echo "Error: kernel file not found"; \
		exit 1; \
	fi
	@if [ ! -f fs.img ]; then \
		echo "Error: fs.img not found"; \
		exit 1; \
	fi
	@echo "*** Now run 'gdb' in another window." 1>&2
	$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)

# Include dependencies
-include $(KERNEL_OBJ_DIR)/*.d $(USER_OBJ_DIR)/*.d