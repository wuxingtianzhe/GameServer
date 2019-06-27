#include "GameProtocol.h"
#include"GameMsg.h"
using namespace std;


GameProtocol::GameProtocol()
{
}


GameProtocol::~GameProtocol()
{
}
/*输入参数是通道传来的原始报文
返回值是转换后的信息对象MultiMsg
转换方式是TCP粘包处理*/
UserData * GameProtocol::raw2request(std::string _szInput)
{
	MultiMsg* pRet = new MultiMsg();
	szLast.append(_szInput);
	while (1)
	{
		if (szLast.size()<8)
		{
			break;
		}
		/*在前四个字节中的读取消息长度*/
		int iLength = 0;
		iLength |= szLast[0] << 0;
		iLength |= szLast[1] << 8;
		iLength |= szLast[2] << 16;
		iLength |= szLast[3] << 24;
		/* 中四个字节读类型id*/
		int id = 0;
		id |= szLast[4] << 0;
		id |= szLast[5] << 8;
		id |= szLast[6] << 16;
		id |= szLast[7] << 24;
		/*通过读到的长度判断后续报文是否合法*/
		if (szLast.size()-8<iLength)
		{
			/*本条报文还不够，啥都不干*/
			break;
		}
		/*构造一套用户请求*/
		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, iLength));
		pRet->m_Msgs.push_back(pMsg);
		szLast.erase(0, 8 + iLength);
	}
	for (auto single:pRet->m_Msgs)	{
		/*将*/
	cout << single->pMsg->Utf8DebugString() << endl;
	}
	return pRet;
}

std::string * GameProtocol::response2raw(UserData & _oUserData)
{
	return nullptr;
}

Irole * GameProtocol::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	return nullptr;
}

Ichannel * GameProtocol::GetMsgSender(BytesMsg & _oBytes)
{
	return nullptr;
}
