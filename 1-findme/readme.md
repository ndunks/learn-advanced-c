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
```
gdb findme
```


