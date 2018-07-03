#pragma once
#include <string>

class MsgDisplaySpi {
public:
	virtual void PrintImp(const std::string &msg) = 0;
};

class MsgDisplayApi
{
public:
	MsgDisplayApi(void) : spi_(NULL) {};
	void SetMsgHandler(MsgDisplaySpi* spi){ spi_ = spi;}
	void Print(const std::string& msg){
		if (spi_) spi_->PrintImp(msg);
	}
	
private:
	MsgDisplaySpi *spi_;
};

