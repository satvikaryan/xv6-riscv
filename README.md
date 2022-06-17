# xv6-riscv
Modified Version of MIT's XV6 Operating System

## Installation
### Install Requirements
```
sudo apt update
sudo apt upgrade
sudo apt install gcc make build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

### Clone XV6 Repository
```
git clone https://github.com/JameelKaisar/xv6-riscv.git
```

### Change Directory
```
cd xv6-riscv
```

### Compile XV6
```
make
```

### Run XV6
```
make qemu
```

### Quit XV6
```
Crtl+a
x
```

### Recompile and Run
```
make clean && make && make qemu && make clean && clear
```

## Commands
### salaam Command
```
$ salaam
Salaam Robo! Kaifa Haluka?
```

```
$ salaam Ajmi
Salaam Ajmi! Kaifa Haluka?
```

### retint Command
```
$ retint
2022
```

### stat Command
```
$ stat grep
ID of Disk Device:  1
Type:               File
Inode Number:       5
Number of Links:    1
Size:               27320
```

```
$ stat console
ID of Disk Device:  1
Type:               Device
Inode Number:       22
Number of Links:    1
Size:               0
```

### dir Command
```
$ dir .
----------------------------------------------------------
| .                                                      |
----------------------------------------------------------
|     1 | Directory  |       1024 | .                    |
|     1 | Directory  |       1024 | ..                   |
|     2 | File       |      23976 | cat                  |
|     3 | File       |      22800 | echo                 |
|     4 | File       |      13144 | forktest             |
|     5 | File       |      27320 | grep                 |
|     6 | File       |      23888 | init                 |
|     7 | File       |      22752 | kill                 |
|     8 | File       |      22704 | ln                   |
|     9 | File       |      26200 | ls                   |
|    10 | File       |      22856 | mkdir                |
|    11 | File       |      22840 | rm                   |
|    12 | File       |      41728 | sh                   |
|    13 | File       |      23864 | stressfs             |
|    14 | File       |     156080 | usertests            |
|    15 | File       |      38032 | grind                |
|    16 | File       |      25104 | wc                   |
|    17 | File       |      22248 | zombie               |
|    18 | File       |      22600 | salaam               |
|    19 | File       |      22440 | retint               |
|    20 | File       |      23800 | stat                 |
|    21 | File       |      30232 | dir                  |
|    22 | Device     |          0 | console              |
----------------------------------------------------------
```
