
/*

int fd = -1;   //文件描述符，先定义一个与程序无关的值，防止fd为任意值导致程序出bug
int err;   //返回调用函数的状态

fd = UART0_Open("/dev/ttyS1");
if (fd <= 0)
{
    perror("open uart fail");
    return -1;
}

do {
    err = UART0_Init(fd, 115200, 0, 8, 1, 'N');
    printf("Set Port Exactly! err=%d\n", err);
    sleep(1);
} while (FALSE == err);

unsigned char mcu_snd_buf[27] = {0};
unsigned char mcu_rcv_buf[25] = {0};
unsigned char *spi_snd_buf = &mcu_snd_buf[6];
unsigned char *spi_rcv_buf = &mcu_rcv_buf[3];

mcu_snd_buf[0] = 0xA5;
mcu_snd_buf[1] = 24;

spi_snd_buf[0] = 0x5a;
spi_snd_buf[1] = 0x90;
spi_snd_buf[2] = 42;

for (int i = 3; i <= 12; i++)
{
    spi_snd_buf[i] = 1;
}

ushort crc_val = Fuc_GetCRC(0xffff, spi_snd_buf, 18);
spi_snd_buf[18] = crc_val >> 8;
spi_snd_buf[19] = crc_val & 0xff;

unsigned char check_sum = 0;
for (int i = 0; i < sizeof(mcu_snd_buf) - 1; i++)
{
    check_sum += mcu_snd_buf[i];
}
mcu_snd_buf[26] = (~check_sum) + 1; // Checksum

hexdump("send data", mcu_snd_buf, sizeof(mcu_snd_buf));
UART0_Send(fd, (char *)mcu_snd_buf, sizeof(mcu_snd_buf));

do {
    err = UART0_Recv(fd, (char *)mcu_rcv_buf, sizeof(mcu_rcv_buf));
    hexdump("recv data", mcu_rcv_buf, sizeof(mcu_rcv_buf));
    printf("recv! ret=%d\n", err);
    sleep(1);
}while (err == FALSE);

*/
