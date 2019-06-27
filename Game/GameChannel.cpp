#include "GameChannel.h"
#include"GameProtocol.h"


GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{

}


GameChannel::~GameChannel()
{
}

AZinxHandler * GameChannel::GetInputNextStage(BytesMsg & _oInput)
{
	return m_proto;
}

ZinxTcpData * GameConnFact::CreateTcpDataChannel(int _fd)
{
	/*创建tcp通道对象*/
	auto pChannel = new GameChannel(_fd);

	/*创建协议对象*/
	auto pProtocol = new GameProtocol();
	/*绑定协议对象*/
	pChannel->m_proto = pProtocol;
	/*将协议对象添加到 kernel*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	return pChannel;
}
