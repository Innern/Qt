#pragma once
#define ENV_NUM 42
// ���Ի�������	TEST_ENV_TYPE
#define TET_NULL					0x0000 // �޶���

// 0x1nn Sensor��ʼ��
#define TET_INIT					0x0100
#define TET_INIT_AFTER_POWER_ON		0x0101 //����SENSOR �ϵ��Streamingǰ��ʼ��
#define TET_INIT_AFTER_STREAM		0x0102 //����SENSOR Streaming���ʼ��
#define TET_FINAL                   0x0103 //Final
// 0x2nn Զ��
#define TET_INF						0x0200 //Զ����������ֻ��վλ���־�������
#define TET_INF_1M					0x0201
#define TET_INF_5M					0x0202
#define TET_INF_10M					0x0203

// 0x3nn �о�
#define TET_MID						0x0300 //�о���������ֻ��վλ���־�������
#define TET_MID_15CM				0x0301
#define TET_MID_20CM				0x0302
#define TET_MID_30CM				0x0303
#define TET_MID_50CM				0x0304

// 0x4nn ����
#define TET_MAC						0x0400 //������������ֻ��վλ���־�������
#define TET_MAC_7CM					0x0401
#define TET_MAC_8CM					0x0402
#define TET_MAC_10CM				0x0403

// 0x5nn �׳�
#define TET_WHITE					0x0500 //�׳���������ֻ��վλ���־�������
#define TET_WHITE_1CM				0x0501
#define TET_WHITE_2CM				0x0502

// 0x6nn �ڳ�
#define TET_BLACK					0x0600 //�ڳ���������ֻ��վλ���־�������


// 0x7nn ���������Ŀ
#define TET_FOCUS					0x0700 //����
#define TET_FOCUS_PR                0x0701 //����PRλ�ò���
#define TET_DISPENSE_PR             0x0702 //�㽺PRλ�ò���
#define TET_DISPENSE                0x0703 //�㽺λ�ò���
#define TET_UV                      0x0704 //UV�̻�λ�ò���
#define TET_PLAY_CAMERA             0x0705 //������λ�ò��ԣ�����ģ��֮��

//0x8nn
#define TET_CHARTALIGN              0x0800 //MOVING CHART

//0x9nn
#define TET_LIGHT_CHANNEL1          0x0900 //OTP��Դͨ��1
#define TET_LIGHT_CHANNEL2          0x0901 //OTP��Դͨ��2
#define TET_LIGHT_CHANNEL3          0x0902 //OTP��Դͨ��3
#define TET_LIGHT_CHANNEL4          0x0903 //OTP��Դͨ��4
#define TET_LIGHT_CHANNEL5          0x0904 //OTP��Դͨ��5
#define TET_LIGHT_CHANNEL6          0x0905 //OTP��Դͨ��6

//0xann
#define TET_OTPCHECK_CHANNEL1          0x0a00 //OTP��Դͨ��1
#define TET_OTPCHECK_CHANNEL2          0x0a01 //OTP��Դͨ��2
#define TET_OTPCHECK_CHANNEL3          0x0a02 //OTP��Դͨ��3
#define TET_OTPCHECK_CHANNEL4          0x0a03 //OTP��Դͨ��4
#define TET_OTPCHECK_CHANNEL5          0x0a04 //OTP��Դͨ��5
#define TET_OTPCHECK_CHANNEL6          0x0a05 //OTP��Դͨ��6

#define TET_ALL						10		// �ܹ�9������

