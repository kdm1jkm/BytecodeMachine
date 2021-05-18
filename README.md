# Stack Machine

---
##How arguments work

    [value1] [value2] [value3]...

    value1 = stack.pop()
    value2 = stack.pop()
    value3 = stack.pop()

-----

## Operator

### if equal

    [value1] [value2]

if `[value1] == [value2]`, push `01`  
else push `00`


### if not equal

    [value1] [value2]

if `[value1] != [value2]`, push `01`   
else push `00`

### if larger 

    [value1] [value2]

if `[value1] > [value2]`, push `01`  
else push `00`


### if smaller

    [value1] [value2]

if `[value1] < [value2]`, push `01`  
else push `00`

### goto

    [byte]

goto `[byte]`of the file


### if-goto

    [byte] [value]

if `[value] != 00`, goto `[byte]` of the file

### push

    [value]

push `[value]`

### pop

    [value]

pop `[value]`

### add

    [value1] [value2]

push `[value1] + [value2]`

### subtract

    [value1] [value2]

push `[value1] - [value2]`

### multiply

    [value1] [value2]

push `[value1] * [value2]`

### divide

    [value1] [value2]

push `[value1] / [value2]`

### print

    [character]

print `[character]`
