#pragma once

#include "iMessage.h"
#include <vector>


class iMediator : public iMessage
{
public:
	virtual ~iMediator() {};

	//	// **********************************************************
	//	// From the iMessage interface
	//	virtual bool SetReciever(iMediator* pTheReciever) = 0;
	//	// Asynchronous (don't return)
	//	virtual bool RecieveMessage(sMessage theMessage) = 0;
	//	// Synchronous (do return with something)
	//	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) = 0;
	//	// **********************************************************

};
