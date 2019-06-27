#include"GameChannel.h"
int main(int argc, char *argv[])
{

	ZinxKernel::ZinxKernelInit();
	/*add the listen channle*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899 , new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return EXIT_SUCCESS;
}