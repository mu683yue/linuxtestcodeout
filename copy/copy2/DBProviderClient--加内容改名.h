三梵蒂冈风光的
#ifndef DBPROVIDERCLIENT_H_
#define DBPROVIDERCLIENT_H_

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
//#include <linux/tcp.h>
#include <linux/types.h>
#include <linux/ip.h>
#include <netinet/tcp.h>


#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <sstream>
#include <iostream>  
#include <fstream>

#include "DBConnectorPool.h"
#include "DBConnector.h"

#if	1
#include "RpcMeta.pb.h"
#include "policyToDBProvider.pb.h"
#include "DBProviderToPolicy.pb.h"
#include "cloudPolicy.pb.h"
#include "DBProviderServerAndClient.pb.h"
#else
#include <3rdparty.h>
#endif
#include "json/json.h"
#include "defines.h"


using namespace std; //std 命名空间


typedef enum {
	LEVEL_ERROR,
	LEVEL_WARN,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_MAX
}LOG_LEVEL;

typedef enum {
	POLICY_MATCH = 3,
	POLICY_BLOCK = 2,
	POLICY_PASS = 1,
	POLICY_MISMATCH = 0
}POLICY_MATCH_RES;

typedef struct _policyToDBPorvider{
	INT64	entId;
	INT64 	user_id;
	INT64 	device_activity_id;
	INT64 	device_id;
	INT64 	enc_csp_product_id;
	INT64 	csp_product_id;
	INT64 	event_id;
	INT64 	network_type_id;
	list<INT64> 	map_group_id;
	list<INT64> 	device_group_id;
	INT32 	msg_id;
	string	session_id;
	string 	traffic_id;
}PolicyToDBPorvider;

typedef struct _freqInfo{
	INT64	id;
	string	freqName;
	INT32	freqPeriod;
	INT32	freqPeriodUnit;
	INT32	threshold;
}PolicyFreqInfo;

typedef struct _encryInfo{
	string	encry_key;
	string	encry_value;
	string	encry_text;
	string	csp_name;
	INT32  	enable_encry;	
}PolicyEncryInfo;


typedef struct _PolicyMatchResult{
	INT64 	deviceActivityId;
	string 	sessionId;
	string 	trafficId;
	string 	trigTime;
	POLICY_MATCH_RES res;
	POLICY_MATCH_RES matchRes;
	INT64 	entId;
	INT64	userId;
	string	policyName;
	INT64 	policyId;
	INT64 	productId;
	string 	productName;
	INT64 	eventId;
	INT64 	eventCataId;
	string 	eventName;
	
	string 	matchDevName;
	INT64 	matchDevID;
	string 	matchMapName;
	INT64 	matchMapID;
	string 	matchNetName;
	INT64 	matchNetID;
	
	INT8	logType;
	INT8	deliveryType;
	INT8	severity;
	INT32	msg_id;
	PolicyFreqInfo freqInfo;
	PolicyEncryInfo encryInfo;
}PolicyMatchResult;


typedef struct _noteInfo{
	INT64	productId;
	string	productName;
	
	INT64	eventId;
	string	eventName;	
	INT64 	eventCataId;
}ProductEventInfo;

typedef struct _PolicySchedule{

	INT64 id;
	string startDate;
	string endDate;
	string startTime1;
	string endTime1;
	string startTime2;
	string endTime2;	
	string startTime3;
	string endTime3;

	string weekday;
}PolicySchedule;

typedef struct _entPolicy{
	INT64	enterpriseId;
	INT64	policyId;
	string	policyName;
	INT8	actionType;
	INT8	logType;
	INT8	deliveryType;
	INT8	severity;

	map<INT64, string>	devsMap;
	map<INT64, string>	netsMap;
	map<INT64, string>	mapsMap;

	list<PolicySchedule> 	listPolicyScheduleList;
	list<ProductEventInfo>	listProductEventInfoList;
	list<PolicyFreqInfo> 	listPolicyFreqInfoList;
}EntPolicy;

#endif



#include "DBConnectorPool.h"

DBConnectorPool::DBConnectorPool(DB_Config & config, int s){
	pthread_mutex_init(&lock,NULL); 
	dbConfig = config;
	pollSize = s;
}

DBConnectorPool::~DBConnectorPool(){
	this->Destorydbpool();
}

//完成连接池的初始化 并将连接句柄存放到list
void  DBConnectorPool::Initdbpool(string &strLogMsg){
	int cntTrue = 0;
	int cntFalse = 0;
	int i;	
	
	for(i = 0; i < pollSize; i++)
	{
		DBConnector * temp = new DBConnector(dbConfig);
		string strMsg;
		if (temp->connect(strMsg) == SUCCESS){
			cntTrue++;
		}else{
			cntFalse++;
		}
		temp->setStatus(STATUS_NOUSE);
		dbPoll.push_back(temp);
		usleep(8000);
	}	
	char tmpStr[MINI_BUF_SIZE] = {0};
	sprintf(tmpStr, "Init DB pool over,pool size:%d, success:%d, failed:%d.", pollSize,cntTrue, cntFalse);
	strLogMsg = tmpStr;
}

void DBConnectorPool::Destorydbpool(){

	list<DBConnector *>::iterator it;
	pthread_mutex_lock(&lock);
	for(it = dbPoll.begin();it != dbPoll.end(); ++it){
		DBConnector * tmp = *it;
		delete tmp;		
	}
	dbPoll.clear();
	pthread_mutex_unlock(&lock);
}

//从连接池 获取连接
DBConnector * DBConnectorPool::Getconnect(){

	list<DBConnector *>::iterator it;
	pthread_mutex_lock(&lock);
	for(it = dbPoll.begin(); it != dbPoll.end(); ++it){
		DBConnector * tmp = *it;
		if (tmp->getStatus() == STATUS_NOUSE){
			tmp->setStatus(STATUS_USING);
			pthread_mutex_unlock(&lock);
			return tmp;
		}
	}
	pthread_mutex_unlock(&lock);
	return NULL;
}

void DBConnectorPool::RecoveryConnector(DBConnector * itor){
	itor->setStatus(STATUS_NOUSE);
}

void DBConnectorPool::reconnect(){
	
	list<DBConnector *>::iterator it;
	for(it = dbPoll.begin(); it != dbPoll.end(); ++it){
		DBConnector * tmp = *it;
		if (tmp->getStatus() == STATUS_INIT){
				string strMsg;
				tmp->connect(strMsg);
		}
	}
}




三梵蒂冈风光的
#ifndef DBPROVIDERCLIENT_H_
#define DBPROVIDERCLIENT_H_

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
//#include <linux/tcp.h>
#include <linux/types.h>
#include <linux/ip.h>
#include <netinet/tcp.h>


#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <sstream>
#include <iostream>  
#include <fstream>

#include "DBConnectorPool.h"
#include "DBConnector.h"

#if	1
#include "RpcMeta.pb.h"
#include "policyToDBProvider.pb.h"
#include "DBProviderToPolicy.pb.h"
#include "cloudPolicy.pb.h"
#include "DBProviderServerAndClient.pb.h"
#else
#include <3rdparty.h>
#endif
#include "json/json.h"
#include "defines.h"


using namespace std; //std 命名空间


typedef enum {
	LEVEL_ERROR,
	LEVEL_WARN,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_MAX
}LOG_LEVEL;

typedef enum {
	POLICY_MATCH = 3,
	POLICY_BLOCK = 2,
	POLICY_PASS = 1,
	POLICY_MISMATCH = 0
}POLICY_MATCH_RES;

typedef struct _policyToDBPorvider{
	INT64	entId;
	INT64 	user_id;
	INT64 	device_activity_id;
	INT64 	device_id;
	INT64 	enc_csp_product_id;
	INT64 	csp_product_id;
	INT64 	event_id;
	INT64 	network_type_id;
	list<INT64> 	map_group_id;
	list<INT64> 	device_group_id;
	INT32 	msg_id;
	string	session_id;
	string 	traffic_id;
}PolicyToDBPorvider;

typedef struct _freqInfo{
	INT64	id;
	string	freqName;
	INT32	freqPeriod;
	INT32	freqPeriodUnit;
	INT32	threshold;
}PolicyFreqInfo;

typedef struct _encryInfo{
	string	encry_key;
	string	encry_value;
	string	encry_text;
	string	csp_name;
	INT32  	enable_encry;	
}PolicyEncryInfo;


typedef struct _PolicyMatchResult{
	INT64 	deviceActivityId;
	string 	sessionId;
	string 	trafficId;
	string 	trigTime;
	POLICY_MATCH_RES res;
	POLICY_MATCH_RES matchRes;
	INT64 	entId;
	INT64	userId;
	string	policyName;
	INT64 	policyId;
	INT64 	productId;
	string 	productName;
	INT64 	eventId;
	INT64 	eventCataId;
	string 	eventName;
	
	string 	matchDevName;
	INT64 	matchDevID;
	string 	matchMapName;
	INT64 	matchMapID;
	string 	matchNetName;
	INT64 	matchNetID;
	
	INT8	logType;
	INT8	deliveryType;
	INT8	severity;
	INT32	msg_id;
	PolicyFreqInfo freqInfo;
	PolicyEncryInfo encryInfo;
}PolicyMatchResult;


typedef struct _noteInfo{
	INT64	productId;
	string	productName;
	
	INT64	eventId;
	string	eventName;	
	INT64 	eventCataId;
}ProductEventInfo;

typedef struct _PolicySchedule{

	INT64 id;
	string startDate;
	string endDate;
	string startTime1;
	string endTime1;
	string startTime2;
	string endTime2;	
	string startTime3;
	string endTime3;

	string weekday;
}PolicySchedule;

typedef struct _entPolicy{
	INT64	enterpriseId;
	INT64	policyId;
	string	policyName;
	INT8	actionType;
	INT8	logType;
	INT8	deliveryType;
	INT8	severity;

	map<INT64, string>	devsMap;
	map<INT64, string>	netsMap;
	map<INT64, string>	mapsMap;

	list<PolicySchedule> 	listPolicyScheduleList;
	list<ProductEventInfo>	listProductEventInfoList;
	list<PolicyFreqInfo> 	listPolicyFreqInfoList;
}EntPolicy;

#endif


