#ifndef DEFINES_H_
#define DEFINES_H_

#include <string>
#include <list>
#include <map>

#include <stdint.h>

using namespace std;

#define INT64	unsigned long long int
#define	INT32	int
#define UINT32	unsigned int
#define	INT16	short int
#define INT8 	char


#define SUCCESS	0
#define FALSE	-1

#define MINI_PACKAGE_LEN_LIMIT	12
#define MAX_PROCESS_PTHREAD_CNT	10

#define BUF_SIZE			1024
#define MINI_BUF_SIZE		128

#define LIMIT_KEEPALIVE		3
#define MAX_PROCESS_PTHREAD_CNT 10

#define DISPATCH_TASK_NUM	3

#define PROTO_HEAD_ID_LEN	8
#define MSG_ID_MAX_LIMIT	60000

#define MSG_HEAD_STR		0xFE
#define MSG_HEAD_LEN_IDENT	1
#define MSG_DATA_LEN_IDENT	3

#define MSG_MINI_LEN		3
#define GET_REDIS_CONNECTER_LOOP_LIMIT 1024

#define DEFAULT_POLICY_THRESHOLD	POLICY_BLOCK
#define REDIS_FREQ_VALUE_AGE	(24*3600)
#define LOGSERVER_LIST_LIMIT	100


typedef enum{
	STATUS_INIT,
	STATUS_USING,
	STATUS_NOUSE,
	STATUS_DESTROY
}CONNECT_STATUS;

typedef struct {
	string strIp;
	int portRev;
	int portData;
}NetConfig;

typedef struct{
	string	strServerIP;
	int 	port;
	string	strDbName;
	string	strUsername;
	string	strPasswd;
	string	strCharset;
	string	strErrMsg;
}DB_Config;

typedef struct{
	DB_Config dbConfig;
	NetConfig serverCfg;
	NetConfig logServerCfg;
	int heartPort;
	int dataPort;
	string logFileName;
	int capability;
}ParseConfig;

/*protobuf*/
typedef struct PrpcHead_t {
	uint8_t magic[4];      //'P','R','P','C'
	uint32_t length;
	uint32_t meta_length;
}PrpcHead;

#define	REDIS_POLICY_ID_STR			"id"
#define	REDIS_ENT_ID_STR			"enterpriseId"
#define	REDIS_POLICY_NAME_STR		"policyName"
#define	REDIS_ACTION_TYPE_STR		"actionType"
#define	REDIS_LOG_TYPE_STR			"logType"
#define	REDIS_DELIVERY_STR			"deliveryType"
#define	REDIS_SEVERITY_STR			"severity"
#define	REDIS_MAP_GROUP_LIST_STR	"mapGroupList"
#define	REDIS_MAP_ID_STR			"id"
#define	REDIS_MAP_VALUE_STR			"value"
#define	REDIS_DEV_GROUP_LIST_STR	"deviceCompanyGroupList"
#define	REDIS_DEV_ID_STR			"id"
#define	REDIS_DEV_VALUE_STR			"value"
#define	REDIS_NET_TYPE_LIST_STR		"networkTypeList"
#define	REDIS_NET_ID_STR			"id"
#define	REDIS_NET_VALUE_STR			"value"
#define	REDIS_SCH_LIST_STR			"schedulerList"
#define	REDIS_SCH_ID_STR			"id"
#define	REDIS_SCH_ENT_ID_STR		"enterpriseId"
#define	REDIS_SCH_NAME_STR			"schedulerName"
#define	REDIS_SCH_START_STR			"startDate"
#define	REDIS_SCH_END_STR			"endDate"
#define	REDIS_SCH_START_TIME1_STR	"startTime1"
#define	REDIS_SCH_END_TIME1_STR		"endTime1"
#define	REDIS_SCH_START_TIME2_STR	"startTime2"
#define	REDIS_SCH_END_TIME2_STR		"endTime2"
#define	REDIS_SCH_START_TIME3_STR	"startTime3"
#define	REDIS_SCH_END_TIME3_STR		"endTime3"
#define	REDIS_SCH_WEEKEND_STR		"weekday"
#define	REDIS_EVENT_LIST_STR		"productEventList"
#define	REDIS_EVENT_ID_STR			"eventId"
#define	REDIS_EVENT_NAME_STR		"eventName"
#define	REDIS_PRODUCT_ID_STR		"productId"
#define	REDIS_PRODUCT_NAME_STR		"productName"
#define	REDIS_PRO_EVENT_CATA_ID_STR	"categoryId"
#define	REDIS_FREQ_LIST_ID_STR		"frequencyList"
#define	REDIS_FREQ_ID_STR			"id"
#define	REDIS_FREQ_NAME_STR			"name"
#define	REDIS_FREQ_ENT_ID_STR		"enterpriseId"
#define	REDIS_FREQ_PERIOD_STR		"period"
#define	REDIS_FREQ_UNIT_STR			"periodUnit"
#define	REDIS_FREQ_THRESHOLD_STR	"threshold"

#define LOGSERVERIP_STR					"logserverip"
#define LOGSERVERIP_DEFAULT				"127.0.0.1"
#define	LOGSERVERPORT_STR				"logserverport"
#define	LOGSERVERPORT_DEFALUT			9222
#define SERVER_IP_STR					"serverip"
#define SERVER_IP_DEFAULT				"127.0.0.1"
#define SERVER_REG_PORT_STR				"serverregport"
#define SERVER_REG_PORT_DEFAULT			11010
#define SERVER_DATA_PORT_STR			"serverdataport"
#define SERVER_DATA_PORT_DEFAULT		11012
#define DBSERVERCHARSET_STR				"dbcharset"
#define DBSERVERCHARSET_DEFAULT			"gb2312"
#define DBSERVERUSER_STR				"dbuser"
#define DBSERVERUSER_DEFAULT			"cscreen"
#define DBSERVERPASSWD_STR				"dbpasswd"
#define DBSERVERPASSWD_DEFAULT			"cscreen"
#define DBSERVERNAME_STR				"dbname"
#define DBSERVERNAME_DEFAULT			"cscreen"

#define DBSERVERPORT_STR				"dbserverport"
#define DBSERVERPORT_DEFAULT			6379
#define DBSERVERIP_STR					"dbserverip"
#define DBSERVERIP_DEFAULT				"10.10.11.21"

#define LOCALREGPORT_STR				"localregport"
#define LOCALREGPORT_DEFAULT			22023	
#define LOCALDATAPORT_STR				"localdataport"
#define LOCALDATAPORT_DEFAULT			22022	

#define LOGFILENAME_STR					"logfilename"
#define LOGFILENAME_DEFAULT				"DBClient.log"

#define REDIS_USER_INFO_PREFIX			"POLICY_USER_BEHIVEPOLICY_"
#define REDIS_POLICY_INFO_PREFIX		"POLICY_POLICY_"
#define REDIS_POLICY_FREQ_PREFIX		"POLICY_USER_FREQ_"

#define REDIS_POLICY_ENT_ENCRY_PREFIX			"POLICY_ENCRY_PRODUCT_"
#define	REDIS_POLICY_ENT_ENCRY_CSP_NAME_PREFIX	"name"
#define REDIS_POLICY_ENT_ENCRY_INFO_PREFIX		"POLICY_ENCRY_"

#define REDIS_POLICY_ENCRY_KEY		"POLICY_ENCRY_KEY"
#define REDIS_POLICY_ENCRY_VALUE	"POLICY_ENCRY_VALUE"
#define REDIS_POLICY_ENCRY_TEXT		"POLICY_ENCRY_TEXT"



#endif
#加新文本识别

