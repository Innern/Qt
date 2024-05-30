#ifndef COMMONDEF_H
#define COMMONDEF_H
#include <QMetaType>

#define SAFE_DELETE_PTR(p) {if(p) delete p; p=nullptr;}
#define SAFE_DELETE_ARRAY(p) {if(p) delete[] p; p=nullptr;}

#define SCALED_IMAGE_WIDTH 12
#define SCALED_IMAGE_HEIGHT 12

#define NAME_SQL_DATABASE   "YQLServer.db"
#define NAME_TABLE_CONFIG   "settings"

#define INSPECTION_DLL "INSPECTION_DLL"
#define INSPECTION_DLL_ENVTYPE "INSPECTION_DLL_ENVTYPE"
#define INSPECTION_DLL_ERRCODE "INSPECTION_DLL_ERRCODE"

#define PATH_DCCMTEST "D:/DccmTest"
#define PATH_NAME_CAMERA "Camera"
#define PATH_NAME_EXE "CameraTest.exe"
#define PATH_EXTRATOOL "/ExtraTool/"
#define PATH_EXTRATOOL_NAME "ExtraTool.exe"

enum LogLevel{SendLevel,ReceiveLevel,RightLevel,ErrorLevel,NormalLevel,ReservedLevel,WaitLevel,GrayLevel};
Q_DECLARE_METATYPE(LogLevel)

enum SocketState{State_Finished=0,State_Testing,State_Failed,State_FailedAndModuleClose};

#ifndef TEST_ENVIRONMENT
#define TET_FINAL                   0x0103 //Final
#define TET_LIGHT_CHANNEL1          0x0900 //OTP光源通道1
#define TET_LIGHT_CHANNEL2          0x0901 //OTP光源通道2
#define TET_LIGHT_CHANNEL3          0x0902 //OTP光源通道3
#define TET_LIGHT_CHANNEL4          0x0903 //OTP光源通道4
#define TET_LIGHT_CHANNEL5          0x0904 //OTP光源通道5
#define TET_LIGHT_CHANNEL6          0x0905 //OTP光源通道6

#endif  // TEST_ENVIRONMENT

#ifndef SOCKET_RESULT

#define RESULT_INSPECTION_NULL				5100// + itemID = 1~99 漏测
#define RESULT_INSPECTION_OK				5200// + itemID = 1~99 OK
#define RESULT_INSPECTION_NG				5300// + itemID = 1~99 NG
#define RESULT_INSPECTION_END				5400


//眼千里软件通用错误代码

//运动控制
#define ERROR_CLINDER_MOVE                  1000		//汽缸运动失败
#define ERROR_LIGHTSOURCE_OPEN              1001		//光源打开失败
#define ERROR_PR                            1002		//PR失败
#define ERROR_LASER                         1003		//镭射测高失败
#define ERROR_MOVE                          1004		//运动错误
#define ERROR_COMMUNICATION_TIMEOUT         1005		//软件通信超时
#define ERROR_MOVE_TIMEOUT                  1006		//运动超时
#define ERROR_SOCKET_OPEN					1007		// 治具处于打开状态

//测试盒
#define ERROR_FIND_BOARDCARD                1101		//找不到工装
#define ERROR_I2C_COMMUNICATION             1102		//I2C通信异常
#define ERROR_FIND_CAMERAMODULE             1103		//找不到模组
#define ERROR_FUSEID                        1104		//Fuseid未读取或读取失败
#define ERROR_CHECK_SENSOR_FLAG		        1105        //Sensor Flag Reg ID错误
#define ERROR_CAMERA_PARAMETER_INIT_FAILED	1106        //初始化参数失败
#define ERROR_STARTPROCESS_FAILED			1107        //抓帧显示线程开启失败
#define ERROR_INSPECTION_INIT_FAILED		1108		//初始化测试项执行结果ng
#define ERROR_DRIVERIC_INIT_FAILED			1109        //DriverIC初始化失败
#define ERROR_CHECK_CAMID_FAILED			1110        //CamID错误
#define ERROR_MEMORY_ALLOC_FAILED			1111        //内存分配失败
#define ERROR_CAMERA_INIT_FAILED			1112		// 启动失败
#define ERROR_NO_LOAD_CONFIG				1113		// 没有加载参数
#define ERROR_GRAB_FRAME					1114		// 采集超时或者内部错误
#define ERROR_FAIL_FRAME_LIMIT_CNT			1115	    // 错误帧数量超出限制
#define ERROR_SOFT_PIN_PULL_UP				1116	    // 拉高软Pin失败
#define ERROR_SENSOR_ENABLE_FAIL			1117	    // 使能Sensor失败
#define ERROR_SN_NUMBER						1118	    // 设备SN号不正确
#define ERROR_CAM_LOST_CONNECTED			1119	    // 模组失去连接

//2019.8.26
#define ERROR_CAM_STANDBY                   1120        // 模组待机状态
#define ERROR_CAM_RUNNING                   1121        // 模组运行状态
#define ERROR_INSP_RUNNING                  1122        // 测试项测试中
#define ERROR_INSP_EXIST		            1123	    // 测试项存在
#define ERROR_INSP_NO_EXIST		            1124	    // 测试项不存在

#define RESULT_PLAY_PAUSE                   1194        // 休眠
#define RESULT_STOP_NULL					1195	    // 没关闭
#define RESULT_STOP_NG						1196	    // 关闭失败
#define RESULT_STOP_OK						1197	    // 关闭成功
#define RESULT_PLAY_NULL					1198	    // 没启动
#define RESULT_PLAY_NG						1199	    // 启动失败
#define RESULT_PLAY_OK						1200	    // 启动成功

//开短路
#define ERROR_OPENSHORT                     1201		//开短路NG

//工作电流
#define ERROR_WORKING_CURRENT               1301		//工作电流NG

//待机电流
#define ERROR_STANDBY_CURRENT               1401		//待机电流NG

//曝光
#define ERROR_EXPOSURE                      1501		//曝光NG
#define ERROR_READ_EXP_I2C					1502		//读取曝光寄存器失败
#define ERROR_CAPTURE_MARK                  1503        //抓取mark点失败

//POD
#define ERROR_POD                           1601        //POD NG
#define ERROR_DARK_PIXEL                    1602		//黑点NG
#define ERROR_BRIGHT_PIXEL                  1603		//白点NG
#define ERROR_DARK_PIXEL_CENTER             1604		//黑点中心NG
#define ERROR_DARK_PIXEL_EDGE               1605		//黑点边缘NG
#define ERROR_DARK_PIXEL_CORNER             1606		//黑点四角NG
#define ERROR_BRIGHT_PIXEL_CENTER           1607		//白点中心NG
#define ERROR_BRIGHT_PIXEL_EDGE             1608		//白点边缘NG
#define ERROR_BRIGHT_PIXEL_COENER           1609		//白点四角NG

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

//光心
#define ERROR_OPTICAL_CENTER                2201		//Optical Center NG

//亮坏点
#define ERROR_DEFECT_PIXEL_DARK_MODE        2301		//Darkdefectpixel NG
#define ERROR_BRIGHT_PIXEL_DARK_MODE        2302        //黑场亮点
#define ERROR_DEFECT_PIXEL_BRIGHT_MODE      2303        //白场坏点

//坏线
#define ERROR_DEAD_LINE_DARK_MODE           2401		//DarkModeDeadLine NG
#define ERROR_DEAD_LINE_BRIGHT_MODE         2402        //BrightModeDeadLine NG

//OTP
#define ERROR_OTP                           2501		//OTP NG
#define ERROR_AWB                           2502		//AWB  apply前NG
#define ERROR_LSC                           2503		//LSC  apply前NG
#define ERROR_AWB_APPLY                     2504		//AWB  apply后NG
#define ERROR_LSC_APPLY                     2505		//LSC  apply后NG

//Focusing
#define ERROR_FOCUSING                      2601		//Focusing NG
#define ERROR_FOCUSING_CENTER               2602		//Focusing 中心
#define ERROR_FOCUSING_CORNER               2603		//Focusing 四角

//AF
#define ERROR_AF_FAR                        2701		//Auto Focusingfar NG
#define ERROR_AF_FAR_CENTER                 2702		//Auto Focusingfar 中心
#define ERROR_AF_FAR_CORNER                 2703		//Auto Focusingfar 四角
#define ERROR_AF_FAR_CODE                   2704		//Auto Focusingfar Code
#define ERROR_AF_NEAR                       2705		//Auto FocusingMacro NG
#define ERROR_AF_NEAR_CENTER                2706		//Auto FocusingMacro 中心
#define ERROR_AF_NEAR_CORNER                2707		//Auto FocusingMacro 四角
#define ERROR_AF_NEAR_CODE                  2708		//Auto FocusingMacro Code
#define ERROR_AF_MIDDLE                     2709		//Auto FocusingMiddle NG
#define ERROR_AF_MIDDLE_CENTER              2710		//Auto FocusingMiddle 中心
#define ERROR_AF_MIDDLE_CORNER              2711        //Auto FocusingMiddle 四角
#define ERROR_AF_MIDDLE_CODE                2712        //Auto FocusingMiddle Code
#define ERROR_HYSTERESIS                    2713        //磁滞NG

//MTF
#define ERROR_MTF                           2801		//MTF NG
#define ERROR_MTF_CENTER                    2802		//MTF中心
#define ERROR_MTF_CORNER                    2803		//MTF四角

//SFR
#define ERROR_SFR                           2851        //SFR
#define ERROR_SFR_CENTER                    2852        //SFR中心
#define ERROR_SFR_CORNER                    2853        //SFR四角

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

//点胶
#define ERROR_DISPENSER                     3201		//点胶
#define ERROR_DISPENSER_PR                  3202        //点胶PR失败
#define ERROR_UV_CURING                     3203        //UV固化失败

//数据库上传
#define ERROR_UPLOAD_DB                     3301	 //数据库上传失败
#define ERROR_WORK_POSITION                 3302     //跳站测试
#define ERROR_DB_UPLOAD_FAIL                3303     //数据库上传失败
#define ERROR_TIMEOUT_DB                    3304     //数据库操作超时
#define ERROR_COMM_DB                       3305     //数据库通信错误
#define ERROR_BIND_FAIL                     3306     //绑定失败

//BYD MES ERR
#define ERROR_3307                          3307

#endif  // SOCKET_RESULT


#endif // COMMONDEF_H
