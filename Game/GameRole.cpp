#include "GameRole.h"
#include"GameMsg.h"
#include"AOIWorld.h"
#include<iostream>
#include"msg.pb.h"
#include"GameChannel.h"
#include"GameProtocol.h"
using namespace std;
/*创建游戏世界的全局对象*/
static AOIWorld world(0, 400, 0, 400, 20, 20);
GameMsg * GameRole::createIDNameLogin()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	return pRet;
}

GameMsg * GameRole::createIDNameLogoff()
{

	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
	return pRet;
}

GameMsg * GameRole::createSrdPlayers()
{
	pb::SyncPlayers* pMsg = new pb::SyncPlayers();
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single : srd_list)
	{
		auto  pPlayer = pMsg->add_ps();
		auto pRole = dynamic_cast<GameRole*>(single);
		pPlayer->set_pid(pRole->iPid);
		pPlayer->set_username(pRole->szName);
		auto pPostion = pPlayer->mutable_p();
		pPostion->set_x(pRole->x);
		pPostion->set_y(pRole->y);
		pPostion->set_z(pRole->z);
		pPostion->set_v(pRole->v);
	}
	GameMsg*  pret = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);
	return pret;
}

GameMsg * GameRole::createSelfPostion()
{
	pb::BroadCast* pMsg = new pb::BroadCast();
	pMsg->set_pid(iPid);
	pMsg->set_username(szName);
	pMsg->set_tp(2);
	 
	auto pPositon = pMsg->mutable_p();
	pPositon->set_x(x);
	pPositon->set_y(y);
	pPositon->set_z(z);
	pPositon->set_v(v);

	GameMsg*  pret = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
	return pret;
}

GameMsg * GameRole::createTalkBroadCast(std::string _content)
{
	pb::BroadCast* pmsg = new pb::BroadCast();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	pmsg->set_tp(1);
	pmsg->set_content(_content);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pmsg);
	return pRet;
}

GameRole::GameRole()
{
	szName = "Tom";
	  x =100;
	  z =100;

}


GameRole::~GameRole()
{
}

bool GameRole::Init()
{
	/*添加到自己到游戏世界*/
	bool bRet = false;
	/*设置玩家ID为当前链接的fd*/
	iPid = m_pProto->m_channel->GetFd();
	bRet = world.AddPlayer(this);
	if (true==bRet)
	{
		/*向自己发送ID和名称*/
		auto pmsg = createIDNameLogin();
		ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
		/*向自己发送周围玩家的位置*/	
		pmsg = createSrdPlayers();
		ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
		/*向周围玩家发送自己的位置*/
		auto srd_list = world.GetSrdPlayers(this);
		for (auto single : srd_list)
		{
			pmsg = createSelfPostion();
			auto pRole = dynamic_cast<GameRole*>(single);
			ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
		}

	}
	return bRet;
}
/*处理游戏相关的用户请求*/
UserData * GameRole::ProcMsg(UserData & _poUserData)
{
	/*测设：打印信息内容*/
	GET_REF2DATA(MultiMsg, input, _poUserData);
	for (auto single : input.m_Msgs)
	{
		std::cout << "type is " << single->enMsgType <<std::endl;
		std::cout << single->pMsg->Utf8DebugString() << std::endl;
		/*聊天信息处理*/
		if (single->enMsgType==GameMsg::MSG_TYPE_CHAT_CONTENT)
		{
			/*取出聊天内容*/
			auto content = dynamic_cast<pb::Talk*>(single->pMsg)->content();
			/*发给所有人*/
			auto role_list = ZinxKernel::Zinx_GetAllRole();
			for (auto pRole:role_list)
			{
				auto pGameRole = dynamic_cast<GameRole*>(pRole);
				auto pmsg = createTalkBroadCast(content);
				ZinxKernel::Zinx_SendOut(*pmsg, *(pGameRole->m_pProto));
			}
		}
		/*向周围玩家发送自己位置*/
		if (single->enMsgType == GameMsg::MSG_TYPE_NEW_POSITION)
		{
			/*取出新位置*/
			auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);
			/*遍历周围的玩家发送*/
			/*向周围玩家发送自己的位置*/
			auto srd_list = world.GetSrdPlayers(this);

			for (auto single : srd_list)
			{
				//组成待发送的报文 
				pb::BroadCast* pMsg = new pb::BroadCast();
				auto pPos = pMsg->mutable_p();
				pPos->set_x(NewPos->x());
				pPos->set_y(NewPos->y());
				pPos->set_z(NewPos->z());
				pPos->set_v(NewPos->v());
				pMsg->set_pid(iPid);
				pMsg->set_tp(4);
				pMsg->set_username(szName);
				auto pRole = dynamic_cast<GameRole*>(single);
				auto msg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
				ZinxKernel::Zinx_SendOut(*msg, *(pRole->m_pProto));
			}
		}
	}
	return nullptr;
}

void GameRole::Fini()
{
	
	/*向周围玩家发送自己的位置*/
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single : srd_list)
	{
		GameMsg* pmsg = createIDNameLogoff();
		auto pRole = dynamic_cast<GameRole*>(single);
		ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
	}
	world.DelPlayer(this);
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}
