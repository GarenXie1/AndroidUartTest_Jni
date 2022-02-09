import java.lang.String;

public class UartTest {

    static {
        // 静态代码块, 加载 .so
        System.loadLibrary("UartTest");
    }

    public static native void sayHello();
    public static native int openUart(String path, int baudrate);
    public static native void closeUart(int fd);

    public static void main(String[] args){
        if(args.length != 2) {
            return;
        }
        System.out.println("args[0] -> " + args[0] + "; args[1] -> " + Integer.parseInt(args[1]));

        int ttyFd = -1;
        sayHello();
        ttyFd = openUart(args[0],Integer.parseInt(args[1]));
        closeUart(ttyFd);
    }
}
