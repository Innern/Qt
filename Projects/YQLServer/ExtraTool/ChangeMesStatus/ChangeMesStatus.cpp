// ChangeMesStatus.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DTCCM2_SDK/dtccm2.h"
#pragma comment(lib,"DTCCM2_SDK/dtccm2.lib")

bool SwitchChannelForG4C(int com, int channel);
bool SwitchChannelForHuizhao(int com, int channel);

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2)
	{
		printf("Input argument Error.\r\n");
		return -1;  // -1 ��������
	}
	for(int i=0;i<argc;++i)
	{
		printf(" %S",argv[i]);
	}
	printf(".\r\n");
	int op = _ttoi(argv[1]);
	int ret=-1;
	if(0 == op)
	{
		// exe op dccf mes_status
		WritePrivateProfileString(L"DataSavingTab",L"SaveToSQL_En",argv[3],argv[2]);
	}
	else if(1 == op)
	{
		// ��ȡ���Ų��Ժ�
		int DeviceNum = 0;
		char *DeviceName[4] = {NULL};
		int m_nDevID=0;

		int i = 0;
		ret = EnumerateDevice(DeviceName,4,&DeviceNum);
		if(DT_ERROR_OK == ret)
		{
			BYTE  TempDeviceSN[512] = {0};
			int RetLen=0;
			for(int i=0;i<DeviceNum;++i)
			{
				//printf("%s\r\n",DeviceName[i]);
				ret = OpenDevice(DeviceName[i],&m_nDevID);
				if ( ret == DT_ERROR_OK)
				{
					memset(TempDeviceSN,0,sizeof(TempDeviceSN));
					ret = GetDeviceSN(TempDeviceSN,512,&RetLen,m_nDevID);
					CloseDevice(m_nDevID);
					if(DT_ERROR_OK == ret)
					{
						printf("SN_%d:%s.\n",i,TempDeviceSN);
					}
					else
					{
						printf("GetDeviceSN FAIL.\r\n");
						break;
					}
				}
				else
				{
					printf("open Device Fail.\r\n");
					break;
				}
			}
		}
		
		for (int i=0; i<DeviceNum; i++)
			if (DeviceName[i] != NULL)
				GlobalFree(DeviceName[i]);//�ͷ�DeviceName
	}
	else if(2 == op)
	{
		// �����б�exe light_type com channel
		if (argc != 5)
		{
			printf("Please input arguments: exe light_type com channel.\r\n");
			return -1;
		}
		int light_type = _ttoi(argv[2]);
		int com = _ttoi(argv[3]);
		int channel = _ttoi(argv[4]);
		printf("Current light_type=%d, com=%d, channel=%d.\r\n", light_type, com, channel);
		bool bret = false;
		switch (light_type)
		{
		case 0:  // G4C
			bret = SwitchChannelForG4C(com, channel);
			break;
		case 1:  // ����
			printf("��δ��ӻ��չ�Դ�й⹦��.\r\n");
			//bret = SwitchChannelForHuizhao(com, channel);
			break;
		default:
			printf("Please set correction light_type : 0-G4C 1-���չ�Դ.\r\n");
			return -2;
		}

		if (!bret)
		{
			printf("Switch light source FAIL.\r\n");
			return -3;
		}
		else
		{
			printf("Switch light source PASS.\r\n");
		}

		ret = 0;
	}
	return ret;
}

bool SwitchChannelForG4C(int com, int channel)
{
	bool ret = true;
	channel = channel + TET_LIGHT_CHANNEL1;
	switch (channel)
	{
	case TET_LIGHT_CHANNEL1:
		ret = CSC600SendCMD(com, "4C 47 00 00 00 93");
		break;
	case TET_LIGHT_CHANNEL2:
		ret = CSC600SendCMD(com, "4C 47 01 00 00 94");
		break;
	case TET_LIGHT_CHANNEL3:
		ret = CSC600SendCMD(com, "4C 47 02 00 00 95");
		break;
	case TET_LIGHT_CHANNEL4:
		ret = CSC600SendCMD(com, "4C 47 03 00 00 96");
		break;
	case TET_LIGHT_CHANNEL5:
		ret = CSC600SendCMD(com, "4C 47 04 00 00 97");
		break;
	case TET_LIGHT_CHANNEL6:
		ret = CSC600SendCMD(com, "4C 47 05 00 00 98");
		break;
	case TET_LIGHT_CHANNEL7:
		ret = CSC600SendCMD(com, "4C 47 06 00 00 99");
		break;
	case TET_LIGHT_CHANNEL8:
		ret = CSC600SendCMD(com, "4C 47 07 00 00 9A");
		break;
	default:
		printf("δ֪�Ĺ�Դͨ��.\r\n");
		ret = false;
		break;
	}

	return ret;
}

bool SwitchChannelForHuizhao(int com, int channel)
{
	bool bret = false;
	/*CCLightCTL m_LightCTL;
	bool Res = m_LightCTL.Init();

	if (!Res)
	{
	printf("���ڳ�ʼ��ʧ��\r\n");
	return false;
	}
	printf("nChangle %d", channel);
	bret = m_LightCTL.SetLightSource(channel - 1);*/

	return bret;
}
