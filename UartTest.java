import java.lang.String;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class UartTest {

    static {
        // 静态代码块, 加载 .so
        System.loadLibrary("UartTest");
    }

    public static native void sayHello();
    public static native FileDescriptor openUart(String path, int baudrate);
    public static native void closeUart(FileDescriptor fd);


	public static FileDescriptor mFd;
	public static FileInputStream mFileInputStream;
	public static FileOutputStream mFileOutputStream;

    public static void main(String[] args) throws IOException {
        if(args.length != 4) {
			System.out.println("Please input : UartTest /dev/ttyHSL1 115200 read ascii");
			System.out.println("Or           : UartTest /dev/ttyHSL1 115200 read hex");
			System.out.println("Or           : UartTest /dev/ttyHSL1 115200 write xx");
            return;
        }
        System.out.println("args[0] -> " + args[0] + "; args[1] -> " + Integer.parseInt(args[1]) +
                "; args[2] -> " + args[2] + "; args[3] -> " + args[3]);

        mFd = openUart(args[0],Integer.parseInt(args[1]));
		mFileInputStream = new FileInputStream(mFd);
		mFileOutputStream = new FileOutputStream(mFd);

		mFileOutputStream.write(args[3].getBytes());

        closeUart(mFd);
    }
}
