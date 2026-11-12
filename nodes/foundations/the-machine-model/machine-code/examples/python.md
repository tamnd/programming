```python run id=decode-x86-64
# x86-64 instructions have different lengths. A decoder must read each one to find the next.
code = bytes.fromhex("b8 2a 00 00 00 48 01 c8 90 48 b8 88 77 66 55 44 33 22 11 c3")
REG = ["ax", "cx", "dx", "bx", "sp", "bp", "si", "di"]

pc = 0
while pc < len(code):
    start = pc
    rex_w = code[pc] == 0x48  # REX.W prefix: the operation uses 64 bits
    if rex_w:
        pc += 1
    op = code[pc]
    pc += 1
    size = "r" if rex_w else "e"
    if 0xB8 <= op <= 0xBF:  # mov register, immediate
        n = 8 if rex_w else 4
        imm = int.from_bytes(code[pc:pc + n], "little")
        pc += n
        text = f"mov {size}{REG[op - 0xB8]}, {imm:#x}"
    elif op == 0x01:  # add r/m, register: a ModRM byte names the two registers
        modrm = code[pc]
        pc += 1
        text = f"add {size}{REG[modrm & 7]}, {size}{REG[modrm >> 3 & 7]}"
    elif op == 0x90:
        text = "nop"
    elif op == 0xC3:
        text = "ret"
    else:
        raise ValueError(f"unknown opcode {op:#x}")
    n = pc - start
    print(f"{code[start:pc].hex(' '):30} {n:2} byte{'s' if n > 1 else ' '}  {text}")
```

```output for=decode-x86-64
b8 2a 00 00 00                  5 bytes  mov eax, 0x2a
48 01 c8                        3 bytes  add rax, rcx
90                              1 byte   nop
48 b8 88 77 66 55 44 33 22 11  10 bytes  mov rax, 0x1122334455667788
c3                              1 byte   ret
```

- Verdict: absent. CPython compiles source to its own bytecode and interprets it, so a Python program does not see machine code. The example treats machine code as data: it decodes five x86-64 instructions of 1, 3, 5 and 10 bytes. The `48` prefix makes an operation 64 bits wide, and the `mov` of a 64-bit constant carries all 8 bytes of it, low byte first. The decoder must read each instruction to know where the next one starts.
