public class UartTest {

    static {
        // 静态代码块, 加载 .so
        System.loadLibrary("UartTest");
    }

    public static native void sayHello();
    public static native int openUart(String path, int baudrate);
    public static native void closeUart(int fd);

    public static void main(String[] args){

		int ttyFd = -1;
        sayHello();
        ttyFd = openUart("/dev/ttyHSL1",115200);
        closeUart(ttyFd);
    }
}
