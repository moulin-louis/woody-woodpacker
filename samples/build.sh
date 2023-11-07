gcc -nostdlib no-std-c.c -o no-std
gcc tiny_c.c -o tiny_c
nasm -f elf64 hello-pie.asm && ld -I /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2  hello-pie.o -pie -o hello-pie && rm -rf hello-pie.o
nasm -f elf64 hello.asm && ld hello.o -o hello && rm -rf hello.o
nasm -f elf64 nodata.asm && ld nodata.o -o nodata && rm -rf nodata.o