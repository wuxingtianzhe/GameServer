#include "GameRole.h"
#include"GameMsg.h"


GameRole::GameRole()
{
}


GameRole::~GameRole()
{
}

bool GameRole::Init()
{
	return true;
}
/*处理游戏相关的用户请求*/
UserData * GameRole::ProcMsg(UserData & _poUserData)
{
	/*测设：打印信息内容*/
	GET_REF2DATA(MultiMsg, input, _poUserData);
	for (auto single : input.m_Msgs)
	{
		std::cout << "type is" << single->enMsgType <<std::endl;
		std::cout << single->pMsg->Utf8DebugString() << std::endl;
	}
	return nullptr;
}

void GameRole::Fini()
{
}
