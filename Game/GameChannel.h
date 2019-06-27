#pragma once
#include<ZinxTCP.h>
#include"GameProtocol.h"
#include"GameChannel.h"
class GameChannel :
	public ZinxTcpData
{
public:
	GameChannel(int _fd);
	virtual ~GameChannel();
	GameProtocol* m_proto = NULL;
	// 通过 ZinxTcpData 继承
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
};

/*创建tcp连接工厂类继承 重写createTcpDataChannel ，构造步骤1的对象*/
class GameConnFact:public IZinxTcpConnFact{
	// 通过 IZinxTcpConnFact 继承
	virtual ZinxTcpData * CreateTcpDataChannel(int _fd) override;
};