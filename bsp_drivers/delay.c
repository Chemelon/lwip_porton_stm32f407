#include "delay.h"
#include "stm32f4xx.h"

static __IO uint32_t uwTick;

void delay_init(void)
{
    /*����ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    /*10 KHz*/
    TIM6->PSC = (FCK_FREQ / CKCNT_FREQ - 1);
    TIM6->ARR = ARR_CNT;
    /*�����¼�����ARR�Ĵ���*/
    TIM6->EGR = TIM_EGR_UG;
    /*���UIF��־λ*/
    TIM6->SR &= ~TIM_SR_UIF;
}


void delay_ms(uint32_t ms)
{
#if 1
    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = ms;

    /* Add a freq to guarantee minimum wait */
    if (wait < 0xFFFFFFFFU)
    {
        wait += (uint32_t)(1);
    }

    while ((HAL_GetTick() - tickstart) < wait)
    {
    }
#else
    TIM6->CNT = 0;
    TIM6->CR1 |= TIM_CR1_CEN;
    while (!(TIM6->SR & TIM_SR_UIF));
    TIM6->CR1 &= ~TIM_CR1_CEN;
    TIM6->SR &= ~TIM_SR_UIF;
#endif
}
/*��TIM6���ó�100us�ж�*/
void TIM6_it_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;//
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//��������ж�
    TIM6->CR1 |= TIM_CR1_CEN;
}

__weak void HAL_IncTick(void)
{
    uwTick += 1;
}

__weak uint32_t HAL_GetTick(void)
{
    return uwTick;
}
