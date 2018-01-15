#ifndef _FRSKYD16_H_
#define _FRSKYD16_H_

//�����ͨ������(���֧��8ͨ��)
typedef enum
{
  	RUDDER   	= 0 , 
	THROTTLE 	= 1 , 
	ELEVATOR 	= 2 , 
	AILERON  	= 3 , 
	AUX1  		= 4 , 
	AUX2		= 5 , 
	AUX3		= 6 , 
	AUX4		= 7 , 
	AUX5  		= 8 , 
	AUX6		= 9 , 
	AUX7		= 10 , 
	AUX8		= 11 , 
	AUX9  		= 12 , 
	AUX10		= 13 , 
	AUX11		= 14 , 
	AUX12		= 15 , 
}ChannelTypeDef ;

enum TXRX_State 
{
	TXRX_OFF,
	TX_EN,
	RX_EN
};

extern bool CommunicationError_flg ;

#define RF_TypeVersion		  0x46						//ң�������� 'F' -> FRSKYD16
#define MasterInitProtocolVersion 0x01						//����Э��汾��
#define PTOTOCOL_MAX_CHANNEL      8						//Э��֧�������ͨ�� 8 ͨ��(�̶�����16ͨ������)
#define TRANSMITTER_CHANNEL       8

extern bool CommunicationError_flg ; 
extern bool HighThrottle_flg ; 
extern bool Bind_flg ; 
extern uint16_t TransmitterID ; 
////////////////////////////////////////////////////////////////////////////////
//SFHSS 8ͨ �������ݷ�������
////////////////////////////////////////////////////////////////////////////////
extern uint16_t FRSKYD16_SendDataBuff[16];
extern const uint8_t FRSKYD16_CH_Code[16];
void     initFRSKYD16(void);
uint16_t ReadFRSKYD16(void);
void SetBind(void) ; 

#endif

