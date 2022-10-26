#include "cMediator_Imp.h"

#include <algorithm>
#include <iterator>
//#include "iShip.h"

cMediator_Imp::cMediator_Imp()
{
	this->m_pLightManager = NULL;
}

cMediator_Imp::~cMediator_Imp()
{
}

// From the iMessage interface
bool cMediator_Imp::SetReciever(iMediator* pTheReciever)
{
	// Doesn't do anything (this IS the mediator)
	return true;
}

// Asynchronous (don't return)
bool cMediator_Imp::RecieveMessage(sMessage theMessage)
{
	//if (theMessage.command == "Explode something here")
	//{
	//	// #0 is location, #1 is radius of explosion

	//	// Go through seeing which ships are damaged

	//	sMessage takeDamage;
	//	takeDamage.command = "Take Damage";
	//	takeDamage.vec_fData.push_back(glm::vec4(10.0f, 0.0f, 0.0f, 0.0f));
	//	this->m_vec_pShips[17]->RecieveMessage(takeDamage);
	//	return true;
	//}//.command == "Explode something here"

	// Set the light manager
	if (theMessage.command == "Set Light Manager Pointer")
	{
		this->m_pLightManager = (iMessage*)theMessage.vec_pVoidPointers[0];
		return true;
	}//.command == "Set Light Manager Pointer"

	// This is used to handle light commands and is just passed directly to the light manager
	if (theMessage.command == "Set Light")
	{
		if (this->m_pLightManager)
		{
			return this->m_pLightManager->RecieveMessage(theMessage);
		}
		// No light manager
		return false;
	}//.command == "Set Light"

//	if (theMessage.command == "Spike")
//	{
//		iShip* pSpike = ...;
//		pSpike->RecieveMessage(theMessage);
//	}


	//if (theMessage.command == "Set Ship Pointer")
	//{
	//	iShip* pTheShip = (iShip*)theMessage.vec_pVoidPointers[0];
	//	this->m_vec_pShips.push_back(pTheShip);
	//}//.command == "Set Ship Pointer"

	//if (theMessage.command == "Set Multiple Ship Pointers")
	//{
	//	this->m_vec_pShips.clear();
	//	// Copy all the ship pointers. 
	//	// Not the cast FROM the void* (pointers to void)
	//	for (unsigned int index = 0; index != theMessage.vec_pVoidPointers.size(); index++)
	//	{
	//		this->m_vec_pShips.push_back((iShip*)theMessage.vec_pVoidPointers[index]);
	//		// Or
	//		//	iShip* pTheShip = (iShip*)theMessage.vec_pVoidPointers[index];
	//		//	this->m_vec_pShips.push_back(pTheShip);
	//	}
	//}//.command == "Set Ship Pointers"


	// Unknown command
	return false;
}

// Synchronous (do return with something)
bool cMediator_Imp::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	//if (theMessage.command == "Find Target")
	//{
	//	// TODO: Do a better "finding of targets"
	//	// For now, just pick some random targets
	//	for (unsigned int count = 0; count != 10; count++)
	//	{
	//		unsigned int randIndex = rand() % this->m_vec_pShips.size();
	//		messageReply.vec_fData.push_back(glm::vec4(this->m_vec_pShips[randIndex]->getMeshPointer()->positionXYZ, 1.0f));
	//	}
	//	return true;
	//}

	// Unknown command
	return false;
}

