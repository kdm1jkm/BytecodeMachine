# Stack Machine

---
## How arguments work

    [value1] [value2] [value3]...

    ...
    value3 = stack.pop()
    value2 = stack.pop()
    value1 = stack.pop()

## Other details

variable: 32bit integer only

-----

## Operator

### equal

    0x00

    [value1] [value2]

if `[value1] == [value2]`, push `0xFFFFFFFF`  
else push `0x00000000`


### negative

    0x01

    [value]

push `![value]`

### if larger 

    0x02

    [value1] [value2]

if `[value1] > [value2]`, push `0xFFFFFFFF`  
else push `0x00000000`


### if smaller

    0x03

    [value1] [value2]

if `[value1] < [value2]`, push `0xFFFFFFFF`  
else push `0x00000000`

### goto

    0x04

    [byte]

goto `[byte]`of the file


### if-goto

    0x05

    [byte] [value]

if `[value] == 0xFFFFFFFF`, goto `[byte]` of the file

### push-value `[value]`

    0x06

push `[value]` to the stack.

### push-var `[varIndex]`



push `[value]` to the stack

### pop `[variable]/[]`

    0x07

    [value]

pop `[value]` and save result to `[variable]` if exists

### add

    0x08

    [value1] [value2]

push `[value1] + [value2]`

### subtract

    0x09

    [value1] [value2]

push `[value1] - [value2]`

### multiply

    0x0A

    [value1] [value2]

push `[value1] * [value2]`

### divide

    0x0B

    [value1] [value2]

push `[value1] / [value2]`

### print

    0x0C

    [character]

print `[character]`