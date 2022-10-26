#include "cMediator.h"
//#include "iShip.h"

#include "cMediator_Imp.h"

cMediator::cMediator()
{
	this->m_Imp = new cMediator_Imp();
}

cMediator::~cMediator()
{
	delete this->m_Imp;
}

// *******************************************************************
// From the iMessage interface
bool cMediator::SetReciever(iMediator* pTheReciever)
{
	this->m_Imp->SetReciever(pTheReciever);
	return true;
}

// Asynchronous (don't return)
bool cMediator::RecieveMessage(sMessage theMessage)
{
	// Pass to the implementation
	return this->m_Imp->RecieveMessage(theMessage);
}

// Synchronous (do return with something)
bool cMediator::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return this->m_Imp->RecieveMessage(theMessage, messageReply);
}
// *******************************************************************


//void cMediator::SetAllTheShips( const std::vector< iShip* > &vec_pShips )
//{
		// This is really a loop inside, but is using the STL library 
		//	to "back insert" copy all the data. 
		// It's:
		// * NOT faster
		// * NOT better
		// It's just one line. That's it.
//	std::copy(vec_pShips.begin(), vec_pShips.end(), std::back_inserter(this->m_vec_pShips));
//}
