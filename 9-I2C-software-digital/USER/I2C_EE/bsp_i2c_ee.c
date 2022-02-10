#include "bsp_i2c_ee.h"



uint8_t ee_CheckDevice(uint8_t _Address){
	uint8_t ucAck;
	
	//发送启动信号
	I2c_Start();
	
	//发送设备地址+读写控制位
	I2c_SendByte(_Address|EEPROM_I2C_WR);
	ucAck=i2c_WaitAck();
	
	
	I2c_Stop();	/* 发送停止信号 */
	i2c_NAck();	/*若输入的是读地址，需要产生非应答信号*/
	return ucAck;
}

/*
*	功能：等待EEPROM到准备状态，在写入数据后，必须调用本函数
*				写入操作时，使用 I2C把数据传输到EEPROM后，
*				EEPROM会向内部空间写入数据需要一定的时间，
*				当EEPEOM写入完成后，会对I2C设备寻址有响应
*				
*				调用本函数可等待至EEPROM内部时序写入完毕
*/
u8	ee_WaitStandby(void){
	u32	wait_count=0;
	
	while(ee_CheckDevice(EEPROM_DEV_ADDR))
	{
		//若检测超过次数，退出循环(避免死循环）	
		if(wait_count ++ >0xFFFF)
			//等待超时	
			return 1;
	}
	return 0;
}
/*
*	功能：向串行EEPROM指定地址写入若干数据，采用页操作可以提高写入效率
*				
*	形参：	_usAddress:起始地址		
*				_usSize:数据长度，单位为字节
*				_pWriteBuf:存放读到的数据的缓冲区指针
*/
u8	ee_WriteBytes(u8 *_pWriteBuf,u16 	_usAddress,u16 _usSize){
	u16 i,m,usAddr;
	/*
	写串行EEPROM不像读操作一样可以连续读取很多字节，每次操作都只能写在同一个page
	对应24xx02芯片，page size =8;
	简单的处理方式为：按照字节写操作模式，每写一个字节，都发送地址
	提高效率采用 page write
	
	*/
	usAddr = _usAddress;
	for(i=0;i<_usSize;i++){
		
		//当发送第一个字节或者是页面首地址时，需要重新发送启动信号和地址
		if((i==0)||(usAddr&(EEPROM_PAGE_SIZE-1))==0){
			//第0步：发送停止信号，结束上一页的通讯，准备下一次通讯
			I2c_Stop();
			/*	通过检查器件应答的方式，判断内部写操作是否完成，一般小于10ms
			CLK频率为KHz时,查询次数为30次左右
			原理与ee_WaitStandby()函数，但是该函数检查完成后会产生停止信号，
			不适用于此处
			*/
			for(m=0;m < 1000; m++){
			//第一步：发送I2C总线信号
				
				I2c_Start();
				
			//第二步：发起控制字节，高7bit是地址，bit0为读写控制位，0：写，1：读
				
				I2c_SendByte(EEPROM_DEV_ADDR|EEPROM_I2C_WR);
				
			//第三步:发送一个时钟，判断器件是否正确应答
					if(i2c_WaitAck() == 0)
							break;
				}
				if(m == 1000)
						goto cmd_fail;//EEPROM器件写超时
				
				//第四步:发送字节地址，24c02只有256个字节，因此1个字节就够了，如果是其他型号，\
				要连发多个地址
				I2c_SendByte((u8)usAddr);
				//第五步:等待ACK
				if(i2c_WaitAck() != 0)
				{
						goto cmd_fail;//EEPROM器件无应答
				}
			}
				//第六步:开始写入数据
				I2c_SendByte(_pWriteBuf[i]);
				
				//第七步:发送ACK
				if(i2c_WaitAck() != 0)
						goto cmd_fail;//EEPROM器件无应答
				
				usAddr++;		//地址增1
			}
			//命令执行成功，发送I2C总线停止信号
			I2c_Stop();
		
			//等待最后一次 EEPROM内部写入完成
				if(ee_WaitStandby() == 1)
						goto cmd_fail;			
			return 1;
	
cmd_fail:	/* 	命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备*/
		I2c_Stop();
		return 0;
}


/*
*	功能：从串行EEPROM指定地址读取若干数据
*				
*	形参：	_usAddress:起始地址		
*				_usSize:数据长度，单位为字节
*				_pReadBuf:存放读到的数据的缓冲区指针
*/
u8	ee_ReadBytes(u8 *_pReadBuf,u16	_usAddress,u16 _usSize){
	u16 i;
	
	//第一步：发起I2C总线启动信号
	I2c_Start();
	
	//第二步:发起控制字节，高七位是地址，最后一位是读写位 ，0：写，1：读
	I2c_SendByte(EEPROM_DEV_ADDR|EEPROM_I2C_WR);//写方向，写地址
	
	//第三步:等待ACK
	if(i2c_WaitAck() != 0)
			goto cmd_fail;		//EEPROM器件无应答
	
	//第四步:发送字节地址，24c02只有256个字节，因此一个字节就够了
	I2c_SendByte((u8)_usAddress);
	
	//第五步：等待ACK
	if(i2c_WaitAck() != 0)
			goto cmd_fail;		//EEPROM器件无应答
	
	//第六步:重新启动I2C总线
	//前面的目的是未来向EEPROM传送地址，下面开始读取数据
	I2c_Start();
	
	//第七步:发送控制字节
	I2c_SendByte(EEPROM_DEV_ADDR|EEPROM_I2C_RD);
	
	//第八步:发送ACK
	if(i2c_WaitAck() != 0)
			goto cmd_fail;		//EEPROM器件无应答
	
	//第九步:循环读取数据
	for(i = 0; i < _usSize; i++){
		
		_pReadBuf[i]=i2c_ReadByte();//读一个字节
		
		//每次读完一个字节后，需要发送ACK，最后一个字节不需要发送ACK,发送NACK
		if(i != _usSize -1)
				i2c_Ack();
		else
			i2c_NAck();
	}
	
	//发送停止信号
	I2c_Stop();	
	return 1;	//执行成功
cmd_fail:	//命令执行失败，切记发送停止信号，避免影响I2C总线上的其他设备
	//I2C总线停止信号
	I2c_Stop();
	return 0;
}


//EPROM读写测试	
u8 ee_test(void){
	u16 i;
	u8 writebuf[EEPROM_SIZE];
	u8 readbuf[EEPROM_SIZE];
	
	if(ee_CheckDevice(EEPROM_DEV_ADDR)==1){
		printf("没有检测到串行EEPROM\r\n");
		return 0;
	}
	//填充缓冲区
	for(i = 0;i < EEPROM_SIZE; i++)
		writebuf[i]=i;
	
	//写
	if(ee_WriteBytes(writebuf,0,EEPROM_SIZE) == 0){
			printf("写EEPROM出错\r\n");
			return 0;
	}else
	printf("写EEPROM成功\r\n");
	//读
		if(ee_ReadBytes(readbuf,0,EEPROM_SIZE) == 0){
			printf("读EEPROM出错\r\n");
			return 0;
	}else
	printf("读EEPROM成功\r\n");
	
	//输出读到的数据(不一致)
	printf("readbuf:\n");
	for(i=0;i<EEPROM_SIZE ; i++){
		if(readbuf[i] != writebuf[i]){
			printf("0x%02X ", readbuf[i]);
			printf("错误：EEPROM读出和写入的数据不一致\r\n");
			return 0;
		}
		if((i % 16)== 0&&(i != 0))
			printf("\n");
		printf(" %02X", readbuf[i]);
		
	}
	printf("\nEEPROM 读写测试成功\r\n");
	return 1;
}
