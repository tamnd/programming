```java run id=classfile-api-bytecode
import java.lang.classfile.ClassFile;
import java.lang.classfile.CodeElement;
import java.lang.classfile.Instruction;
import java.lang.constant.ClassDesc;
import java.lang.constant.ConstantDescs;
import java.lang.constant.MethodTypeDesc;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.lang.reflect.AccessFlag;

public class Main {
    public static void main(String[] args) throws Throwable {
        // Write the bytecode of "static int add(int a, int b) { return a + b; }" by hand.
        MethodTypeDesc type = MethodTypeDesc.of(ConstantDescs.CD_int, ConstantDescs.CD_int, ConstantDescs.CD_int);
        byte[] cls = ClassFile.of().build(ClassDesc.of("Main$Adder"), cb -> cb
            .withMethodBody("add", type, ClassFile.ACC_PUBLIC | ClassFile.ACC_STATIC, code -> code
                .iload(0)
                .iload(1)
                .iadd()
                .ireturn()));
        System.out.println("class file: " + cls.length + " bytes");

        // Read the instructions back from the class file.
        var model = ClassFile.of().parse(cls);
        for (var m : model.methods()) {
            if (!m.flags().has(AccessFlag.STATIC)) continue;
            for (CodeElement e : m.code().orElseThrow()) {
                if (e instanceof Instruction i) {
                    System.out.println("  " + i.opcode().name().toLowerCase() + " (" + i.sizeInBytes() + " byte)");
                }
            }
        }

        // The JVM loads the class, checks the bytecode and runs it.
        var lookup = MethodHandles.lookup().defineHiddenClass(cls, true);
        var add = lookup.findStatic(lookup.lookupClass(), "add", MethodType.methodType(int.class, int.class, int.class));
        System.out.println("add(40, 2) = " + (int) add.invokeExact(40, 2));
    }
}
```

```output for=classfile-api-bytecode
class file: 113 bytes
  iload_0 (1 byte)
  iload_1 (1 byte)
  iadd (1 byte)
  ireturn (1 byte)
add(40, 2) = 42
```

- Verdict: absent. A Java program runs as JVM bytecode, and the JIT compiler makes the machine code out of sight. The class file API writes a method of four one-byte instructions that push the two arguments, add them and return the result. The program reads the instructions back from the class file and loads it as a hidden class; the JVM verifies the bytecode before it runs it.
