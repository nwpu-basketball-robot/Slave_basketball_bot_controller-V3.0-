#include "data.h"


/********************************************

    �˲��ִ����NRF24L01���г�ʼ��
		�����յ�������˵�����£�
    �����յ���Ϊ*λ���ַ���  eg:ffxxyywwff
    ����ǰ�����λ��ffΪ�涨�����ݸ�ʽ
    xx��ʾx������ٶȣ���λΪm/s,��һ��x��ʾ����λ���ڶ���x��ʾС��λ
    yy��ʾy������ٶȣ���λΪm/s,��һ��y��ʾ����λ���ڶ���y��ʾС��λ
    ww��ʾy������ٶȣ���λΪrad/s,��һ��w��ʾ����λ���ڶ���w��ʾС��λ

********************************************/



u8 tmp_buf[13];	                                                                      //������������ڴ�����ߵ�����
int t;                                                                                //�޻���˵
extern robot basketballbot; 
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};                         //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};                         //���յ�ַ



void NRF24L01_SPI_Init(void)
{
	
 	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_Cmd(SPI1, DISABLE);               																							//ʧ��SPI����
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;                  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                                    //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                                //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                                      //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                                      //����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                                        //NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		            //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                                          //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);                                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE);  
  

	
}
 
//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);                               //ʹ��GPIOGʱ��

	//GPIOG6,7�������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                       //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                      //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                                  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                        //����
  GPIO_Init(GPIOG, &GPIO_InitStructure);                                              //��ʼ��GPIOG6,GPIOG7
	
	//GPIOG.8��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                                        //����ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                        //����
  GPIO_Init(GPIOG, &GPIO_InitStructure);                                              //��ʼ��GPIOG8
	
  
 	SPI1_Init();    		                                                                //��ʼ��SPI2 
	
	NRF24L01_SPI_Init();                                                                //���NRF���ص��޸�SPI������

	NRF24L01_CE=0; 			                                                                //ʹ��24L01
	NRF24L01_CSN=1;			                                                                //SPIƬѡȡ��	 		 	 



/**************NRF24L01��⺯�������䲻��������LED0����*****************/

	while(NRF24L01_Check())
	{
		LED1=!LED1;
		delay_ms(50);
	}
	

	
	NRF24L01_RX_Mode();		                                                              //NRF24L01��ʼ��Ϊ����ģʽ
	
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);                                             //spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);                                    //д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5);                                                   //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;                                                                   //���24L01����	
	return 0;		                                                                        //��⵽24L01
}	 	 



//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
  NRF24L01_CSN=0;                                                                     //ʹ��SPI����
  status =SPI1_ReadWriteByte(reg);                                                    //���ͼĴ����� 
  SPI1_ReadWriteByte(value);                                                          //д��Ĵ�����ֵ
  NRF24L01_CSN=1;                                                                     //��ֹSPI����	   
  return(status);       			                                                        //����״ֵ̬
}



//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
  NRF24L01_CSN = 0;                                                                   //ʹ��SPI����		
  SPI1_ReadWriteByte(reg);                                                            //���ͼĴ�����
  reg_val=SPI1_ReadWriteByte(0XFF);                                                   //��ȡ�Ĵ�������
  NRF24L01_CSN = 1;                                                                   //��ֹSPI����		    
  return(reg_val);                                                                    //����״ֵ̬
}	



//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
  u8 status,u8_ctr;	       
  NRF24L01_CSN = 0;                                                                    //ʹ��SPI����
  status=SPI1_ReadWriteByte(reg);                                                      //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
  for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);              //��������
  NRF24L01_CSN=1;                                                                      //�ر�SPI����
  return status;                                                                       //���ض�����״ֵ̬
}



//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
  u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;                                                                    //ʹ��SPI����
  status = SPI1_ReadWriteByte(reg);                                                    //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++);                      //д������	 
  NRF24L01_CSN = 1;                                                                    //�ر�SPI����
  return status;                                                                       //���ض�����״ֵ̬
}				   


//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);                                              //spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_CE=0;
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);                                //д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE=1;                                                                       //��������	   
	while(NRF24L01_IRQ!=0);                                                              //�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);                                                       //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);                                        //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)                                                                       //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);                                                 //���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)                                                                        //�������
	{
		return TX_OK;
	}
	return 0xff;                                                                         //����ԭ����ʧ��
}




//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);                                              //spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);                                                       //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);                                        //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)                                                                        //���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);                               //��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);                                                 //���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;                                                                            //û�յ��κ�����
}					    




//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ���,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
  NRF24L01_CE=0;	  
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);           //дRX�ڵ��ַ
	  
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);                                        //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);                                    //ʹ��ͨ��0�Ľ��յ�ַ  	 
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	                                         //����RFͨ��Ƶ��		  
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);                           //ѡ��ͨ��0����Ч���ݿ��� 	    
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);                                     //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);                                      //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}		



//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ���,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE=0;	    
  NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);              //дTX�ڵ��ַ 
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);           //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);                                        //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);                                    //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);                                   //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);                                          //����RFͨ��Ϊ40
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);                                     //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);                                       //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE=1;                                                                       //CEΪ��,10us����������
}

void NRF24L01_RX()
{ 
	   int t;
     if(NRF24L01_RxPacket(tmp_buf)==0)                                                 //�ж��Ƿ���յ���Ϣ
			{
				for(t=0;t<13;t++)
				//    printf("%c",tmp_buf[t]);
			 //  printf("\r\n");
					basketballbot.robot_expected_speed.x=(tmp_buf[3]-48)+(tmp_buf[4]-48)*0.1;    //�������������Ŀ��x�ٶ�
				  if(tmp_buf[2]=='1') basketballbot.robot_expected_speed.x=-basketballbot.robot_expected_speed.x;	
				  basketballbot.robot_expected_speed.y=(tmp_buf[6]-48)+(tmp_buf[7]-48)*0.1;    //�������������Ŀ��y�ٶ�
				  if(tmp_buf[5]=='1') basketballbot.robot_expected_speed.y=-basketballbot.robot_expected_speed.y;	
				  basketballbot.robot_expected_speed.w=(tmp_buf[9]-48)+(tmp_buf[10]-48)*0.1;    //�������������Ŀ��w�ٶ�
				  if(tmp_buf[8]=='1') basketballbot.robot_expected_speed.w=-basketballbot.robot_expected_speed.w;	
			//	 printf("%f  %f  %f\r\n",basketballbot.robot_expected_speed.x,basketballbot.robot_expected_speed.y,basketballbot.robot_expected_speed.w);
				
				  
	       // Set_Speed_Robot_To_Motor();                                                  //����������Ŀ���ٶ�ת��Ϊ����ٶ�
	       // DRIVER_Set_Speed();                                                          //�����з��͵��Ŀ���ٶȸ����
			}
			else ;

}



/**********************


    �����յ�������˵�����£�
    �����յ���Ϊ*λ���ַ���  eg:ffxxyywwff
    ����ǰ�����λ��ffΪ�涨�����ݸ�ʽ
    xx��ʾx������ٶȣ���λΪm/s,��һ��x��ʾ����λ���ڶ���x��ʾС��λ
    yy��ʾy������ٶȣ���λΪm/s,��һ��y��ʾ����λ���ڶ���y��ʾС��λ
    ww��ʾy������ٶȣ���λΪrad/s,��һ��w��ʾ����λ���ڶ���w��ʾС��λ
  

**********************/




