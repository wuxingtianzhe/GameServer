#include"GameChannel.h"
#include"msg.pb.h"
#include"GameMsg.h"
#include"AOIWorld.h"
#include"ZinxTimer.h"
#include"RandName.h"
extern RandName random_name;
//class myPlayer :public Player {
//public:
//	myPlayer(int _x, int _y, std::string _name) :x(_x), y(_y), name(_name) {}
//	int x;
//	int y;
//	std::string name;
//	// 通过 Player 继承
//	virtual int GetX() override
//	{
//		return x;
//	}
//	virtual int GetY() override
//	{
//		return y;
//	}
//};


//守护进程
void daemonlize()
{
	//1.fork
	int ipid = fork();
	//2.父进程推出
	if (0>ipid)
	{
		exit(-1);
	}
	if (0<ipid)
	{
		//2.父进程退出
		exit(0);
	}
	//3.子进程设置会话id
	setsid();
	//4.子进程设置执行路径
	//5.子进程重定向3个文件描述符/dev/null
	int nullfd = open("/dev/null", O_RDWR);
		if (nullfd>=0)
		{
			dup2(nullfd, 0);
			dup2(nullfd, 1);
			dup2(nullfd, 2);
			close(nullfd);
		}
		//进程监控
		while (1)
		{
			int pid = fork();
			if (0>pid)
			{
				exit(-1);
			}
			/*父进程等子进程退出*/
			if (0<pid)
			{
				int iStatus = 0;
				wait(&iStatus);
				if (0 == iStatus)
				{
					exit(0);
				}
			}
			else
			{
				break;
			}
		}
}
int main(int argc, char *argv[])
{
	/*测试代码1*/
	/*pb::SyncPid *pmsg = new pb::SyncPid();
	pmsg->set_pid(1);
	pmsg->set_username("test");

	GameMsg gm(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	auto output = gm.serialize();

	for (auto byte : output)
	{
		printf("%02X ", byte);
	}
	puts("");

	char buff[] = { 0x08, 0x01, 0x12, 0x04 ,0x74, 0x65, 0x73, 0x74 };
	std::string input(buff, sizeof(buff));*/
	/*测试代码2*/
	/*auto ingm = GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, input);
	std::cout << dynamic_cast<pb::SyncPid *> (ingm.pMsg)->pid() << std::endl;
	std::cout << dynamic_cast<pb::SyncPid *> (ingm.pMsg)->username() << std::endl;*/
	/* 测试代码3*/
	/*AOIWorld w(20, 200, 50, 230, 6, 6);
	myPlayer p1(60, 107, "1");
	myPlayer p2(91, 118, "2");
	myPlayer p3(147, 133, "3");
	w.AddPlayer(&p1);
	w.AddPlayer(&p2);
	w.AddPlayer(&p3);
	auto srd_list = w.GetSrdPlayers(&p1);
	for (auto single : srd_list)
	{
		std::cout << dynamic_cast<myPlayer *>(single)->name << std::endl;
	}*/
	daemonlize();
	random_name.LoadFile();
	ZinxKernel::ZinxKernelInit();
	/*add the listen channle*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899 , new GameConnFact())));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return EXIT_SUCCESS;
}