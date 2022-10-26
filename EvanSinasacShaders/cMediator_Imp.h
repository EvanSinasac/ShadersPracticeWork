#pragma once

// This is the implementation class for the mediator.
// All the actual "brains" of the mediator should be here, not in the cMediator

#include "iMediator.h"

//#include "iShip.h"

class cMediator_Imp :
	public iMediator
{
public:
	cMediator_Imp();
	~cMediator_Imp();

	// ****************************************
	// From the iMessage interface
	virtual bool SetReciever(iMediator* pTheReciever);
	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);
	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
	// ****************************************

//	void SetAllTheShips(const std::vector< iShip* > &vec_pShips);

	//std::vector< iShip* > m_vec_pShips;

	// The light manager has an iMessage interface
	iMessage* m_pLightManager;
};
