#include "GameChannel.h"
#include"GameProtocol.h"
#include"GameRole.h"

GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{

}


GameChannel::~GameChannel()
{
	if (NULL!=m_proto)
	{
		ZinxKernel::Zinx_Del_Proto(*m_proto);
		delete m_proto;
	}
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
	pProtocol->m_channel = pChannel;
	/*创建角色对象*/
	auto pRole = new GameRole();
	/*绑定协议对象*/
	pChannel->m_proto = pProtocol;
	/*绑定角色对象*/
	pProtocol->m_role = pRole;
	pRole->m_pProto = pProtocol;
	/*将协议对象添加到 kernel*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	/*将角色对象添加到kernel*/	
	ZinxKernel::Zinx_Add_Role(*pRole);
	return pChannel;
}
