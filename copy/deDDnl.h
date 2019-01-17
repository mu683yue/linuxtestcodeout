     1	#ifndef DEFINES_H_
     2	#define DEFINES_H_
       
     3	#include <string>
     4	#include <list>
     5	#include <map>
       
     6	#include <stdint.h>
       
     7	using namespace std;
       
     8	#define INT64	unsigned long long int
     9	#define	INT32	int
    10	#define UINT32	unsigned int
    11	#define	INT16	short int
    12	#define INT8 	char
       
       
    13	#define SUCCESS	0
    14	#define FALSE	-1
       
    15	#define MINI_PACKAGE_LEN_LIMIT	12
    16	#define MAX_PROCESS_PTHREAD_CNT	10
       
    17	#define BUF_SIZE			1024
    18	#define MINI_BUF_SIZE		128
       
    19	#define LIMIT_KEEPALIVE		3
    20	#define MAX_PROCESS_PTHREAD_CNT 10
       
    21	#define DISPATCH_TASK_NUM	3
       
    22	#define PROTO_HEAD_ID_LEN	8
    23	#define MSG_ID_MAX_LIMIT	60000
       
    24	#define MSG_HEAD_STR		0xFE
    25	#define MSG_HEAD_LEN_IDENT	1
    26	#define MSG_DATA_LEN_IDENT	3
       
    27	#define MSG_MINI_LEN		3
    28	#define GET_REDIS_CONNECTER_LOOP_LIMIT 1024
       
    29	#define DEFAULT_POLICY_THRESHOLD	POLICY_BLOCK
    30	#define REDIS_FREQ_VALUE_AGE	(24*3600)
    31	#define LOGSERVER_LIST_LIMIT	100
       
       
    32	typedef enum{
    33		STATUS_INIT,
    34		STATUS_USING,
    35		STATUS_NOUSE,
    36		STATUS_DESTROY
    37	}CONNECT_STATUS;
       
    38	typedef struct {
    39		string strIp;
    40		int portRev;
    41		int portData;
    42	}NetConfig;
       
    43	typedef struct{
    44		string	strServerIP;
    45		int 	port;
    46		string	strDbName;
    47		string	strUsername;
    48		string	strPasswd;
    49		string	strCharset;
    50		string	strErrMsg;
    51	}DB_Config;
       
    52	typedef struct{
    53		DB_Config dbConfig;
    54		NetConfig serverCfg;
    55		NetConfig logServerCfg;
    56		int heartPort;
    57		int dataPort;
    58		string logFileName;
    59		int capability;
    60	}ParseConfig;
       
    61	/*protobuf*/
    62	typedef struct PrpcHead_t {
    63		uint8_t magic[4];      //'P','R','P','C'
    64		uint32_t length;
    65		uint32_t meta_length;
    66	}PrpcHead;
       
    67	#define	REDIS_POLICY_ID_STR			"id"
    68	#define	REDIS_ENT_ID_STR			"enterpriseId"
    69	#define	REDIS_POLICY_NAME_STR		"policyName"
    70	#define	REDIS_ACTION_TYPE_STR		"actionType"
    71	#define	REDIS_LOG_TYPE_STR			"logType"
    72	#define	REDIS_DELIVERY_STR			"deliveryType"
    73	#define	REDIS_SEVERITY_STR			"severity"
    74	#define	REDIS_MAP_GROUP_LIST_STR	"mapGroupList"
    75	#define	REDIS_MAP_ID_STR			"id"
    76	#define	REDIS_MAP_VALUE_STR			"value"
    77	#define	REDIS_DEV_GROUP_LIST_STR	"deviceCompanyGroupList"
    78	#define	REDIS_DEV_ID_STR			"id"
    79	#define	REDIS_DEV_VALUE_STR			"value"
    80	#define	REDIS_NET_TYPE_LIST_STR		"networkTypeList"
    81	#define	REDIS_NET_ID_STR			"id"
    82	#define	REDIS_NET_VALUE_STR			"value"
    83	#define	REDIS_SCH_LIST_STR			"schedulerList"
    84	#define	REDIS_SCH_ID_STR			"id"
    85	#define	REDIS_SCH_ENT_ID_STR		"enterpriseId"
    86	#define	REDIS_SCH_NAME_STR			"schedulerName"
    87	#define	REDIS_SCH_START_STR			"startDate"
    88	#define	REDIS_SCH_END_STR			"endDate"
    89	#define	REDIS_SCH_START_TIME1_STR	"startTime1"
    90	#define	REDIS_SCH_END_TIME1_STR		"endTime1"
    91	#define	REDIS_SCH_START_TIME2_STR	"startTime2"
    92	#define	REDIS_SCH_END_TIME2_STR		"endTime2"
    93	#define	REDIS_SCH_START_TIME3_STR	"startTime3"
    94	#define	REDIS_SCH_END_TIME3_STR		"endTime3"
    95	#define	REDIS_SCH_WEEKEND_STR		"weekday"
    96	#define	REDIS_EVENT_LIST_STR		"productEventList"
    97	#define	REDIS_EVENT_ID_STR			"eventId"
    98	#define	REDIS_EVENT_NAME_STR		"eventName"
    99	#define	REDIS_PRODUCT_ID_STR		"productId"
   100	#define	REDIS_PRODUCT_NAME_STR		"productName"
   101	#define	REDIS_PRO_EVENT_CATA_ID_STR	"categoryId"
   102	#define	REDIS_FREQ_LIST_ID_STR		"frequencyList"
   103	#define	REDIS_FREQ_ID_STR			"id"
   104	#define	REDIS_FREQ_NAME_STR			"name"
   105	#define	REDIS_FREQ_ENT_ID_STR		"enterpriseId"
   106	#define	REDIS_FREQ_PERIOD_STR		"period"
   107	#define	REDIS_FREQ_UNIT_STR			"periodUnit"
   108	#define	REDIS_FREQ_THRESHOLD_STR	"threshold"
       
   109	#define LOGSERVERIP_STR					"logserverip"
   110	#define LOGSERVERIP_DEFAULT				"127.0.0.1"
   111	#define	LOGSERVERPORT_STR				"logserverport"
   112	#define	LOGSERVERPORT_DEFALUT			9222
   113	#define SERVER_IP_STR					"serverip"
   114	#define SERVER_IP_DEFAULT				"127.0.0.1"
   115	#define SERVER_REG_PORT_STR				"serverregport"
   116	#define SERVER_REG_PORT_DEFAULT			11010
   117	#define SERVER_DATA_PORT_STR			"serverdataport"
   118	#define SERVER_DATA_PORT_DEFAULT		11012
   119	#define DBSERVERCHARSET_STR				"dbcharset"
   120	#define DBSERVERCHARSET_DEFAULT			"gb2312"
   121	#define DBSERVERUSER_STR				"dbuser"
   122	#define DBSERVERUSER_DEFAULT			"cscreen"
   123	#define DBSERVERPASSWD_STR				"dbpasswd"
   124	#define DBSERVERPASSWD_DEFAULT			"cscreen"
   125	#define DBSERVERNAME_STR				"dbname"
   126	#define DBSERVERNAME_DEFAULT			"cscreen"
       
   127	#define DBSERVERPORT_STR				"dbserverport"
   128	#define DBSERVERPORT_DEFAULT			6379
   129	#define DBSERVERIP_STR					"dbserverip"
   130	#define DBSERVERIP_DEFAULT				"10.10.11.21"
       
   131	#define LOCALREGPORT_STR				"localregport"
   132	#define LOCALREGPORT_DEFAULT			22023	
   133	#define LOCALDATAPORT_STR				"localdataport"
   134	#define LOCALDATAPORT_DEFAULT			22022	
       
   135	#define LOGFILENAME_STR					"logfilename"
   136	#define LOGFILENAME_DEFAULT				"DBClient.log"
       
   137	#define REDIS_USER_INFO_PREFIX			"POLICY_USER_BEHIVEPOLICY_"
   138	#define REDIS_POLICY_INFO_PREFIX		"POLICY_POLICY_"
   139	#define REDIS_POLICY_FREQ_PREFIX		"POLICY_USER_FREQ_"
       
   140	#define REDIS_POLICY_ENT_ENCRY_PREFIX			"POLICY_ENCRY_PRODUCT_"
   141	#define	REDIS_POLICY_ENT_ENCRY_CSP_NAME_PREFIX	"name"
   142	#define REDIS_POLICY_ENT_ENCRY_INFO_PREFIX		"POLICY_ENCRY_"
       
   143	#define REDIS_POLICY_ENCRY_KEY		"POLICY_ENCRY_KEY"
   144	#define REDIS_POLICY_ENCRY_VALUE	"POLICY_ENCRY_VALUE"
   145	#define REDIS_POLICY_ENCRY_TEXT		"POLICY_ENCRY_TEXT"
       
       
       
   146	#endif
   147	#加新文本识别
       
