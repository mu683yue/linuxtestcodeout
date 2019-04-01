#include <time.h> //C璇█鐨勫ご鏂囦欢 
#include <unistd.h>
#include "DBProvider.h"
#include "cloudPolicy.pb.h"


int ServicePort = 11012;
list<string> logList;
pthread_mutex_t *ServiceReadSocksListLock = NULL;//
const char * protobufHead = "CSSG";

const char * logFileName = "log.txt";

int checkClientSocketConnectStatus(int sock)
{
    if (sock <= 0)
        return 0;
    struct tcp_info info;
    int len = sizeof(info);
    getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *) & len);
    if ((info.tcpi_state == TCP_ESTABLISHED)) {
        return 1;
    } else {
        return 0;
    }
}

int fd;
static int acnt = 0;
int getSystemTime()   
{   
	char tmpstr[1024] = {0};
	time_t timer;   
	struct tm* t_tm;   
	time(&timer);   
	t_tm = localtime(&timer);   
	sprintf(tmpstr,"%d-today is %02d-%02d-%02d\n", acnt++,t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec); 
	pthread_mutex_lock( ServiceReadSocksListLock ); 
	logList.push_back(tmpstr);
	pthread_mutex_unlock( ServiceReadSocksListLock );
	return 0;   
} 

void writelog(void * arg){

	int fd = open( logFileName, O_RDWR|O_CREAT|O_APPEND);

	if (fd < 0){
		printf("log file open err!!!\n");
	}
	string strTmp;
	while(1){
		usleep(5000);
		while(1){
			list<string>::iterator it;			
			if (0 != pthread_mutex_trylock(ServiceReadSocksListLock)){					
					break;
			}
			int len = logList.size();
			if (len <= 0) {
				pthread_mutex_unlock( ServiceReadSocksListLock );
				break;
			}
			it = logList.begin();	
			strTmp = *it;
			logList.erase(it);
			pthread_mutex_unlock( ServiceReadSocksListLock );

			write(fd, strTmp.data(), strTmp.length());	
		}
	}	
}

int socketCreateConnect(string &ipAddr, int port, int clientPort)
{
	int iret = -1;
	int  sockfd, num;  
	struct sockaddr_in server;  

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
		return iret;	
	}  

	if (0 < clientPort){
		sockaddr_in client;
		client.sin_family = AF_INET;
		client.sin_addr.s_addr = htonl(INADDR_ANY);
		client.sin_port = htons(clientPort);
		if (bind( sockfd, (struct sockaddr *) &client, sizeof(client)) == -1) {
			return iret;	
		}
	}

	bzero(&server,sizeof(server));  
	server.sin_family= AF_INET;  
	server.sin_port = htons(port);  
	server.sin_addr.s_addr=inet_addr(ipAddr.data()); 
	
	if(connect(sockfd,(struct sockaddr *)&server,sizeof(server)) == -1){  
		return iret;
	}   

	iret = sockfd;
	return iret;
}


int testconnectServer(){

	
	int iret = 1, sockService = 0;
	string str = "127.0.0.1";
	
	sockService = socketCreateConnect( str, ServicePort, 0);
	if (sockService <= 0){
		iret = 0;
	}	
	iret = sockService;
	return iret;
}

void sendDataToDbProviderServer(int sockService, int cnt){
	string buf;
	PrpcHead head;
	head.magic[0] = protobufHead[0];
	head.magic[1] = protobufHead[1];
	head.magic[2] = protobufHead[2];
	head.magic[3] = protobufHead[3];

	//结果序列化
	policymsgpolicytodbprovider::MsgFromPolicyToDBProvider data;
	data.set_user_id(52);
	data.set_enterprise_id(321);
	data.set_device_activity_id(12345);
	data.set_device_id(1);
	data.set_product_id(14);
	data.set_event_id(14000);
	data.set_network_id(1);
	data.set_map_id(16);
	data.set_msg_id(cnt);
	data.set_session_id("123testsession_id123");
	data.set_traffic_id("123traffic_id123");
	
	// 对象序列化为string
	string protoBufData;
	data.SerializeToString(&protoBufData);
	int resProtoBufLen = protoBufData.length();
	
	//RpcMeta序列化数据
	
	RpcMetaMsg::RpcMeta rpcmeta;
	string metaData;
	rpcmeta.SerializeToString(&metaData);
	int metaLen = metaData.length();
	
	head.length = metaLen + resProtoBufLen;
	head.meta_length = metaLen;
	head.length = htonl(head.length);
	head.meta_length = htonl(head.meta_length);
	
	char tmpBuf[BUF_SIZE] = {0};
	memcpy(tmpBuf, (void*)&head, sizeof(PrpcHead));
	buf.assign(tmpBuf, sizeof(PrpcHead));
	buf += metaData;
	buf += protoBufData;	

	write(sockService, buf.data(), buf.length());
}


void processTestMessage(string &strServerDatas){


}

void testServer(void *arg){

	int sockService = 0;
	char tmpbuf[1024] = {0};
	int cnt = 1;
	bool isneedSend = true;
	bool isLinked = false;
	int iret = 0,i = 0;
	char * buf= NULL;
	int nfds = 0;
	struct epoll_event evClient,events[20];//
	int clientRegEpollFd;//

	clientRegEpollFd = epoll_create(256);
	buf = (char *)malloc(BUF_SIZE);

	//while (1)
	{
		while (false == isLinked){
			sockService = testconnectServer();
			if (0 >= sockService){
				sleep(1);
			}else{
				isLinked = true;
				//break;
			}	
		}
		evClient.data.fd = sockService;
	    evClient.events = EPOLLIN|EPOLLET;
	    epoll_ctl(clientRegEpollFd,EPOLL_CTL_ADD, sockService, &evClient);	

		//do{
			if (sockService <= 0){
				isLinked = false;
				//break;
			}
			
			if (isneedSend){
				if (cnt > 50000)
					cnt = 1;
				memset(tmpbuf, 0, 1024);
				//char *tmpstr = "send data to server!\n";
				//logList.push_back(tmpstr);						
				sendDataToDbProviderServer(sockService,cnt);
				isneedSend = false;
				getSystemTime();
			}
			nfds = epoll_wait(clientRegEpollFd,events,20,500);
			for(i=0;i<nfds;++i){
					 if(events[i].data.fd == sockService){

						memset(buf, 0, BUF_SIZE);
						iret = read(sockService, buf, BUF_SIZE);
						//printf("sock:%d, rev:%s\n", sockService, buf);
						if (iret <= 0){
							if (0 == checkClientSocketConnectStatus(sockService)){
								isLinked = false;
								close(sockService);	
								
								//break;
							}
						}
						else{
							//string strCon;
							//strCon.assign(buf, iret);
							//processTestMessage(strCon);
						}
						isneedSend = true;
					 }
					 isLinked = true;
				}		
		//}while(1);		
	}
	printf("test over!\n");
}
int main(int argc, char *argv[]){

	int i = 0;
	pthread_t thread;

	ServiceReadSocksListLock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init( ServiceReadSocksListLock ,NULL);
	
	for (; i < 10; i++){
		pthread_create(&thread,NULL, (void *(*)(void *))testServer, NULL);
		pthread_detach(thread);		
	}
	pthread_create(&thread,NULL, (void *(*)(void *))writelog, NULL);
	pthread_detach(thread); 	


	while(1){
		sleep(10);
	}
}

