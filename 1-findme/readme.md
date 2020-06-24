# Find The Password, Patch it, Exploit it

## How to find the password?

**Dump will not find the password**

``` bash
# ./findme keep
OK
# ./findme anything
Wrong

strings findme | greep keep

objdump -M intel -d findme | keep

```
### Using gdb
``` bash
# Start GDB
gdb ./findme
# Run the program (run/r)
run
# Get the breakpoint
(gdb) info file
# Give us info that Entry point: 0x5555555546a0, set as breakpoint (break/b)
(gdb) b *0x5555555546a0
(gdb) b *0x55555555486c
# Run again with "AAA" as password
run AAA
# Show assembly
layout asm
# show instruction pointer
display/i $pc
# show register
layout reg
# Step into until goto in the first "call", enter key for repeat last command
si
....

|0x7ffff7a5a1f0 <__libc_start_main>      push   r14
│0x7ffff7a5a1f2 <__libc_start_main+2>    push   r13
│0x7ffff7a5a1f4 <__libc_start_main+4>    push   r12
│0x7ffff7a5a1f6 <__libc_start_main+6>    push   rbp
# Next until you found call rpb
ni
# Print stack trace
bt
#0  0x0000555555554840 in ?? ()
#1  0x00007ffff7a5a270 in __libc_start_main (main=0x555555554660, argc=2, argv=0x7fffffffdc68, init=0x555555554840, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffdc58) at ../csu/libc-start.c:247
#2  0x00005555555546ca in ?? ()

#Explore the function call until
bt
#0  0x00005555555545e0 in ?? ()
#1  0x0000555555554871 in ?? ()
#2  0x00007ffff7a5a270 in __libc_start_main (main=0x555555554660, argc=2, argv=0x7fffffffdc68, init=0x555555554840, ini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffdc58) at ../csu/libc-start.c:247
#3  0x00005555555546ca in ?? ()

```

Too long, try to set break point directly from dissably sections

Investigation the area where is checking the args and output "No password input"
```
 68a:	e8 91 ff ff ff       	call   620 <puts@plt>
```

Setting the breakpoint

``` bash
gdb findme
run
info file
# .text loaded at 0x0000555555554660, chane to point our target at '68a'
b *0x000055555555468a
run
layout asm

```

