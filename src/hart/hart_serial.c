#include "hart_driver.h"
#include "hart_frame.h"

// extern void hart_proxy_dequeue(void);
// extern void hart_proxy_enqueue(void);

unsigned char serical_init(unsigned int baud, 
																	unsigned char data_bit, 
																	parity_type parity, 
																	unsigned char stop_bit)
{
	unsigned char initialized = TRUE;
	GPIO_InitTypeDef GPIO_InitStruct; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	#if 0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;              //CTS
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;            //RTS
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	#endif

	USART_InitStructure.USART_BaudRate = baud;
	switch ( parity )
	{
		case HT_SERICAL_NONE:
			USART_InitStructure.USART_Parity = USART_Parity_No;
			break;
		case HT_SERICAL_ODD:
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			break;
		case HT_SERICAL_EVEN:
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			break;
		default:
			initialized = FALSE;
			break;
	}
	switch ( data_bit )
	{
	case 8:
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	case 7:
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	default:
		initialized = FALSE;
		break;
	}
	switch( stop_bit )
	{
		case 2:
			USART_InitStructure.USART_StopBits = USART_StopBits_2;
			break;
		case 1:
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			break;
		default:
			initialized = FALSE;
			break;
			
	}
	if( initialized )
	{
		enter_critical_section( );
		#if 0
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
		#endif
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure);
		USART_Cmd(USART3, ENABLE);
		exit_critical_section( );
	}
	return initialized;
}

void serical_enable(unsigned char rx_enable, unsigned char tx_enable)
{
		enter_critical_section( );
    if( rx_enable )
    {
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    }
    else
    {
       USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
    }
    if( tx_enable )
    {
       USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    }
    else
    {
       USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    }
    exit_critical_section( );
}

unsigned char serical_put_byte(unsigned char *byte)
{
	USART_SendData(USART3, *byte);
	return TRUE;
}

unsigned char serical_get_byte(unsigned char *byte)
{
	*byte = (u8)USART_ReceiveData(USART3);
	return TRUE;
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		//hart_proxy_enqueue( );	
		hart_rcv_msg();
	}
	else if(USART_GetITStatus(USART3,USART_IT_TXE))
	{
		//hart_proxy_dequeue( );
		hart_xmt_msg();
	}
	
}

void
enter_critical_section( void )
{
  __disable_irq();
}

void
exit_critical_section( void )
{
  __enable_irq();
}


