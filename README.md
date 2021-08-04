# Stack Machine

---

## 매개변수 작동법

    [value1] [value2] [value3]...

    ...
    value3 = stack.pop()
    value2 = stack.pop()
    value1 = stack.pop()

## 기타

변수: 32bit 정수

---

## 산술연산자(0x0\_)

### add

    0x00

    [value1] [value2]

### sub

    0x01

    [value1] [value2]

### neg

    0x02

    [value]

### eq

    0x03

    [value1] [value2]

### gt

    0x04

    [value1] [value2]

### lt

    0x05

    [value1] [value2]

### and

    0x06

    [value1] [value2]

### or

    0x07

    [value1] [value2]

### not

    0x08

    [value]

## 메모리 분기(0x1\_)

### goto

    0x10

    [value]

### if-goto

    0x11

    [bool] [value]

## 메모리(0x2\_)

### push [value]

    0x20

### pop

    0x21

    [value]

## 입출력(0x3\_)

### print

    0x30

    [value]
