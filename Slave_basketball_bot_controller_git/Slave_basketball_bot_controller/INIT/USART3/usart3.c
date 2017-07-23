#include "data.h"

extern Queue rx_queue;
u8 datae[256];
int count;
extern robot basketballbot;

/********************************************

      此部分代码对串口3进行初始化
			USART3_TX    --    GPIOB_PIN_10
		  USART3_RX    --    GPIOB_PIN_11

      USART3主要依靠串口消息队列与上位机进行通信获取软件组的相关命令
			
********************************************/



void USART3_Init(u32 bound)
{

     USART_InitTypeDef USART3_InitStructure;
	   GPIO_InitTypeDef  GPIO_InitStructure;
	   NVIC_InitTypeDef  NVIC_InitStructure;
	   
	   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);                                    //使能GPIOB时钟
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);                                    //使能USART3时钟
     
	
	   GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);                                 //GPIOB10复用为USART3
	   GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);                                 //GPIOB11复用为USART3
	
	
	/*****************************************************************************/
	
	   //USART3端口配置
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;                                 //GPIOB10与GPIOB11
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                             //开启IO复用功能
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                      //速度50MHz
	   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                           //推挽复用输出
	   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                             //上拉
	   GPIO_Init(GPIOB,&GPIO_InitStructure);                                                    //初始化GPIOB10、GPIOB11
	
     //USART3 初始化设置
	   USART3_InitStructure.USART_BaudRate = 115200;                                            //波特率设置为115200
	   USART3_InitStructure.USART_WordLength = USART_WordLength_8b;                             //字长为8位数据格式
	   USART3_InitStructure.USART_StopBits = USART_StopBits_1;                                  //一个停止位
	   USART3_InitStructure.USART_Parity = USART_Parity_No;                                     //无奇偶校验位
	   USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;         //无硬件数据流控制
	   USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                        //收发模式
     USART_Init(USART3, &USART3_InitStructure);                                               //初始化串口3
	   
		 USART_Cmd(USART3, ENABLE);                                                               //使能串口3
		 USART_ClearFlag(USART3, USART_FLAG_TC);                                                  //中断标志位清零
		 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                                           //开启串口3的接收中断
     
	   //串口3中断优先级配置
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                                        //选择串口3中断通道
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;                                  //抢占优先级1
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		                                    //子优先级2
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                                    //IRQ通道使能
	   NVIC_Init(&NVIC_InitStructure);	                                                        //根据指定的参数初始化VIC寄存器
		 
}


//串口3中断处理函数
void USART3_IRQHandler(void)     
{
	 char Rx_data = 0;                                                                          //定义此数据用于中转串口接收到的数据
		
	
/**************************以下为串口3中断的具体处理过程*******************************/
	
	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)                                     //是否串口3接收中断发生
    {
			if(queue_full(rx_queue))
			{		   		   
				USART3_SendChar(0xff);
				USART3_SendChar(0xff);
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);	//关闭串口2	 	
			}
			else
			{
					Rx_data  = USART3->DR;                                                              //获取串口3接收到的数据
				  if(Rx_data==0x68)	LED0=!LED0;
					queue_put(&rx_queue, Rx_data);                                                      //数据
			}
			
/**************************以上为串口3中断的具体处理过程*******************************/			
			
			
      USART_ClearITPendingBit(USART3,USART_IT_RXNE);			                                    //中断标志位清零
    }
}

void USART3_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
		USART_SendData(USART3,b);
}
