// Normal.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include<WinSock2.h>
#include<process.h>
#include<list>
#include<mutex>
#include<cmath>
#include<condition_variable>
#include <atomic>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
unsigned int WINAPI CreateServ(LPVOID args);
unsigned int WINAPI Proc(LPVOID args);
using namespace std;
const int _thread_count = 8;

FD_SET _readSet;
FD_SET _writeSet;
list<SOCKET> _sockList;
list<SOCKET> _removeList;
mutex lock4cv;
mutex lock4cv2;
condition_variable cv;
condition_variable cv2;
int _thread_unfinish;
vector<int> _vec;
char* buf2 = "hello client";

int main()
{
	for (int i = 0; i < _thread_count; i++)
	{
		_vec.push_back(0);
	}
	/*
	unsigned long _beginthreadex( void *security, unsigned stack_size, unsigned ( __stdcall *start_address )( void * ), void *arglist, unsigned initflag, unsigned *thrdaddr );
	//第1个参数：安全属性，NULL为默认安全属性
	//第2个参数：指定线程堆栈的大小。如果为0，则线程堆栈大小和创建它的线程的相同。一般用0
	//第3个参数：指定线程函数的地址，也就是线程调用执行的函数地址(用函数名称即可，函数名称就表示地址)
	//第4个参数：传递给线程的参数的指针，可以通过传入对象的指针，在线程函数中再转化为对应类的指针
    //第5个参数：线程初始状态，0:立即运行；CREATE_SUSPEND：suspended（悬挂）
	//第6个参数：用于记录线程ID的地址
	*/
	_beginthreadex(0, 0, CreateServ, 0, 0, 0);
	for (int i = 0; i < _thread_count; i++)
	{
		int* temp = new int(i);
		_beginthreadex(0, 0, Proc, temp, 0, 0);
	}
	cin.get();
	cin.get();
	return 0;
}
bool _isFinish()
{
	return _thread_unfinish <= 0;
}


unsigned int WINAPI Proc(LPVOID args)
{
	int* I = (int*)args;

	while (true)
	{
		{
			unique_lock<mutex> l(lock4cv);
			if (_vec[*I] <= 0)
			{
				cv.wait(l);
			}
			_vec[*I] = 0;
		}
		int start = ceil(_sockList.size() / (double)_thread_count)* *I;
		if (start < *I)
		{
			start = *I;
		}
		if (_sockList.size() <= start)
		{
			_thread_unfinish -= 1;
			lock4cv2.lock();
			cv2.notify_all();
			lock4cv2.unlock();
			continue;
		}
		int end = start + ceil(_sockList.size() / (double)_thread_count);

		int i = 0;
		for (auto iter = _sockList.begin(); iter != _sockList.end(); ++iter)
		{
			if (i >= start&&i < end) {
				SOCKET& s = *iter;
				if (FD_ISSET(s, &_readSet))
				{
					cout << "proc by:" << *I << endl;
					char buf[128];
					int len = recv(s, buf, 128, 0);
					if (len <= 0)
					{
						closesocket(s);
						_removeList.push_back(s);
						continue;
					}
					FD_SET(s, &_writeSet);
					cout <<"receive : "<< buf << endl;
				}
				if (FD_ISSET(s, &_writeSet))
				{
					send(s, buf2, 128, 0);
				}
			}
			i++;
		}
		lock4cv2.lock();
		_thread_unfinish -= 1;
		cv2.notify_all();
		lock4cv2.unlock();
	}
}
unsigned int WINAPI CreateServ(LPVOID args) {
	WORD wVersion;
	WSADATA wsaData;
	int err;
	/*
		声明调用不同的Winsock版本。例如MAKEWORD(2,2)就是调用2.2版，MAKEWORD(1,1)就是调用1.1版
	*/
	wVersion = MAKEWORD(2, 1);
	/*
		初始化套接字库，成功返回0
	*/
	err = WSAStartup(wVersion, &wsaData);
	if (err != 0) {
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return 0;
	}
	/*
		创建套接字 int socket(int domine,int type,int protocol);
		第一个参数：协议域，即协议族。决定了地址类型，取值有 AF_INET(IPV4)、AF_INET6(IPV6)、AF_LOCAL（或称AF_UNIX，Unix域socket）、AF_ROUTE等等。通常情况下我们使用AF_INET
		第二个参数：套接口类型。主要SOCK_STREAM(建立TCP连接)、SOCK_DGRAM（建立UDP）、SOCK_RAW；
					SOCK_STREAM(建立TCP连接)：提供面向连接的可靠的数据传输服务。数据被看作是字节流，无长度限制。例如FTP协议就采用这种。
					SOCK_DGRAM（建立UDP）：提供无连接的数据传输服务，不保证可靠性。
					SOCK_RAW：该接口允许对较低层次协议，如IP，ICMP直接访问。
		第三个参数：指定协议，常用的协议有，IPPROTO_TCP、IPPTOTO_UDP、IPPROTO_SCTP、IPPROTO_TIPC等，它们分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议。通常情况系设为0。

		注意：并不是上面的type和protocol可以随意组合的，如SOCK_STREAM不可以跟IPPROTO_UDP组合。当protocol为0时，会自动选择type类型对应的默认协议

		成功：返回套接字描述符（套接字的索引）
		失败：返回-1
	*/
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
	const char chOpt = 1;
	/*
	int setsockopt(int s,int level,int optname,const char *optval,int optlen);
		s：标识一个套接字的描述符。
		level：选项定义的层次；目前仅支持SOL_SOCKET和IPPROTO_TCP层次。
		optname：需设置的选项。TCP_NODELAY是唯一使用IPPROTO_TCP层的选项，其他所有选项都使用SOL_SOCKET层。TCP_NODELAY选项禁止Nagle算法。
				 Nagle算法通过将未确认的数据存入缓冲区直到蓄足一个包一起发送的方法，来减少主机发送的零碎小数据包的数目。
		optval：指针，指向存放选项值的缓冲区。
		optlen：optval缓冲区长度。
	*/
	setsockopt(sockSrv, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(chOpt));

	int nSendBufLen = 16 * 1024 * 1024;
	/*
		设置发送缓冲区：

		在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节
		(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据
		量比较大，可以设置socket缓冲区，而避免了send()不断的循环发送

		同理：接收缓存区设置为：
		// 接收缓冲区
		int nRecvBuf=32*1024;//设置为32K
		setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	*/
	setsockopt(sockSrv, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBufLen, sizeof(int));

	ULONG NonBlock = 1;//允许非阻塞模式
	/*
		控制套接字的模式
		int ioctlsocket( int s, long cmd, u_long * argp);
		s：一个标识套接口的描述字。
		cmd：对套接口s的操作命令。FIONBIO 允许或禁止套接口s的非阻塞模式。argp指向一个无符号长整型，如允许非阻塞模式则非零，如禁止非阻塞模式则为零。
		argp：指向cmd命令所带参数的指针。
	*/
	ioctlsocket(sockSrv, FIONBIO, &NonBlock);

	//	struct in_addr s;
	//	inet_pton(AF_INET, "127.0.0.1",(void*)&s);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(ADDR_ANY);//htonl()    "Host to Network Long", INADDR_ANY 就是inet_addr("0.0.0.0") 即监听所有
	addrSrv.sin_family = AF_INET;                  //AF_INET    代表TCP/IP协议族
	addrSrv.sin_port = htons(6001);                //htons()    "Host to Network Short"

	::bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	
	/*
	int listen( int sockfd, int backlog);
		sockfd：用于标识一个已捆绑未连接套接口的描述字。
		backlog：等待连接队列的最大长度。
	*/
	err = listen(sockSrv, 100);
	if (err == SOCKET_ERROR) {
		cout << "listen failed" << endl;
		WSACleanup();
		return 0;
	}


	//accept loop
	while (true) {

		FD_ZERO(&_readSet);  /*将set清零使集合中不含任何socket*/
		FD_ZERO(&_writeSet); /*将set清零使集合中不含任何socket*/

		FD_SET(sockSrv, &_readSet);/*将 sockSrv 加入_readSet集合*/
		for (auto iter = _sockList.begin(); iter != _sockList.end(); ++iter)
		{
			SOCKET& s = *iter;
			FD_SET(s, &_readSet);
		}
		/*
			select()，本函数用于确定一个或多个套接口的状态，对每一个套接口，调用者可查询它
			的可读性、可写性及错误状态信息，用fd_set结构来表示一组等待检查的套接口

			int select( int nfds, fd_set FAR* readfds,　fd_set * writefds, fd_set * exceptfds,　const struct timeval * timeout);
			nfds：      select监视的文件描述符数，视进程中打开的文件数而定，一般设置为监视各文件中的最大文件描述符值加1。不能错！在Windows中这个参数的值无所谓，可以设置不正确。
						（第一个参数不管,是兼容目的）
			readfds：  （可选）指针，指向一组等待可读性检查的套接口。
			writefds： （可选）指针，指向一组等待可写性检查的套接口。
			exceptfds：（可选）指针，指向一组等待错误检查的套接口。
			timeout：   select()最多等待时间，对阻塞操作则为NULL。 timeout=NULL（阻塞：直到有一个fd位被置为1函数才返回）
		*/
		select(0, &_readSet, &_writeSet, 0, 0);//检查套节字是否可读，可写,很多情况下就是是否有数据(注意,只是说很多情况) 这里select是否出错没有写 

		/*
			判断描述符fd是否在给定的描述符集fdset中，通常配合select函数使用，由于select函数成功返回时会将未准备好的描述符位清零。
			通常我们使用FD_ISSET是为了检查在select函数返回后，某个描述符是否准备好，以便进行接下来的处理操作。
			当描述符fd在描述符集fdset中返回非零值，否则，返回零。
		*/
		if (FD_ISSET(sockSrv, &_readSet))//检查sockSrv是否在这个集合里面,select将更新这个集合,把其中不可读的套节字去掉,只保留符合条件的套节字在这个集合里面
		{
			/*
				SOCKET accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
				addr用于存放客户端的地址，addrlen在调用函数时被设置为addr指向区域的长度，在函数调用结束后被设置为实际地址信息的长度。
				本函数会阻塞等待知道有客户端请求到达。
			*/
			SOCKET s = accept(sockSrv, 0, 0);
			ioctlsocket(s, FIONBIO, &NonBlock);
			_sockList.push_back(s);
		}
		else {
			_thread_unfinish = _thread_count;

			for (int i = 0; i < _thread_count; i++)
			{
				_vec[i] = 1;
			}
			cv.notify_all();// 唤醒所有线程

			unique_lock<mutex> l(lock4cv2);
			cv2.wait(l, _isFinish);//wait()用作条件等待，当条件不满足时（即，条件变量返回false），wait方法将当前线程挂起

			for (auto iter = _removeList.begin(); iter != _removeList.end(); ++iter)
			{
				_sockList.remove(*iter);
			}
			_removeList.clear();
		}
	}
}
