
#include <jni.h>
#include <stdio.h>
#include "UartTest.h"

#include <termios.h>
#include <fcntl.h>
#include <errno.h>



void jni_c_sayHello(JNIEnv *env, jobject cls)
{
	printf("%s ---> Uart test ...\n",__func__);
}


static const JNINativeMethod methods[] = {
	{"sayHello", "()V", (void *)jni_c_sayHello}, /* Java native sayHello <--> C ,c_hello */
};


static void dump_data(unsigned char * b, int count)
{
	printf("%i bytes: ", count);
	int i;
	for (i=0; i < count; i++) {
		printf("%02x ", b[i]);
	}

	printf("\n");
}

static void dump_data_ascii(unsigned char * b, int count)
{
	int i;
	printf("%i bytes: ", count);
	for (i=0; i < count; i++) {
		printf("%c", b[i]);
	}
	printf("\n");
}


static speed_t getBaudrate(jint baudrate)
{
	switch(baudrate) {
	case 0: return B0;
	case 50: return B50;
	case 75: return B75;
	case 110: return B110;
	case 134: return B134;
	case 150: return B150;
	case 200: return B200;
	case 300: return B300;
	case 600: return B600;
	case 1200: return B1200;
	case 1800: return B1800;
	case 2400: return B2400;
	case 4800: return B4800;
	case 9600: return B9600;
	case 19200: return B19200;
	case 38400: return B38400;
	case 57600: return B57600;
	case 115200: return B115200;
	case 230400: return B230400;
	case 460800: return B460800;
	case 500000: return B500000;
	case 576000: return B576000;
	case 921600: return B921600;
	case 1000000: return B1000000;
	case 1152000: return B1152000;
	case 1500000: return B1500000;
	case 2000000: return B2000000;
	case 2500000: return B2500000;
	case 3000000: return B3000000;
	case 3500000: return B3500000;
	case 4000000: return B4000000;
	default: return -1;
	}
}



/* System.loadLibrary */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	JNIEnv *env;
	jclass cls;
	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
		return JNI_ERR; /* JNI version not supported */
	}
	cls = (*env)->FindClass(env, "UartTest"); // 寻找 HelloJNI Java 类
	if (cls == NULL) {
		return JNI_ERR;
	}
	/* 2. map java hello <-->c c_hello */
	if ((*env)->RegisterNatives(env, cls, methods, (sizeof(methods)/sizeof(methods[0]))) < 0)
		return JNI_ERR;
	return JNI_VERSION_1_4;
}


/*
 * Class:     UartTest
 * Method:    openUart
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT jobject JNICALL Java_UartTest_openUart(JNIEnv *env, jclass cls, jstring path, jint baudrate) {

	int fd = -1;
	struct termios cfg;
	speed_t baudSpeed;
	unsigned char rb[10] = {0};
	int count = 0;
	jobject mobjFileDescriptor;

	/* Opening device */
	const char *ttyPath = (*env)->GetStringUTFChars(env, path, NULL);
	fd = open(ttyPath, O_RDWR);	// 以阻塞方式读写,O_NONBLOCK
	if (fd == -1)
	{
		printf("Cannot open %s\n",ttyPath);
		(*env)->ReleaseStringUTFChars(env, path, ttyPath);
		/* TODO: throw an exception */
		return NULL;
	}


	/* Configure device */
	baudSpeed = getBaudrate(baudrate);
	
	memset(&cfg, 0, sizeof(struct termios));
	/* 	CREAD -> 使能接收 
	*	B115200 -> 设置波特率
	*	CS8 -> 数据位大小为 8bit
	*	CLOCAL -> 本地模式，不改变端口的所有者
	*	cfg.c_cflag &= ~CSTOPB -> 默认配置为 1 个停止位
	*	奇偶检验位 --> (cfg.c_cflag &= ~PARENB;cfg.c_iflag &= ~INPCK;)
	**/
	cfg.c_cflag = baudSpeed | CS8 | CREAD | CLOCAL;
	cfg.c_cflag &= ~CSTOPB;
	cfg.c_cflag &= ~PARENB;
	cfg.c_iflag &= ~INPCK;

	
	cfg.c_cc[VTIME] = 500;	// 5 seconds read timeout

	/* 清空缓冲区 */
	tcflush(fd, TCIOFLUSH);
	/* 写入配置、使配置生效 */
	if (tcsetattr(fd, TCSANOW, &cfg) < 0)
	{
		printf("Err, Unable to set comm port\n");
		return NULL;
	}


	// 测试读写操作
	/*
	count = read(fd, &rb, sizeof(rb));
	if (count > 0){
		dump_data_ascii(&rb,count);
	}else{
	}

	count = write(fd, &rb, count);
	if (count < 0) {
		printf("write failed - errno=%d (%s)\n", errno, strerror(errno));
	}
	*/

	printf("%s,ttyPath -> %s ,fd -> %d \n", __func__, ttyPath,fd);
	(*env)->ReleaseStringUTFChars(env, path, ttyPath);


	/* Create a corresponding file descriptor */
	jclass clsFileDescriptor = (*env)->FindClass(env, "java/io/FileDescriptor");
	if(clsFileDescriptor == NULL){
		printf("%s, can't find FileDescriptor Class.\n", __func__);
	}else{
		printf("%s, find FileDescriptor Class done.\n", __func__);
	}

	jmethodID idFileDescriptor = (*env)->GetMethodID(env, clsFileDescriptor, "<init>", "()V");
	if(idFileDescriptor == NULL){
		printf("%s, can't get <init> constructor method.\n", __func__);
	}else{
		printf("%s, get <init> constructor method done.\n", __func__);
	}

	mobjFileDescriptor = (*env)->NewObject(env, clsFileDescriptor, idFileDescriptor);
	if(mobjFileDescriptor == NULL){
		printf("%s, can not create FileDescriptor object.\n", __func__);
	}else{
		printf("%s, create FileDescriptor object done.\n", __func__);
	}

	jfieldID descriptorID = (*env)->GetFieldID(env, clsFileDescriptor, "descriptor", "I");
	if(descriptorID == NULL){
		printf("%s, can not get field descriptor ID.\n", __func__);
	}else{
		printf("%s, get field descriptor ID done.\n", __func__);
	}
	printf("%s, old descriptor -> %d \n", __func__, (*env)->GetIntField(env, mobjFileDescriptor, descriptorID));

	(*env)->SetIntField(env, mobjFileDescriptor, descriptorID, (jint)fd);
	printf("%s, new descriptor -> %d \n", __func__, (*env)->GetIntField(env, mobjFileDescriptor, descriptorID));

	return mobjFileDescriptor;
}

/*
 * Class:     UartTest
 * Method:    closeUart
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_UartTest_closeUart(JNIEnv *env, jclass cls,jobject objFileDescriptor) {

	int fd;

	jclass clsFileDescriptor = (*env)->FindClass(env, "java/io/FileDescriptor");
	if(clsFileDescriptor == NULL){
		printf("%s, can't find FileDescriptor Class.\n", __func__);
	}else{
		printf("%s, find FileDescriptor Class done.\n", __func__);
	}
	
	jfieldID descriptorID = (*env)->GetFieldID(env, clsFileDescriptor, "descriptor", "I");
	if(descriptorID == NULL){
		printf("%s, can not get field descriptor ID.\n", __func__);
	}else{
		printf("%s, get field descriptor ID done.\n", __func__);
	}

	fd = (*env)->GetIntField(env, objFileDescriptor, descriptorID);
	printf("%s, get descriptor fd -> %d \n", __func__, fd);
	close(fd);
	printf("%s, close tty UART fd -> %d \n", __func__, fd);
}

