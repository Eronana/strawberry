## strawberry
strawberry is a script language

## Build
```bash
make
```

## Test
```bash
make lexer.test
make parser.test
make codegen.test
make assembler.test
make disassembler.test
make vm.test
```

## How to use
You can use `-o <filename>` for `-s`, `-x` and `-c`
```bash
# run script file or binary
bin/strawberry xxx.st
# compile script to assembly
bin/strawberry xxx.st -s
# compile script to binary
bin/strawberry xxx.st -x
# compile assembly to binary
bin/strawberry xxx.asm -c
```

## Todo
- [ ] Dosc
- [ ] Language specification
- [ ] Graphical interface(OpenGL)
