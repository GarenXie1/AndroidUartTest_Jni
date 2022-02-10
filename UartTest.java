import java.lang.String;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

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
	public static byte[] mInBuffer;
	public static int len=-1, i=-1;

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
		if(args[2].equals("write")){

			System.out.print("Write Uart data:" +args[3] + "\n");
			mFileOutputStream.write(args[3].getBytes());
		}else if(args[2].equals("read")){

			mInBuffer = new byte[1024];
			Arrays.fill(mInBuffer, (byte) 0);

			len = mFileInputStream.read(mInBuffer);

			if(args[3].equals("ascii")){

				System.out.print("data ascii ->" + new String(mInBuffer,0,len) + "\n");
			}else if(args[3].equals("hex")){

				System.out.print("data Hex :");
				for(i = 0 ; i < len ; i ++){
					System.out.printf("0x%x ",mInBuffer[i]);
				}
				System.out.print("\n");
			}
		}
        closeUart(mFd);
    }
}
