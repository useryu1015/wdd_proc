/**
 * @file dataDef.h
 * @author miao
 * @version 1.0
 * @par Copyright (c): catNofish
 * @date 2022年8月26日
 * @brief
 * @details
 * @par History:
 * 		V1.0: miao, 2022年8月26日,创建
 */
#ifndef INC_DATADEF_H_
#define INC_DATADEF_H_

#include <stdint.h>
#include <time.h>

#define AI_NUM_MAX				32u
#define AO_NUM_MAX				32u
#define DI_NUM_MAX				32u
#define DO_NUM_MAX				32u
#define LED_NUM_MAX				16u

/**
 * 外设初始化状态：硬件是否可选
 *  fix： 会不会在程序运行时，添加通道？  如：是否支持设备热插拔
 *  保持
*/
typedef struct RTU_INFO
{
	char type[32];
	char softwareVer[16];
	char hardwareVer[16];
	uint16_t aiChNumI;		/*!< AI:电流通道数量 */
	uint16_t aiChNumV;		/*!< AI:电压通道数量 */
	uint16_t diChNum;		/*!< ＤI:通道数量 */
	struct envMonitorSet
	{
		uint16_t isInsTemperature;		/*!< 是否安装温度 */
		uint16_t isInsHumidity;			/*!< 是否安装湿度 */
		uint16_t isInsNoise;			/*!< 是否安装噪声 */
		uint16_t isInsPm2_5;			/*!< 是否安装PM2.5 */
		uint16_t isInsPm10;				/*!< 是否安装PM10 */
		uint16_t isInsO2;				/*!< 是否安装氧气浓度 */
		uint16_t isInsTvoc;				/*!< 是否安装TVOC */
		uint16_t rsv[9];
	}envMonitorSet;
// } RTU_INFO, *pRTU_INFO;
}__attribute__((packed)) RTU_INFO, *pRTU_INFO;

/**
 * 定义硬件参数
 *  只读
*/
typedef struct SHM_DATA_RO
{
	uint16_t run;
	uint16_t err;
	struct timeval updateTime;			/*!< 数据更新时间 */
	float aiValI[AI_NUM_MAX];			/*!< AI:电流数据,单位ｍＡ */
	float aiValV[AI_NUM_MAX];			/*!< AI:电压数据,单位Ｖ */
	float diVal[DI_NUM_MAX];			/*!< ＤI: 待定 */
	struct envMonitor
	{
		float temperature;			/*!< 温度,单位℃ */
		float humidity;				/*!< 湿度,单位％ */
		float noise;				/*!< 噪声,单位db */
		float pm2_5;				/*!< pm2.5,ug/m3 */
		float pm10;					/*!< pm10,ug/m3 */
		float O2;					/*!< 氧气,单位% */
		float tvoc;					/*!< tvoc,单位mg/m3 */
		float rsv[9];
	}envMonitor;
}__attribute__((packed)) SHM_DATA_RO, *pSHM_DATA_RO;

/*!
 *  @brief
 */
typedef struct SHM_DATA_RW
{
	uint16_t rsv;			// fix: 非空 or 交换set与info顺序。
}__attribute__((packed)) SHM_DATA_RW, *pSHM_DATA_RW;

/*!
 * @brief
 */
typedef struct SHM_DATA_DEF
{
	SHM_DATA_RO data;
	SHM_DATA_RW set;
	RTU_INFO info;
}__attribute__((packed)) SHM_DATA_DEF, *pSHM_DATA_DEF;

extern pSHM_DATA_DEF shmData;


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* INC_DATADEF_H_ */
