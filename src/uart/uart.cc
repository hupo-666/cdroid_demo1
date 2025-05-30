﻿
#include "uart.h"

/*******************************************************************
*名称：             UART0_Open
*功能：             打开串口并返回串口设备文件描述
*入口参数：         fd      文件描述符
                    port    串口号(ttyS0,ttyS1,ttyS2)
*出口参数：正确返回为1，错误返回为0
*******************************************************************/
int UART0_Open(const char *port)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK); // 非阻塞 或 fcntl(fd, F_SETFL, FNDELAY) //设为非阻塞，道理同上
    if (fd < 0)
    {
        perror("Can't Open Serial Port");
        return (FALSE);
    }    
    printf("fd->open=%d\n", fd);
    return fd;
}
/*******************************************************************
*名称：             UART0_Close
*功能：             关闭串口并返回串口设备文件描述
*入口参数：         fd          文件描述符
                    port        串口号(ttyS0,ttyS1,ttyS2)
*出口参数：void
*******************************************************************/

void UART0_Close(int fd)
{
    close(fd);
}

/*******************************************************************
 *名称：             UART0_Set
 *功能：             设置串口数据位，停止位和效验位
 *入口参数：         fd          串口文件描述符
 *                   speed       串口速度
 *                   flow_ctrl   数据流控制
 *                   databits    数据位   取值为 7 或者8
 *                   stopbits    停止位   取值为 1 或者2
 *                   parity      效验类型 取值为N,E,O,,S
 *出口参数：正确返回为1，错误返回为0
 *******************************************************************/
int UART0_Set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{

    int i;
    int status;
    int speed_arr[] = {B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[]  = {115200, 19200, 9600, 4800, 2400, 1200, 300};

    struct termios options;

    /*  tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，
        该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.  */
    if (tcgetattr(fd, &options) != 0)
    {
        perror("SetupSerial 1");
        return (FALSE);
    }

    /* 修改控制模式，保证程序不会占用串口 */
    options.c_cflag |= CLOCAL;

    /* 启动接收器，能够从串口中读取输入数据 */
    options.c_cflag |= CREAD;

    /*  CSIZE字符大小掩码，将与设置databits相关的标致位置零 */
    options.c_cflag &= ~CSIZE;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /*
     * ICANON: 标准模式
     * ECHO: 回显所输入的字符
     * ECHOE: 如果同时设置了ICANON标志，ERASE字符删除前一个所输入的字符，WERASE删除前一个输入的单词
     * ISIG: 当接收到INTR/QUIT/SUSP/DSUSP字符，生成一个相应的信号
     */

    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /*
     * BRKINT: BREAK将会丢弃输入和输出队列中的数据(flush)，并且如果终端为前台进程组的控制终端，则BREAK将会产生一个
     SIGINT信号发送到这个前台进程组
     * ICRNL: 将输入中的CR转换为NL
     * INPCK: 允许奇偶校验
     * ISTRIP: 剥离第8个bits
     * IXON: 允许输出端的XON/XOF流控
     */
    /* OPOST: 表示处理后输出，按照原始数据输出 */
    options.c_oflag &= ~(OPOST);

    //设置串口输入波特率和输出波特率
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if (speed == name_arr[i])
        {
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
        }
    }

    //设置数据流控制
    switch (flow_ctrl)
    {

    case 0:   //不使用流控制
        options.c_cflag &= ~CRTSCTS;
        break;

    case 1:   //使用硬件流控制
        options.c_cflag |= CRTSCTS;
        break;
    case 2:   //使用软件流控制
        options.c_cflag |= IXON | IXOFF | IXANY;
        break;
    }
    //设置数据位
    //屏蔽其他标志位
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
    case 5: options.c_cflag |= CS5; break;
    case 6: options.c_cflag |= CS6; break;
    case 7: options.c_cflag |= CS7; break;
    case 8: options.c_cflag |= CS8; break;
    default: fprintf(stderr, "Unsupported data size\n"); return (FALSE);
    }
    //设置校验位
    switch (parity)
    {
    case 'n':
    case 'N':   //无奇偶校验位。
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O':   //设置为奇校验
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E':   //设置为偶校验
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 's':
    case 'S':   //设置为空格
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default: fprintf(stderr, "Unsupported parity\n"); return (FALSE);
    }
    // 设置停止位
    switch (stopbits)
    {
    case 1: options.c_cflag &= ~CSTOPB; break;
    case 2: options.c_cflag |= CSTOPB; break;
    default: fprintf(stderr, "Unsupported stop bits\n"); return (FALSE);
    }

    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN]  = 1; /* 读取字符的最少个数为1 */

    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    tcflush(fd, TCIFLUSH);

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        perror("com set error!\n");
        return (FALSE);
    }
    return (TRUE);
}
/*******************************************************************
 *名称：                UART0_Init()
 *功能：                串口初始化
 *入口参数：            fd         文件描述符
 *                      speed      串口速度
 *                      flow_ctrl  数据流控制
 *                      databits   数据位   取值为 7 或者8
 *                      stopbits   停止位   取值为 1 或者2
 *                      parity     效验类型 取值为N,E,O,,S
 *
 *出口参数：正确返回为1，错误返回为0
 *******************************************************************/
int UART0_Init(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
    int err;
    //设置串口数据帧格式
    if (UART0_Set(fd, speed, flow_ctrl, databits, stopbits, parity) == FALSE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*******************************************************************
 * 名称：            UART0_Recv
 * 功能：            接收串口数据
 * 入口参数：        fd         文件描述符
 *                   rcv_buf    接收串口中数据存入rcv_buf缓冲区中
 *                   data_len   一帧数据的长度
 * 出口参数：        正确返回为1，错误返回为0
 *******************************************************************/
int UART0_Recv(int fd, unsigned char *rcv_buf, int data_len)
{
    int    len, fs_sel;
    fd_set fs_read;

    struct timeval time;

    FD_ZERO(&fs_read);
    FD_SET(fd, &fs_read);

    time.tv_sec  = 0;
    time.tv_usec = 10000;

    //使用select实现串口的多路通信
    fs_sel = select(fd + 1, &fs_read, NULL, NULL, &time);
    if (fs_sel)
    {
        len = read(fd, rcv_buf, data_len);
        return len;
    }
    else
    {
        return FALSE;
    }
}
/********************************************************************
 * 名称：            UART0_Send
 * 功能：            发送数据
 * 入口参数：        fd           文件描述符
 *                   send_buf     存放串口发送数据
 *                   data_len     一帧数据的个数
 * 出口参数：        正确返回为1，错误返回为0
 *******************************************************************/
int UART0_Send(int fd, unsigned char *send_buf, int data_len){
    int offset = 0;
    int len    = 0;

    while (offset < data_len) {
        len = write(fd, send_buf + offset, data_len - offset);
        if (len > 0) {
            offset += len;
        } else {
            tcflush(fd, TCOFLUSH);
            break;
        }
    }
    return offset;
}
