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

## 명령어

| 산술 | `ADD`  | `SUB`  | `NEG`  |  `EQ`  |  `GT`  |  `LT`  | `AND`  |  `OR`  |
| ---- | :----: | :----: | :----: | :----: | :----: | :----: | :----: | :----: |
|      | `0x00` | `0x01` | `0x02` | `0x03` | `0x04` | `0x05` | `0x06` | `0x07` |

| 분기 |   `jump address(4byte)`    |  `if-jump address(4byte)`  |
| ---- | :------------------------: | :------------------------: |
|      | `0x10 0x__ 0x__ 0x__ 0x__` | `0x20 0x__ 0x__ 0x__ 0x__` |

| 메모리 | `push segment(1) index(1, 4)` | `pop segment(1) index(1, 4)` |
| ------ | :---------------------------- | :--------------------------- |
|        | `0x20`                        | `0x21`                       |

| segment | `constant` | `argument` | `local` | `in(push only)` | `out(pop only)` |
| ------- | :--------: | :--------: | :-----: | :-------------: | :-------------: |
| code    |   `0x30`   |   `0x31`   | `0x32`  |     `0x33`      |     `0x34`      |
| index   |   4byte    |   1byte    |  1byte  |      1byte      |      1byte      |

## 파일 작성법

파일의 처음 두 바이트는 각각 local배열의 길이, argument의 갯수를 나타냄.  
-> jump명령어 작성시 고려하지 않음!
