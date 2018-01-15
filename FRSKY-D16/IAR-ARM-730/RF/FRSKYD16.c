/*******************************************************************************
--------------------------------------------------------------------------------
FRSKY-D16 ע��ļ��� : 
(1) ң������ס������ϵ�,�������״̬(��������1000�����ݰ�(18mS*1000 = 18S)���˳�����״̬)��
(2) ����ң���� ID �� ������һ����Ƶ�б�(��47��Ƶ��)��
--------------------------------------------------------------------------------
FRSKYD16Э�� : ������ 18mS  ���������ݷ��ͣ� ÿ������ 30Byte (���ջ�32Byte , ������1Byte RSSI   1Byte LQI)  ǰ8ͨ������������ �� ��8ͨ�ۼ� 2048 �ڷ���(���������ڲ�ͬƵ���ڽ���)
               ���� : �̶��� ͨ�� 0 
	       ��Ƶ : �̶� 47 ͨ�� �� ͨ��ң�������ɵ��������������Ƶ������

�������ݰ�(30Byte) :  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29
                      0x1D 0x03 0x01 0xAA 0xBB 0xCC 0xDD 0xEE 0xFF 0xGG 0xHH 0xII 0xJJ 0X00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xKK 0xLL 
���� : 
[0] : 0x1D
[1] : 0x03
[2] : 0x01
[3][4] : ң����ID��  
[5]   [6][7][8][9][10]: [5] Ϊͨ��ǰ����  [6][7][8][9][10] Ϊ [5] ��ʼ �� 5(����[5]) ͨ��Ƶ���б�
[11] : Ϊң�����ͺŴ���(FRSKY_X7 : 0x05    FRSKY_X9D_Plus : 0x03)
[12] : Ϊң�������õĽ������к�( 0 -> 63) 
[13] -> [27] : �̶�Ϊ0x00
[28][29]: Ϊ���ݰ�У���(��Ҫ��� [3] -> [27])  


�������ݰ�(30Byte) :  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29
                      0x1D 0xAA 0xBB 0xCC 0xDD 0xEE 0xFF 0xGG 0x00 0xHH 0xII 0xJJ 0xKK 0xLL 0xMM 0xNN 0xOO 0xPP 0xQQ 0xRR 0xSS 0xTT 0x00 0x00 0x00 0x00 0x00 0x00 0xKK 0xLL 
���� :
[0]    : 0x1D
[1][2] : ң���� ID ��
[3]    : Ϊң�����ͺŴ���(FRSKY_X7 : 0x05    FRSKY_X9D_Plus : 0x03)
[4]    : (ctr<<6) + hopping_frequency_no(��ǰ��Ƶͨ����)
[5]    : (chanskip - ctr)>>2 (chanskipΪ��Ƶ�ż��)
[6]    : Ϊң�������õĽ������к�( 0 -> 63) 
[7]    : ���߱�������ͨ����־λ(����˱�־λ��Ϊ0  ��ʾ����������е��߱�����������   �˳���ֱ�������˵��߱������� ��Լ����ʧ 1.6% ���ݰ���������Ӱ��ʹ��)
[9][10][11]  : ͨ��1��2/9��10������ (С��2048Ϊͨ��1��2   ��֮Ϊͨ��9 ��10)
[12][13][14] : ͨ��3��4/11��12������(С��2048Ϊͨ��3��4   ��֮Ϊͨ��11��12)
[15][16][17] : ͨ��5��6/13��14������(С��2048Ϊͨ��5��6   ��֮Ϊͨ��13��14)
[18][19][20] : ͨ��7��8/15��16������(С��2048Ϊͨ��7��8   ��֮Ϊͨ��15��16)
[21] : ң������һ�η������к� + ��һ�ν������к�(����ң��)
[22] -> [27] : �̶�Ϊ 0x00 
[28][29] : Ϊ���ݰ�У���(��Ҫ��� [3] -> [27])  

--------------------------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ĳЩ���ջ���ʱ��Ҫ���ر��ϸ� : �����κ�Ӱ��ʱ��Ĳ����޸��ر�С�ġ�(��ȷ�� uS)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*******************************************************************************/
#include "include.h"

#define FRSKYD16_PACKET_LEN  30
#define FRSKYD16_BINDCHANNEL 47 						//��Ƶ�б��47��Ƶ�ι̶�Ϊ����Ƶ�� 0 



bool     Bind_flg = false ; 
uint16_t FRSKYD16_BindCounts = 0 ; 						// �������ݰ����͸���

uint8_t	 FRSKYD16_ChannelShip = 0 ;       					// ��Ƶ���(ǰ������Ƶ�μ��)
uint8_t  FRSKYD16_ctr = 0 ; 							
uint8_t  FRSKYD16_CountsRst = 0 ; 						
uint8_t  FRSKYD16_HOPChannel[50] ; 						// ��Ƶ�б�(����ң����ID�����47����Ƶ��(������Ƶ����Ч))	
uint8_t  FRSKYD16_calData[50];							// ��¼��Ƶͨ��Ƶ��ֵ

uint8_t  FRSKYD16_Channel_Num = 0   ; 						// ��Ƶͨ����
bool CommunicationError_flg = false ; 
bool HighThrottle_flg = true ; 							//�����ű�־λ
uint16_t TransmitterID ; 							//ң����ΨһID
uint8_t  SendPacket[40] ; 							//�������ݰ����� (1) �������ݰ�14Byte   (2)����ң�����ݰ� 28Byte(8 + 16CH*2 = 40)

typedef enum 
{
	FRSKYD16_BIND  		= 0x00 , 
	FRSKYD16_BIND_PASSBACK	= 0x01 , 
	FRSKYD16_DATA  		= 0x02 ,	
    	FRSKYD16_TUNE  		= 0x03 ,
}FRSKYD16PhaseTypeDef ;
FRSKYD16PhaseTypeDef FRSKYD16Phase = FRSKYD16_DATA ; 

//Channel values are 12-bit values between 988 and 2012, 1500 is the middle.
uint16_t FRSKYD16_SendDataBuff[16]  = { 1500 , 1500 , 988 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500 , 1500};

//FRSKYD16 ͨ������˳��
const uint8_t  FRSKYD16_CH_Code[16] = {AILERON, ELEVATOR, THROTTLE, RUDDER, AUX1, AUX2, AUX3, AUX4, AUX5, AUX6, AUX7, AUX8, AUX9, AUX10, AUX11, AUX12};

//==============================================================================
//			FRSKYD16 ��ʼ��������ַ
//==============================================================================
static void FRSKYD16_InitDeviceAddr(bool IsBindFlg)
{
	CC2500_WriteReg(CC2500_0C_FSCTRL0 , 0x00); 
	CC2500_WriteReg(CC2500_18_MCSM0,    0x08) ;	
	CC2500_WriteReg(CC2500_09_ADDR , IsBindFlg ? 0x03 : (TransmitterID & 0xFF));
	CC2500_WriteReg(CC2500_07_PKTCTRL1,0x05);
}

//==============================================================================
//			FRSKYD16 ���÷���ͨ��
//==============================================================================
static void FRSKYD16_TuneChannel(uint8_t Channel)
{
	CC2500_Strobe(CC2500_SIDLE);						//��������״̬
	CC2500_WriteReg(CC2500_25_FSCAL1, FRSKYD16_calData[Channel]);		//���÷���ͨ��
	CC2500_WriteReg(CC2500_0A_CHANNR, FRSKYD16_HOPChannel[Channel]);	//���÷���ͨ��
	CC2500_Strobe(CC2500_SCAL);						//У׼Ƶ�ʺϳ������ر�
}

//==============================================================================
//				CRCУ��
//==============================================================================
static const uint16_t CRC_Short[16] = 	//CRC У������
{
	0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
	0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7 
};

static uint16_t CRCTable(uint8_t val)
{
	uint16_t word ;
	word = CRC_Short[val&0x0F] ;
	val /= 16 ;
	return word ^ (0x1081 * val) ;
}

static uint16_t crc_x(uint8_t *data, uint8_t len)
{
	uint16_t crc = 0;
	for(uint8_t i=0; i < len; i++)
	{
		crc = (crc<<8) ^ CRCTable((uint8_t)(crc>>8) ^ *data++);
	}
	return crc;
}

//ͨ�����ݻ���
static uint16_t  CalChannelData( uint8_t channel)
{	
  	if(channel > 15) channel = 15 ; 
  	uint16_t DataTemp = FRSKYD16_SendDataBuff[channel] ; 
	
  	if(HighThrottle_flg == true)
	{
		if(channel == THROTTLE) DataTemp =  THR_Output_Min ; 
		else 		        DataTemp =  Output_Mid ; 
	}
	
	return (uint16_t)(((DataTemp - 860) * 3) >> 1) + 64;		//mapped 0 ,2140(125%) range to 64,1984 ;
}


//==============================================================================
//			SFHSS �����л�����ͨ��
//���� : ��Ϊ�ϵ�ʱ�Ѿ�У׼��Ƶ�ʣ��һض���Ƶ��ֵ�����Դ˴�����ֱ������ǰ��ض���
//Ƶ��ֵ��������Ƶ��У׼ʱ�䡣
//==============================================================================
static void FRSKYD16_tune_chan_fast(void)
{
	CC2500_Strobe(CC2500_SIDLE);
	CC2500_WriteReg(CC2500_0D_FREQ2 , Fre_Carrier_H);
	CC2500_WriteReg(CC2500_0E_FREQ1 , Fre_Carrier_M);
	CC2500_WriteReg(CC2500_0F_FREQ0 , Fre_Carrier_L);
	DelayUs(2);
	CC2500_Strobe(CC2500_SIDLE);
	CC2500_WriteReg(CC2500_0D_FREQ2 , Fre_Carrier_H);
	CC2500_WriteReg(CC2500_0E_FREQ1 , Fre_Carrier_M);
	CC2500_WriteReg(CC2500_0F_FREQ0 , Fre_Carrier_L);
	CC2500_WriteReg(CC2500_25_FSCAL1, FRSKYD16_calData[FRSKYD16_Channel_Num]);
	CC2500_WriteReg(CC2500_0A_CHANNR, FRSKYD16_HOPChannel[FRSKYD16_Channel_Num]);
	//CC2500_Strobe(CC2500_SCAL);
	//CC2500_Strobe(CC2500_SRX);
}

//==============================================================================
//			������һ��Ƶ��
//==============================================================================
static void FRSKYD16_calc_next_chan(void)
{
    	FRSKYD16_Channel_Num = (FRSKYD16_Channel_Num + FRSKYD16_ChannelShip) % 47 ;
}

//==============================================================================
//�������ʽ
//==============================================================================
static void  FRSKYD16_build_Bind_packet(void)
{
  	//�̶���
	SendPacket[0] 	= 0x1D;       
	SendPacket[1] 	= 0x03;          
	SendPacket[2] 	= 0x01; 
	
	//ң���� ID ��	
	SendPacket[3]   = (TransmitterID >> 8) & 0xFF  ;
	SendPacket[4]   = TransmitterID & 0xFF ;

	uint8_t  idx 	= (FRSKYD16_BindCounts % 10) * 5 ;
	SendPacket[5]   = idx;	
	SendPacket[6]   = FRSKYD16_HOPChannel[idx++];
	SendPacket[7]   = FRSKYD16_HOPChannel[idx++];
	SendPacket[8]   = FRSKYD16_HOPChannel[idx++];
	SendPacket[9]   = FRSKYD16_HOPChannel[idx++];
	SendPacket[10]  = FRSKYD16_HOPChannel[idx++];
	SendPacket[11]  = 0x02;
	SendPacket[12]  = 0x01; 
	
	//�̶�Ϊ0
	SendPacket[13] 	= 0x00;
	SendPacket[14] 	= 0x00;
	SendPacket[15] 	= 0x00;
	SendPacket[16] 	= 0x00;
	SendPacket[17] 	= 0x00;
	SendPacket[18] 	= 0x00;
	SendPacket[19] 	= 0x00;
	SendPacket[20] 	= 0x00;
	SendPacket[21] 	= 0x00;
	SendPacket[22] 	= 0x00;
	SendPacket[23] 	= 0x00;
	SendPacket[24] 	= 0x00;
	SendPacket[25] 	= 0x00;
	SendPacket[26] 	= 0x00;
	SendPacket[27] 	= 0x00;
	
	//���ݰ�У���
	uint16_t lcrc = crc_x(&SendPacket[3], 25);
	
	SendPacket[28] = lcrc >> 8;
	SendPacket[29] = lcrc;
}

//==============================================================================
//���ݰ���ʽ
//==============================================================================
static void  FRSKYD16_build_Data_packet(void)
{
	static uint8_t lpass;
	uint16_t chan_0 ;
	uint16_t chan_1 ; 
	uint8_t startChan = 0;
	
	// �̶��� + ң���� ID
	SendPacket[0] 	= 0x1D; 
	SendPacket[1]   = (TransmitterID >> 8) & 0xFF  ;
	SendPacket[2]   = TransmitterID & 0xFF ;
	SendPacket[3] 	= 0x02;
	
	//  
	SendPacket[4] = (FRSKYD16_ctr<<6) + FRSKYD16_Channel_Num; 
	SendPacket[5] = FRSKYD16_CountsRst;
	SendPacket[6] = 0x01;
	
	
	if(FRSKYD16_Channel_Num == 0x21) 
	  FRSKYD16_Channel_Num = 0x21 ; 
	//
	SendPacket[7] = 0;
	SendPacket[8] = 0;
	
	//
	if (lpass & 1) startChan += 8 ;
	
	//����ͨ������
	for(uint8_t i = 0; i <12 ; i+=3)
	{
	  	//12 bytes
		chan_0 = CalChannelData(startChan);		 
		if(lpass & 1 ) chan_0 += 2048;			
		startChan += 1 ;
		
		//
		chan_1 = CalChannelData(startChan);		
		if(lpass & 1 ) chan_1+= 2048;		
		startChan+=1;
		
		//
		SendPacket[9+i]   = (chan_0 & 0xFF); 
		SendPacket[9+i+1] = (((chan_0>>8) & 0x0F)|(chan_1 << 4));
		SendPacket[9+i+2] = chan_1 >> 4;
	}

	SendPacket[21] = 0x08 ; 
	//��һ������ ���� �� 8 ͨ
	lpass += 1 ;
	
	for (uint8_t i=22;i<28;i++)
	{
		SendPacket[i]=0;
	}
	uint16_t lcrc = crc_x(&SendPacket[3], 25);
	
	SendPacket[28]=lcrc>>8;   //high byte
	SendPacket[29]=lcrc;      //low byte
}

//==============================================================================
//FRSKYD16 : ���� FRSKYD16 ͨ��(ͨ������õ� 47 ��ͨ�� ����ѯʱ������47��ͨ������Ƶ)
//������Ƶ�μ���� 5 ����
// 1  - 26  : ȡ 16 ��Ƶ��
// 27 - 52  : ȡ 15 ��Ƶ��
// 53 - 76  : ȡ 16 ��Ƶ��
//==============================================================================
void Calc_FRSKYD16_Channel()
{
	uint8_t  idx = 0;
	uint16_t id_tmp = ~ TransmitterID; 					//ID�� ��λȡ��
	
	while(idx < 47)
	{
		uint8_t i;
		uint8_t count_1_26 = 0, count_27_52 = 0, count_53_76 = 0;
		id_tmp = id_tmp * 0x0019660D + 0x3C6EF35F;			// Randomization
		uint8_t next_ch = ((id_tmp >> 8) % 0x4B) + 1;			// Use least-significant byte and must be larger than 1
		for (i = 0; i < idx; i++)
		{
			if(FRSKYD16_HOPChannel[i] == next_ch)    	break;
			if(FRSKYD16_HOPChannel[i] < 27) 		count_1_26++;
			else if (FRSKYD16_HOPChannel[i] < 53)  		count_27_52++;
			else                                    	count_53_76++;
		}
		if (i != idx)  continue;					//˵��û�бȶ���(������һ��Ƶ���ص���������Ƶ��������ѡ��)
		
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// ������Ƶ������Ҫ���� 5 ���� (��һ��Ƶ�������ж�)
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(idx)
		{
			uint8_t Temp = 0 ; 
			if(next_ch > FRSKYD16_HOPChannel[idx - 1]) 	Temp = next_ch - FRSKYD16_HOPChannel[idx - 1] ; 
			else 						Temp = FRSKYD16_HOPChannel[idx - 1] - next_ch ; 
		  	if(Temp < 5)	continue ; 
		}
		
		if(next_ch == 0)  break; 
		
		// 1  - 26  : ȡ 16 ��Ƶ��
		// 27 - 52  : ȡ 15 ��Ƶ��
		// 53 - 76  : ȡ 16 ��Ƶ��
		if (((next_ch < 27) && (count_1_26 < 16)) || ((next_ch >= 27) && (next_ch < 53) && (count_27_52 < 15)) || ((next_ch >= 53) && (count_53_76 < 16)))
		{
			FRSKYD16_HOPChannel[idx++] = next_ch;
		}
	}
	
	//����Ƶ�ι̶�Ϊ 0 
	FRSKYD16_HOPChannel[FRSKYD16_BINDCHANNEL] = 0 ; 
}

//==============================================================================
//���߷�����ת����
//==============================================================================
uint16_t ReadFRSKYD16(void)
{
  	static uint8_t Cnts = 0 ; 
	switch(FRSKYD16Phase)
	{
		//���Ͷ������ݰ�
		case FRSKYD16_BIND : 
		  	if(FRSKYD16_BindCounts < 1200)
			{
				FRSKYD16_TuneChannel(FRSKYD16_BINDCHANNEL) ; 
				CC2500_Strobe(CC2500_SFRX);
		  		FRSKYD16_build_Bind_packet() ;
				CC2500_Strobe(CC2500_SIDLE);
				CC2500_WriteData(SendPacket, FRSKYD16_PACKET_LEN);
				++FRSKYD16_BindCounts ; 
				FRSKYD16Phase = FRSKYD16_BIND_PASSBACK ; 
				Cnts = 0 ; 
			}  
			else
			{
			  	Bind_flg = false ; 
				FRSKYD16_BindCounts = 0 ; 
				FRSKYD16_Channel_Num = 0 ; 
				FRSKYD16_InitDeviceAddr(Bind_flg) ;
				
				//ע�� : ֻ����������� �� �������ָʾ����˸ ; �������У׼�� �� ��������һ���ط�����
				if(MenuCtrl.RunStep == __stSarttUp)
				{
					LED_State_Shake &= ~LED_BIND  ; 
					LED_State_ON    |= LED_BIND   ; 
					
					if(RunStatus == __stNormal)
					{
						beepCmd(BindFreCounts , __stStop);
					}
				}
				
				FRSKYD16Phase = FRSKYD16_DATA ; 
				
			}
			return 1925 ;
		
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// ���ڽ��������� 9mS������ 4�� 2mS(����ң����ʱ��)
		// ʵ��Э���в�����Ҫ�˲�����
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case FRSKYD16_BIND_PASSBACK :
		  	if(Cnts < 3)
			{
				++Cnts ; 
			}
			else 
			{
				FRSKYD16Phase = FRSKYD16_BIND ;
			}
		  	return 2634 ;
		  
		//�������ݰ�
		case FRSKYD16_DATA : 
		  	FRSKYD16_calc_next_chan();
			FRSKYD16_tune_chan_fast();
			FRSKYD16_build_Data_packet();
			CC2500_Strobe(CC2500_SIDLE);	
			CC2500_WriteData(SendPacket, FRSKYD16_PACKET_LEN);
			FRSKYD16Phase = FRSKYD16_TUNE ;
			Cnts = 0 ; 
			return 1555 ;  
			
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// ���ڽ����ݰ���� 9mS������ 4�� 2mS(����ң����ʱ��)
		// ʵ��Э���в�����Ҫ�˲�����
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case FRSKYD16_TUNE :
		  	if(Cnts < 3)
			{
				++Cnts ; 
			}
			else 
			{
				FRSKYD16Phase = FRSKYD16_DATA ;
			}
			return 2634 ;
		  
		  
	}
	return 0 ; 
}


void SetBind(void)
{
  	FRSKYD16_BindCounts = 0 ; 
	FRSKYD16Phase = FRSKYD16_BIND ;
	FRSKYD16_TuneChannel(FRSKYD16_HOPChannel[FRSKYD16_BINDCHANNEL]) ; 
}

//==============================================================================
//			FRSKYD16 ��ʼ��
//==============================================================================
void initFRSKYD16(void)
{
  	//��ȡ ң���� ID ��
  	TransmitterID = GetUniqueID();

	//ͨ�� ң���� ID �ţ������ 47 ����Ƶ�� 
	Calc_FRSKYD16_Channel();
	
	//ͨ����ͨ�� ADC ��ȡһ�� 1 - 46 ֮��������
	FRSKYD16_ChannelShip = (ADC_Value[0] + ADC_Value[1] + ADC_Value[2] + ADC_Value[3] + ADC_Value[4] + ADC_Value[5] + ADC_Value[6])% 46 + 1 ; 	 // Initialize it to random 0-47 inclusive(ȡ����IO ADC)
	while((FRSKYD16_ChannelShip - FRSKYD16_ctr) % 4) FRSKYD16_ctr = (FRSKYD16_ctr + 1) % 4 ;
	FRSKYD16_CountsRst = (FRSKYD16_ChannelShip - FRSKYD16_ctr) >> 2 ; 
	
	//��ʼ�� CC2500 , ���س�ʼ���Ƿ�ɹ���־λ
	CommunicationError_flg = CC2500_Init() ; 
	if(CommunicationError_flg == true)
	{
	  	//���߳�ʼ��ʧ�ܣ����ϱ���
	  	if(RunStatus < __stRF_err)				//״̬����ǰ��Ҫ�ж�״̬�ȼ����Ƿ����(���򲻸���,����ʾ)
		{
			beepCmd(NormalFreCounts , __stRFModelLostWarning);
			RunStatus = __stRF_err ; 
			LED_State_ON = LED_NONE ; 
			LED_State_Shake = LED_ALL ; //����ͨ��LED��˸
		}
		
		//======================================================
		//��ת�����ϱ���״̬
		//======================================================
		MenuCtrl.RunStep = __stError ;
		MenuCtrl.Sub_RunStep = 0 ; 
	}
	else
	{
		// У׼ ��ͨ��Ƶ��ֵ
		for (uint8_t i = 0 ; i < 48 ; i++)
		{
			CC2500_Strobe(CC2500_SIDLE);
			CC2500_WriteReg(CC2500_0A_CHANNR , FRSKYD16_HOPChannel[i]);
			CC2500_Strobe(CC2500_SCAL);
			DelayUs(1000);
			FRSKYD16_calData[i]  =  CC2500_ReadReg(CC2500_25_FSCAL1);
		}
		
		
		//��ס���밴���ϵ� �������ģʽ������
		if(!(GPIOE -> IDR & (1<<7)))
		{
			Bind_flg = true ;
		}
		
		FRSKYD16Phase = FRSKYD16_DATA ;
		FRSKYD16_TuneChannel(FRSKYD16_HOPChannel[FRSKYD16_Channel_Num]) ; 
	}
	
	
	
}