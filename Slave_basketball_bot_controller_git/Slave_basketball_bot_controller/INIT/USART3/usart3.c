#include "data.h"

extern Queue rx_queue;
u8 datae[256];
int count;
extern robot basketballbot;

/********************************************

      �˲��ִ���Դ���3���г�ʼ��
			USART3_TX    --    GPIOB_PIN_10
		  USART3_RX    --    GPIOB_PIN_11

      USART3��Ҫ����������Ϣ��������λ������ͨ�Ż�ȡ�������������
			
********************************************/



void USART3_Init(u32 bound)
{

     USART_InitTypeDef USART3_InitStructure;
	   GPIO_InitTypeDef  GPIO_InitStructure;
	   NVIC_InitTypeDef  NVIC_InitStructure;
	   
	   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);                                    //ʹ��GPIOBʱ��
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);                                    //ʹ��USART3ʱ��
     
	
	   GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);                                 //GPIOB10����ΪUSART3
	   GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);                                 //GPIOB11����ΪUSART3
	
	
	/*****************************************************************************/
	
	   //USART3�˿�����
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;                                 //GPIOB10��GPIOB11
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                             //����IO���ù���
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                      //�ٶ�50MHz
	   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                           //���츴�����
	   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                             //����
	   GPIO_Init(GPIOB,&GPIO_InitStructure);                                                    //��ʼ��GPIOB10��GPIOB11
	
     //USART3 ��ʼ������
	   USART3_InitStructure.USART_BaudRate = 115200;                                            //����������Ϊ115200
	   USART3_InitStructure.USART_WordLength = USART_WordLength_8b;                             //�ֳ�Ϊ8λ���ݸ�ʽ
	   USART3_InitStructure.USART_StopBits = USART_StopBits_1;                                  //һ��ֹͣλ
	   USART3_InitStructure.USART_Parity = USART_Parity_No;                                     //����żУ��λ
	   USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;         //��Ӳ������������
	   USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                        //�շ�ģʽ
     USART_Init(USART3, &USART3_InitStructure);                                               //��ʼ������3
	   
		 USART_Cmd(USART3, ENABLE);                                                               //ʹ�ܴ���3
		 USART_ClearFlag(USART3, USART_FLAG_TC);                                                  //�жϱ�־λ����
		 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                                           //��������3�Ľ����ж�
     
	   //����3�ж����ȼ�����
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                                        //ѡ�񴮿�3�ж�ͨ��
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;                                  //��ռ���ȼ�1
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		                                    //�����ȼ�2
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                                    //IRQͨ��ʹ��
	   NVIC_Init(&NVIC_InitStructure);	                                                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
		 
}


//����3�жϴ�����
void USART3_IRQHandler(void)     
{
	 char Rx_data = 0;                                                                          //���������������ת���ڽ��յ�������
		
	
/**************************����Ϊ����3�жϵľ��崦�����*******************************/
	
	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)                                     //�Ƿ񴮿�3�����жϷ���
    {
			if(queue_full(rx_queue))
			{		   		   
				USART3_SendChar(0xff);
				USART3_SendChar(0xff);
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);	//�رմ���2	 	
			}
			else
			{
					Rx_data  = USART3->DR;                                                              //��ȡ����3���յ�������
				  if(Rx_data==0x68)	LED0=!LED0;
					queue_put(&rx_queue, Rx_data);                                                      //����
			}
			
/**************************����Ϊ����3�жϵľ��崦�����*******************************/			
			
			
      USART_ClearITPendingBit(USART3,USART_IT_RXNE);			                                    //�жϱ�־λ����
    }
}

void USART3_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
		USART_SendData(USART3,b);
}
