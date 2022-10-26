#pragma once

#include "iMediator.h"

// Forward declare for implementation
// (i.e. PIMPL pattern: pointer to implementation)
class cMediator_Imp;

class cMediator : public iMediator
{
public:
	cMediator();
	virtual ~cMediator();

	// ****************************************
	// From the iMessage interface
	virtual bool SetReciever(iMediator* pTheReciever);
	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);
	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
	// ****************************************

	// These methods are being used by the main, or whatever is "higher level"
	// thing that isn't just using the message passing commands
//	void SetAllTheShips( const std::vector< iShip* > &vec_pShips );

private:

	cMediator_Imp* m_Imp;
};
