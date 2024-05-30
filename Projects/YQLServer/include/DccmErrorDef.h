#pragma once

//HLT MES ERRCODE  add by zc 2018.10.30
#define ERROR_SYS_ERR                       9000  //MESϵͳ�쳣");
#define ERROR_FACTORY_CODE_EMPTY            9001  //�������벻��Ϊ��");
#define ERROR_PASSCODE_EMPTY                9002  //���벻��Ϊ��");
#define ERROR_SIGN_ERROR                    9003  //ǩ������");
#define ERROR_SOFTWARE_VER_EMPTY            9004  //����汾�Ų���Ϊ��");
#define ERROR_MACHINE_NO_EMPTY              9005  //�豸��Ų���Ϊ��");
#define ERROR_MACHINE_NOUSE                 9006  //�豸��ͣ��");
#define ERROR_SOFTWARE_VER_ERR              9007  //����汾����");
#define ERROR_MACHINE_INFO_NOT_FIND         9008  //δ�ҵ��豸��Ϣ");
#define ERROR_MES_CONNECT_FAIL              9009  //MES����ʧ��");
#define ERROR_SIGN_EMPTY                    9010  //ǩ������Ϊ��");
#define ERROR_BATCH_NO_EMPTY                9011  //���κŲ���Ϊ��");
#define ERROR_MODULE_PLAY_FAIL              9012  //ģ��δ����");
#define ERROR_STATUS_EMPTY                  9013  //�����ʶ����Ϊ��");
#define ERROR_STATUS_ERROR                  9014  //�����ʶ��ʽ����ȷ");
#define ERROR_SN_EMPTY                      9015  //SN����Ϊ��");
#define ERROR_PSCODE_EMPTY                  9016  //������벻��Ϊ��");

#define ERROR_BATCH_NO_ERR                  9017  //���κŴ���");
#define ERROR_PSCODE_NO_EXIT                9018  //���������ڸù���");
#define ERROR_LAST_STATION_FAIL             9019  //��һվFail");//XXX����ΪNG
#define ERROR_NOT_BELONG_BATCH              9020  //�����ڸ����β�Ʒ����δ�����");
#define ERROR_SKIP_STATION                  9021  //��һվ©��");//���������վ
#define ERROR_STATION_PASS                  9022  //��վ��PASS");
#define ERROR_SHAKE                         9023  //�𶯹��������,�������");
#define ERROR_LAST_PS_NOT_TRANSMIT          9024  //��һվ����δת��");

#define ERROR_SECRET_ERR                    9025  //secret����");
#define ERROR_ALREADY_BIND                  9026  //�˲�Ʒ�Ѱ�OK");
#define ERROR_UPLOAD_DATA_EMPTY             9027  //�����ϴ����Ϊ��");
#define ERROR_PS_NOT_BY_PCS                 9028  //��ǰ������By Pcs��վ");
#define ERROR_SN_FORMAT_ERR                 9029  //SN�������������");
#define ERROR_SN_ID_BIND_ERR                9030  //�󶨵�SN��SensorID��һ��");
#define ERROR_BATCH_FULL                    9031  //������������");

#define ERROR_QRCODE_USED                   9032  //��ά���ظ�");//��ά���ѱ�������Ʒ��
#define ERROR_PSCODE_ERR                    9033  //�������ô���");
#define ERROR_PS_MUST_BIND                  9034  //������������");
#define ERROR_NOT_BIND                      9035  //δ������");
#define ERROR_MODULE_NOT_BELONG_SHEET       9036  //��Ʒ�����ڸ÷�����");
#define ERROR_LAST_STATION_NOT_UPLOAD       9037  //��һվ����δ�ϴ�");



#define RESULT_INSPECTION_NULL				5100// + itemID = 1~99 ©��
#define RESULT_INSPECTION_OK				5200// + itemID = 1~99 OK
#define RESULT_INSPECTION_NG				5300// + itemID = 1~99 NG
#define RESULT_INSPECTION_END				5400

// ���ս��

#define RESULT_FINAL_PASS					5000	    // PASS
#define RESULT_FINAL_FAIL					5001	    // FAIL
#define RESULT_FINAL_NULL					5002	    // û���


//��ǧ�����ͨ�ô������

//�˶�����
#define ERROR_CLINDER_MOVE                  1000		//�����˶�ʧ��			
#define ERROR_LIGHTSOURCE_OPEN              1001		//��Դ��ʧ��			
#define ERROR_PR                            1002		//PRʧ��			
#define ERROR_LASER                         1003		//������ʧ��			
#define ERROR_MOVE                          1004		//�˶�����			
#define ERROR_COMMUNICATION_TIMEOUT         1005		//���ͨ�ų�ʱ			
#define ERROR_MOVE_TIMEOUT                  1006		//�˶���ʱ		
#define ERROR_SOCKET_OPEN					1007		// �ξߴ��ڴ�״̬

//���Ժ�
#define ERROR_FIND_BOARDCARD                1101		//�Ҳ�����װ			
#define ERROR_I2C_COMMUNICATION             1102		//I2Cͨ���쳣			
#define ERROR_FIND_CAMERAMODULE             1103		//�Ҳ���ģ��			
#define ERROR_FUSEID                        1104		//Fuseidδ��ȡ���ȡʧ��	
#define ERROR_CHECK_SENSOR_FLAG		        1105        //Sensor Flag Reg ID����
#define ERROR_CAMERA_PARAMETER_INIT_FAILED	1106        //��ʼ������ʧ��
#define ERROR_STARTPROCESS_FAILED			1107        //ץ֡��ʾ�߳̿���ʧ��
#define ERROR_INSPECTION_INIT_FAILED		1108		//��ʼ��������ִ�н��ng
#define ERROR_DRIVERIC_INIT_FAILED			1109        //DriverIC��ʼ��ʧ��
#define ERROR_CHECK_CAMID_FAILED			1110        //CamID����
#define ERROR_MEMORY_ALLOC_FAILED			1111        //�ڴ����ʧ��
#define ERROR_CAMERA_INIT_FAILED			1112		// ����ʧ��
#define ERROR_NO_LOAD_CONFIG				1113		// û�м��ز���
#define ERROR_GRAB_FRAME					1114		// �ɼ���ʱ�����ڲ�����
#define ERROR_FAIL_FRAME_LIMIT_CNT			1115	    // ����֡������������
#define ERROR_SOFT_PIN_PULL_UP				1116	    // ������Pinʧ��
#define ERROR_SENSOR_ENABLE_FAIL			1117	    // ʹ��Sensorʧ��
#define ERROR_SN_NUMBER						1118	    // �豸SN�Ų���ȷ
#define ERROR_CAM_LOST_CONNECTED			1119	    // ģ��ʧȥ����

//2019.8.26
#define ERROR_CAM_STANDBY                   1120        // ģ�����״̬
#define ERROR_CAM_RUNNING                   1121        // ģ������״̬
#define ERROR_INSP_RUNNING                  1122        // �����������
#define ERROR_INSP_EXIST		            1123	    // ���������
#define ERROR_INSP_NO_EXIST		            1124	    // ���������

#define RESULT_PLAY_PAUSE                   1194        // ����
#define RESULT_STOP_NULL					1195	    // û�ر�
#define RESULT_STOP_NG						1196	    // �ر�ʧ��
#define RESULT_STOP_OK						1197	    // �رճɹ�
#define RESULT_PLAY_NULL					1198	    // û����
#define RESULT_PLAY_NG						1199	    // ����ʧ��
#define RESULT_PLAY_OK						1200	    // �����ɹ�

//����·		
#define ERROR_OPENSHORT                     1201		//����·NG	

//��������			
#define ERROR_WORKING_CURRENT               1301		//��������NG	

//��������			
#define ERROR_STANDBY_CURRENT               1401		//��������NG		

//�ع�			
#define ERROR_EXPOSURE                      1501		//�ع�NG		
#define ERROR_READ_EXP_I2C					1502		//��ȡ�ع�Ĵ���ʧ��
#define ERROR_CAPTURE_MARK                  1503        //ץȡmark��ʧ��

//POD		
#define ERROR_POD                           1601        //POD NG
#define ERROR_DARK_PIXEL                    1602		//�ڵ�NG			
#define ERROR_BRIGHT_PIXEL                  1603		//�׵�NG			
#define ERROR_DARK_PIXEL_CENTER             1604		//�ڵ�����NG			
#define ERROR_DARK_PIXEL_EDGE               1605		//�ڵ��ԵNG			
#define ERROR_DARK_PIXEL_CORNER             1606		//�ڵ��Ľ�NG			
#define ERROR_BRIGHT_PIXEL_CENTER           1607		//�׵�����NG			
#define ERROR_BRIGHT_PIXEL_EDGE             1608		//�׵��ԵNG			
#define ERROR_BRIGHT_PIXEL_COENER           1609		//�׵��Ľ�NG			

//POG			
#define ERROR_POG                           1701		//POG NG		

//YShading			
#define ERROR_YSHADING                      1801		//YShading NG	
#define ERROR_SATURATION                    1802        //Saturation

//Color Shading			
#define ERROR_COLORSHADING                  1901		//colorshading NG		

//RU			
#define ERROR_RU                            2001		//RU NG		

//RI			
#define ERROR_RI                            2101		//RI NG			

//����			
#define ERROR_OPTICAL_CENTER                2201		//Optical Center NG			

//������			
#define ERROR_DEFECT_PIXEL_DARK_MODE        2301		//Darkdefectpixel NG	
#define ERROR_BRIGHT_PIXEL_DARK_MODE        2302        //�ڳ�����
#define ERROR_DEFECT_PIXEL_BRIGHT_MODE      2303        //�׳�����

//����			
#define ERROR_DEAD_LINE_DARK_MODE           2401		//DarkModeDeadLine NG	
#define ERROR_DEAD_LINE_BRIGHT_MODE         2402        //BrightModeDeadLine NG

//OTP			
#define ERROR_OTP                           2501		//OTP NG 			
#define ERROR_AWB                           2502		//AWB  applyǰNG			
#define ERROR_LSC                           2503		//LSC  applyǰNG			
#define ERROR_AWB_APPLY                     2504		//AWB  apply��NG			
#define ERROR_LSC_APPLY                     2505		//LSC  apply��NG			

//Focusing			
#define ERROR_FOCUSING                      2601		//Focusing NG			
#define ERROR_FOCUSING_CENTER               2602		//Focusing ����			
#define ERROR_FOCUSING_CORNER               2603		//Focusing �Ľ�		

//AF			
#define ERROR_AF_FAR                        2701		//Auto Focusingfar NG			
#define ERROR_AF_FAR_CENTER                 2702		//Auto Focusingfar ����			
#define ERROR_AF_FAR_CORNER                 2703		//Auto Focusingfar �Ľ�			
#define ERROR_AF_FAR_CODE                   2704		//Auto Focusingfar Code			
#define ERROR_AF_NEAR                       2705		//Auto FocusingMacro NG			
#define ERROR_AF_NEAR_CENTER                2706		//Auto FocusingMacro ����			
#define ERROR_AF_NEAR_CORNER                2707		//Auto FocusingMacro �Ľ�			
#define ERROR_AF_NEAR_CODE                  2708		//Auto FocusingMacro Code			
#define ERROR_AF_MIDDLE                     2709		//Auto FocusingMiddle NG			
#define ERROR_AF_MIDDLE_CENTER              2710		//Auto FocusingMiddle ����	
#define ERROR_AF_MIDDLE_CORNER              2711        //Auto FocusingMiddle �Ľ�
#define ERROR_AF_MIDDLE_CODE                2712        //Auto FocusingMiddle Code	
#define ERROR_HYSTERESIS                    2713        //����NG

//MTF			
#define ERROR_MTF                           2801		//MTF NG			
#define ERROR_MTF_CENTER                    2802		//MTF����			
#define ERROR_MTF_CORNER                    2803		//MTF�Ľ�

//SFR
#define ERROR_SFR                           2851        //SFR
#define ERROR_SFR_CENTER                    2852        //SFR����
#define ERROR_SFR_CORNER                    2853        //SFR�Ľ�

//PDAF2			
#define ERROR_PDAF2                         2901		//PDAF2 NG	

//PDAF3			
#define ERROR_PDAF3                         3001		//PDAF3 NG			

//OTP Check			
#define ERROR_OTPCHECK                      3101		//OTP Check NG			
#define ERROR_OTPCHECK_SUM                  3102		//Check SUM			
#define ERROR_OTPCHECK_FLAG                 3103		//Check FLAG			
#define ERROR_OTPCHECK_AFCODE               3104		//Check AFCODE			
#define ERROR_OTP_CHECK_AWB                 3105		//Check AWB			

//�㽺			
#define ERROR_DISPENSER                     3201		//�㽺
#define ERROR_DISPENSER_PR                  3202        //�㽺PRʧ��
#define ERROR_UV_CURING                     3203        //UV�̻�ʧ��

//���ݿ��ϴ�			
#define ERROR_UPLOAD_DB                     3301	 //���ݿ��ϴ�ʧ��
#define ERROR_WORK_POSITION                 3302     //��վ����
#define ERROR_DB_UPLOAD_FAIL                3303     //���ݿ��ϴ�ʧ��
#define ERROR_TIMEOUT_DB                    3304     //���ݿ������ʱ
#define ERROR_COMM_DB                       3305     //���ݿ�ͨ�Ŵ���
#define ERROR_BIND_FAIL                     3306     //��ʧ��

//BYD MES ERR
#define ERROR_3307                          3307     

//#define ERROR_    3400		//Reserve			
