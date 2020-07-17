# GNU C UNION Behaviour


## OUTPUT

```
char 1, short 2, int 4, long 8, 
-------------
sizeof(number8) = 1
sizeof(number16) = 2
sizeof(number32) = 4
sizeof(number64) = 8
-------------
number8.int8 = -1 = -1
number8.int8_u = -1 = 255
number8 = 255
number8.int8 = -1
number8.int8_u = 255
-------------
number64.int8 = 256 = 0
number64.int8 = 0
number64.int16 = 0
-------------
number64.int8 = 0xfdfe = -2
number64.int8 = -2
number64.int16 = 254
number64.int32 = 254
number64 (8) : fe 00 00 00 00 00 00 00
-------------
number64.int16 = 0xffff = -1
number64.int8 = -1
number64.int16 = -1
number64.int32 = 65535
number64.int64 = 65535
number64 (8) : ff ff 00 00 00 00 00 00
-------------
*ptr_number64 = 140724603388159 ffff00ff
ptr_number64->int8_u = 255
ptr_number64->int16_u = 255
ptr_number64->int32_u = 4294902015
ptr_number64->int64_u = 280379759984895
*ptr_number64 (8) : ff 00 ff ff 00 ff 00 00
```
