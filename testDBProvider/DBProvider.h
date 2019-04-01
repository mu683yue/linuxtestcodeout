#dsfghdf
#ifndef DBPROVIDER_H_
#define DBPROVIDER_H_

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


#include "RpcMeta.pb.h"
#include "policyToDBProvider.pb.h"
#include "cloudPolicy.pb.h"

using namespace std; //std 命名空间

#define SERVER_IP_MAX_LEN	20
#define BUF_SIZE			1024
#define MINI_BUF_SIZE		128

#define	MINI_PACKAGE_LEN_LIMIT	12
#define LIMIT_KEEPALIVE		3
#define MAX_PROCESS_PTHREAD_CNT 10

#define DISPATCH_TASK_NUM	3

#define PROTO_HEAD_ID_LEN	8
#define MSG_ID_MAX_LIMIT	60000

#define MSG_HEAD_STR		0xFE
#define MSG_HEAD_LEN_IDENT	1
#define MSG_DATA_LEN_IDENT	3

#define TASK_AGE_LIMIT		5	//老化数据，单位为秒

#define INT64	unsigned long long int
#define	INT32	int
#define UINT32	unsigned int
#define	INT16	short int
#define INT8 	char


typedef enum {
	LEVEL_ERROR,
	LEVEL_WARN,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_MAX
}LOG_LEVEL;


struct _taskInfo{
	INT64  user_id;
	string session_id;
	string traffic_id;
		
	string	device_activity_id;
	INT64  device_model_id;
	INT64  csp_product_id;
	INT64  event_id;
	INT64  network_type_id;
	INT64  map_group_id;
	INT64  msg_id;

	INT64 local_msg_id;
	time_t ageTime;
	int sock;

	void *policyData;

	bool operator==(const struct _taskInfo &rhs) const
	{
	   if( this->local_msg_id != rhs.local_msg_id) return false;
	   if( this->sock != rhs.sock) return false;

	   return true;
	}		

	bool operator<(const struct _taskInfo & rhs) const
	{
		if (rhs.sock < this->sock) return true;
		return false;
	}

	bool operator>(const struct _taskInfo & rhs) const
	{
		if (rhs.sock < this->sock) return false;
		return true;
	}


	bool operator!=(const struct _taskInfo &rhs) const
	{
	   if( this->local_msg_id != rhs.local_msg_id) return true;
	   if( this->sock != rhs.sock) return true;
	
	   return false;
	}	
};

typedef struct _taskInfo TaskInfo;


typedef struct _ClientList{

	string strIpAddr;
	int clientRegPort;
	int clientDataPort;
	
	int	dataSock;
	int clientSock;
	int capability;

	int keepalive;
	int errCount;

	pthread_mutex_t *taskPoolLock;
	list<TaskInfo> taskPool;

	struct _ClientList* next;
}ClientList;

typedef struct _taskCToSList{

	string ipAddr;
	int port;

	int sock;
}TaskCToSList;

/*protobuf*/
typedef struct PrpcHead_t {
	uint8_t magic[4];      //'P','R','P','C'
	uint32_t length;
	uint32_t meta_length;
}PrpcHead;


#endif


