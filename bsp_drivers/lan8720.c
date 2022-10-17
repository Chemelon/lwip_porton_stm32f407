#include "lan8720.h"
#include "stm32f4xx.h"
#include "stm32f4x7_eth.h"
#include "delay.h"
#include "lwipopts.h"
#include "eth.h"
//#include "lwipopts.h"
//#include "netif.h"
//#include "lwip/ip_addr.h"

ETH_InitTypeDef ETH_InitStructure;


void lan8720_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /*RMII mode*/
    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
    /*enable gpio clk*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG, ENABLE);

    /* Configure ETH_MDIO */
    GPIO_InitStructure.GPIO_Pin = ETH_MDIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(ETH_MDIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_MDIO_PORT, ETH_MDIO_SOURCE, ETH_MDIO_AF);

    /* Configure ETH_MDC */
    GPIO_InitStructure.GPIO_Pin = ETH_MDC_PIN;
    GPIO_Init(ETH_MDC_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_MDC_PORT, ETH_MDC_SOURCE, ETH_MDC_AF);

    /* Configure ETH_RMII_REF_CLK */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_REF_CLK_PIN;
    GPIO_Init(ETH_RMII_REF_CLK_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_REF_CLK_PORT, ETH_RMII_REF_CLK_SOURCE, ETH_RMII_REF_CLK_AF);

    /* Configure ETH_RMII_CRS_DV */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_CRS_DV_PIN;
    GPIO_Init(ETH_RMII_CRS_DV_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_CRS_DV_PORT, ETH_RMII_CRS_DV_SOURCE, ETH_RMII_CRS_DV_AF);

    /* Configure ETH_RMII_RXD0 */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_RXD0_PIN;
    GPIO_Init(ETH_RMII_RXD0_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_RXD0_PORT, ETH_RMII_RXD0_SOURCE, ETH_RMII_RXD0_AF);

    /* Configure ETH_RMII_RXD1 */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_RXD1_PIN;
    GPIO_Init(ETH_RMII_RXD1_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_RXD1_PORT, ETH_RMII_RXD1_SOURCE, ETH_RMII_RXD1_AF);

    /* Configure ETH_RMII_TX_EN */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_TX_EN_PIN;
    GPIO_Init(ETH_RMII_TX_EN_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_TX_EN_PORT, ETH_RMII_TX_EN_SOURCE, ETH_RMII_TX_EN_AF);

    /* Configure ETH_RMII_TXD0 */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_TXD0_PIN;
    GPIO_Init(ETH_RMII_TXD0_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_TXD0_PORT, ETH_RMII_TXD0_SOURCE, ETH_RMII_TXD0_AF);

    /* Configure ETH_RMII_TXD1 */
    GPIO_InitStructure.GPIO_Pin = ETH_RMII_TXD1_PIN;
    GPIO_Init(ETH_RMII_TXD1_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ETH_RMII_TXD1_PORT, ETH_RMII_TXD1_SOURCE, ETH_RMII_TXD1_AF);
//����PD3Ϊ�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_3);                  //Ӳ����λLAN8720
    delay_ms(50);
    GPIO_SetBits(GPIOD, GPIO_Pin_3);                    //��λ����
}

void lan8720_macdma_init(void)
{
    uint8_t mac_addr[] = { MAC_ADDR0, 
                            MAC_ADDR1, 
                            MAC_ADDR2, 
                            MAC_ADDR3, 
                            MAC_ADDR4, 
                            MAC_ADDR5 };

    __IO uint32_t  EthStatus = 0;

    /* Enable ETHERNET clock  */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                           RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while (ETH_GetSoftwareResetStatus() == SET);

    /* ETHERNET Configuration --------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
    /* ������������Ӧ���� */
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
//  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
//  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
//  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    /* �رշ��� */
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    /* �ر��ش����� */
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    /* �ر��Զ�ȥ��PDA/CRC����  */
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    /* �رս������е�֡ */
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    /* ����������й㲥֡ */
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    /* �رջ��ģʽ�ĵ�ַ����  */
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    /* �����鲥��ַʹ��������ַ����    */
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    /* �Ե�����ַʹ��������ַ����  */
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    /* ����ipv4��TCP/UDP/ICMP��֡У���ж��   */
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

    /*------------------------   DMA   -----------------------------------*/

    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    /*������ʹ��֡У���ж�ع��ܵ�ʱ��һ��Ҫʹ�ܴ洢ת��ģʽ,�洢ת��ģʽ��Ҫ��֤����֡�洢��FIFO��,
    ����MAC�ܲ���/ʶ���֡У��ֵ,����У����ȷ��ʱ��DMA�Ϳ��Դ���֡,����Ͷ�������֡*/

    /* ��������TCP/IP����֡ */
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    /* �����������ݵĴ洢ת��ģʽ  */
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    /* �����������ݵĴ洢ת��ģʽ   */
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

    /* ��ֹת������֡ */
    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    /* ��ת����С�ĺ�֡ */
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    /* �򿪴���ڶ�֡���� */
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    /* ����DMA����ĵ�ַ���빦�� */
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    /* �����̶�ͻ������ */
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
    /* DMA���͵����ͻ������Ϊ32������ */
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    /*DMA���յ����ͻ������Ϊ32������ */
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* Configure Ethernet */
    /* ����ETH */
    EthStatus = ETH_Init(&ETH_InitStructure, ETHERNET_PHY_ADDRESS);

    /* initialize MAC address in ethernet MAC */
    ETH_MACAddressConfig(ETH_MAC_Address0, mac_addr);

}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(void)
{
  /* Enable ETHERNET clock  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                        RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
                        
  /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();

  /* Software reset */
  ETH_SoftwareReset();

  /* Wait for software reset */
  while (ETH_GetSoftwareResetStatus() == SET);

  /* ETHERNET Configuration --------------------------------------------------*/
  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
  ETH_StructInit(&ETH_InitStructure);

  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
	/* ������������Ӧ���� */
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
//  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable; 
//  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
//  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;   
  /* �رշ��� */
  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	/* �ر��ش����� */
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	/* �ر��Զ�ȥ��PDA/CRC����  */
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	/* �رս������е�֡ */
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	/* ����������й㲥֡ */
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	/* �رջ��ģʽ�ĵ�ַ����  */
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	/* �����鲥��ַʹ��������ַ����    */
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	/* �Ե�����ַʹ��������ַ����  */
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
	/* ����ipv4��TCP/UDP/ICMP��֡У���ж��   */
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

  /*------------------------   DMA   -----------------------------------*/  

  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
	/*������ʹ��֡У���ж�ع��ܵ�ʱ��һ��Ҫʹ�ܴ洢ת��ģʽ,�洢ת��ģʽ��Ҫ��֤����֡�洢��FIFO��,
	����MAC�ܲ���/ʶ���֡У��ֵ,����У����ȷ��ʱ��DMA�Ϳ��Դ���֡,����Ͷ�������֡*/
	
	/* ��������TCP/IP����֡ */
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
	/* �����������ݵĴ洢ת��ģʽ  */
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
	/* �����������ݵĴ洢ת��ģʽ   */
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

	/* ��ֹת������֡ */
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	/* ��ת����С�ĺ�֡ */
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	/* �򿪴���ڶ�֡���� */
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	/* ����DMA����ĵ�ַ���빦�� */
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	/* �����̶�ͻ������ */
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
	/* DMA���͵����ͻ������Ϊ32������ */
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
	/*DMA���յ����ͻ������Ϊ32������ */
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

  /* Configure Ethernet */
	/* ����ETH */
  ETH_Init(&ETH_InitStructure, ETHERNET_PHY_ADDRESS);
}

void ETH_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

/* Enable GPIOs clocks */
  RCC_AHB1PeriphClockCmd(ETH_MDIO_GPIO_CLK            | ETH_MDC_GPIO_CLK          |
                         ETH_RMII_REF_CLK_GPIO_CLK    | ETH_RMII_CRS_DV_GPIO_CLK  |
                         ETH_RMII_RXD0_GPIO_CLK       | ETH_RMII_RXD1_GPIO_CLK    |
                         ETH_RMII_TX_EN_GPIO_CLK      | ETH_RMII_TXD0_GPIO_CLK    |
                         ETH_RMII_TXD1_GPIO_CLK       | RCC_AHB1Periph_GPIOD, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


  /* MII/RMII Media interface selection --------------------------------------*/
  SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);




/* Ethernet pins configuration ************************************************/
   /*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PB11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
				ETH_NRST -------------------------> PI1
                                                  */

   /* Configure ETH_MDIO */
  GPIO_InitStructure.GPIO_Pin = ETH_MDIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(ETH_MDIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_MDIO_PORT, ETH_MDIO_SOURCE, ETH_MDIO_AF);
	
	/* Configure ETH_MDC */
  GPIO_InitStructure.GPIO_Pin = ETH_MDC_PIN;
  GPIO_Init(ETH_MDC_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_MDC_PORT, ETH_MDC_SOURCE, ETH_MDC_AF);
	
	/* Configure ETH_RMII_REF_CLK */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_REF_CLK_PIN;
  GPIO_Init(ETH_RMII_REF_CLK_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_REF_CLK_PORT, ETH_RMII_REF_CLK_SOURCE, ETH_RMII_REF_CLK_AF);
	
	/* Configure ETH_RMII_CRS_DV */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_CRS_DV_PIN;
  GPIO_Init(ETH_RMII_CRS_DV_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_CRS_DV_PORT, ETH_RMII_CRS_DV_SOURCE, ETH_RMII_CRS_DV_AF);
	
	/* Configure ETH_RMII_RXD0 */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_RXD0_PIN;
  GPIO_Init(ETH_RMII_RXD0_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_RXD0_PORT, ETH_RMII_RXD0_SOURCE, ETH_RMII_RXD0_AF);
	
	/* Configure ETH_RMII_RXD1 */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_RXD1_PIN;
  GPIO_Init(ETH_RMII_RXD1_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_RXD1_PORT, ETH_RMII_RXD1_SOURCE, ETH_RMII_RXD1_AF);
	
	/* Configure ETH_RMII_TX_EN */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_TX_EN_PIN;
  GPIO_Init(ETH_RMII_TX_EN_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_TX_EN_PORT, ETH_RMII_TX_EN_SOURCE, ETH_RMII_TX_EN_AF);
	
	/* Configure ETH_RMII_TXD0 */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_TXD0_PIN;
  GPIO_Init(ETH_RMII_TXD0_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_TXD0_PORT, ETH_RMII_TXD0_SOURCE, ETH_RMII_TXD0_AF);
	
	/* Configure ETH_RMII_TXD1 */
  GPIO_InitStructure.GPIO_Pin = ETH_RMII_TXD1_PIN;
  GPIO_Init(ETH_RMII_TXD1_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ETH_RMII_TXD1_PORT, ETH_RMII_TXD1_SOURCE, ETH_RMII_TXD1_AF);		
	//����PD3Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD , GPIO_Pin_3);					//Ӳ����λLAN8720
	delay_ms(50);	
	GPIO_SetBits(GPIOD , GPIO_Pin_3);				 	//��λ���� 
}

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{
  /* Configure the GPIO ports for ethernet pins */
  ETH_GPIO_Config();

  /* Configure the Ethernet MAC/DMA */
  ETH_MACDMA_Config();

}

#if 0
/**
  * @brief  Link callback function, this function is called on change of link status.
  * @param  The network interface
  * @retval None
  */
void ETH_link_callback(struct netif *netif)
{
    __IO uint32_t timeout = 0;
    uint32_t tmpreg, RegValue;
    ip4_addr_t ipaddr;
    ip4_addr_t netmask;
    ip4_addr_t gw;

    if (netif_is_link_up(netif))
    {
        /* Restart the autonegotiation */
        if (ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
        {
            /* Reset Timeout counter */
            timeout = 0;

            /* Enable auto-negotiation */
            ETH_WritePHYRegister(ETHERNET_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

            /* Wait until the auto-negotiation will be completed */
            do
            {
                timeout++;
            }
            while (!(ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));

            /* Reset Timeout counter */
            timeout = 0;

            /* Read the result of the auto-negotiation */
            RegValue = ETH_ReadPHYRegister(ETHERNET_PHY_ADDRESS, PHY_SR);

            /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
            if ((RegValue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
            {
                /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
                ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
            }
            else
            {
                /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
                ETH_InitStructure.ETH_Mode = ETH_Mode_HalfDuplex;
            }
            /* Configure the MAC with the speed fixed by the auto-negotiation process */
            if (RegValue & PHY_SPEED_STATUS)
            {
                /* Set Ethernet speed to 10M following the auto-negotiation */
                ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
            }
            else
            {
                /* Set Ethernet speed to 100M following the auto-negotiation */
                ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
            }

//      /* This is different for every PHY */
//          ETH_EXTERN_GetSpeedAndDuplex(ETHERNET_PHY_ADDRESS, &ETH_InitStructure);

            /*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
            /* Get the ETHERNET MACCR value */
            tmpreg = ETH->MACCR;

            /* Set the FES bit according to ETH_Speed value */
            /* Set the DM bit according to ETH_Mode value */
            tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

            /* Write to ETHERNET MACCR */
            ETH->MACCR = (uint32_t)tmpreg;

            _eth_delay_(ETH_REG_WRITE_DELAY);
            tmpreg = ETH->MACCR;
            ETH->MACCR = tmpreg;
        }

        /* Restart MAC interface */
        ETH_Start();

#ifdef USE_DHCP
        ipaddr.addr = 0;
        netmask.addr = 0;
        gw.addr = 0;
        DHCP_state = DHCP_START;
#else
        IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
        IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
        IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif /* USE_DHCP */

        netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);

        /* When the netif is fully configured this function must be called.*/
        netif_set_up(&gnetif);

    }
    else
    {
        ETH_Stop();
#ifdef USE_DHCP
        DHCP_state = DHCP_LINK_DOWN;
        dhcp_stop(netif);
#endif /* USE_DHCP */

        /*  When the netif link is down this function must be called.*/
        netif_set_down(&gnetif);
    }
}
#endif

