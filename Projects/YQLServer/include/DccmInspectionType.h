#pragma once
#define ENV_NUM 42
// 测试环境类型	TEST_ENV_TYPE
#define TET_NULL					0x0000 // 无定义

// 0x1nn Sensor初始化
#define TET_INIT					0x0100
#define TET_INIT_AFTER_POWER_ON		0x0101 //用于SENSOR 上电后，Streaming前初始化
#define TET_INIT_AFTER_STREAM		0x0102 //用于SENSOR Streaming后初始化
#define TET_FINAL                   0x0103 //Final
// 0x2nn 远景
#define TET_INF						0x0200 //远景，适用于只分站位不分距离的情况
#define TET_INF_1M					0x0201
#define TET_INF_5M					0x0202
#define TET_INF_10M					0x0203

// 0x3nn 中景
#define TET_MID						0x0300 //中景，适用于只分站位不分距离的情况
#define TET_MID_15CM				0x0301
#define TET_MID_20CM				0x0302
#define TET_MID_30CM				0x0303
#define TET_MID_50CM				0x0304

// 0x4nn 近景
#define TET_MAC						0x0400 //近景，适用于只分站位不分距离的情况
#define TET_MAC_7CM					0x0401
#define TET_MAC_8CM					0x0402
#define TET_MAC_10CM				0x0403

// 0x5nn 白场
#define TET_WHITE					0x0500 //白场，适用于只分站位不分距离的情况
#define TET_WHITE_1CM				0x0501
#define TET_WHITE_2CM				0x0502

// 0x6nn 黑场
#define TET_BLACK					0x0600 //黑场，适用于只分站位不分距离的情况


// 0x7nn 特殊测试项目
#define TET_FOCUS					0x0700 //调焦
#define TET_FOCUS_PR                0x0701 //调焦PR位置测试
#define TET_DISPENSE_PR             0x0702 //点胶PR位置测试
#define TET_DISPENSE                0x0703 //点胶位置测试
#define TET_UV                      0x0704 //UV固化位置测试
#define TET_PLAY_CAMERA             0x0705 //上下料位置测试（点亮模组之后）

//0x8nn
#define TET_CHARTALIGN              0x0800 //MOVING CHART

//0x9nn
#define TET_LIGHT_CHANNEL1          0x0900 //OTP光源通道1
#define TET_LIGHT_CHANNEL2          0x0901 //OTP光源通道2
#define TET_LIGHT_CHANNEL3          0x0902 //OTP光源通道3
#define TET_LIGHT_CHANNEL4          0x0903 //OTP光源通道4
#define TET_LIGHT_CHANNEL5          0x0904 //OTP光源通道5
#define TET_LIGHT_CHANNEL6          0x0905 //OTP光源通道6

//0xann
#define TET_OTPCHECK_CHANNEL1          0x0a00 //OTP光源通道1
#define TET_OTPCHECK_CHANNEL2          0x0a01 //OTP光源通道2
#define TET_OTPCHECK_CHANNEL3          0x0a02 //OTP光源通道3
#define TET_OTPCHECK_CHANNEL4          0x0a03 //OTP光源通道4
#define TET_OTPCHECK_CHANNEL5          0x0a04 //OTP光源通道5
#define TET_OTPCHECK_CHANNEL6          0x0a05 //OTP光源通道6

#define TET_ALL						10		// 总共9个大类

