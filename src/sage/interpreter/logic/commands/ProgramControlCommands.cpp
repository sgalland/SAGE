﻿#include "ProgramControlCommands.h"
#include "../../AgiInterpreter.h"
#include "ObjectControlCommands.h"
#include "ObjectMotionControlCommands.h"

void ProgramControlCommands::exec_new_room(uint8_t roomID)
{
	// Commands stop.update and unanimate are issued to all objects;
	 //ObjectControlCommands::stop_update();
	// ObjectControlCommands::unanimate_all();
	// All resources except Logic(0) are discarded;
	disard_all_resources();
	// Command player.control is issued;
	//ObjectMotionControlCommands::player_control();
	// unblock command is issued;
	//ObjectMotionControlCommands::unblock();
	// set.horizon(36) command is issued;
	ObjectMotionControlCommands::set_horizon(36);
	// v1 is assigned the value of v0; v0 is assigned n (or the value of vn when the command is new.room.v); v4 is assigned 0; v5 is assigned 0; v16 is assigned the ID number of the VIEW resource that was associated with Ego (the player character).
	AgiInterpreter::variables[1] = AgiInterpreter::variables[0];
	AgiInterpreter::variables[0] = roomID;
	AgiInterpreter::variables[4] = 0;
	AgiInterpreter::variables[5] = 0;
	AgiInterpreter::variables[16] = 0; // ID of view resource that is the Ego - Ego is ALWAYS view 0
	// Logic(i) resource is loaded where i is the value of v0 !
	AgiInterpreter::logicProcessor.Execute(AgiInterpreter::variables[0]);
	// Set Ego coordinates according to v2:
	// * if Ego touched the bottom edge, put it on the horizon;
	// * if Ego touched the top edge, put it on the bottom edge of the screen;
	// * if Ego touched the right edge, put it at the left and vice versa.
	// ** v2 is assigned 0 (meaning Ego has not touched any edges).
	// ** f5 is set to 1 (meaning in the first interpreter cycle after the new_room command all initialization parts of all logics loaded and called from the initialization part of the new room's logic will be called. In the subsequent cycle f5 is reset to 0 (see section Interpreter work cycle and the source of the "Thunderstorm" program. This is very important!).
	// ** Clear keyboard input buffer and return to the main AGI loop.
}

void ProgramControlCommands::disard_all_resources()
{
	for (int i = 0; i < AgiInterpreter::MAX_RESOURCES; i++)
	{
		AgiView *currentView = AgiInterpreter::views[i];
		if (currentView != nullptr)
		{
			delete currentView;
			currentView = nullptr;
		}
	}

	for (int i = 0; i < AgiInterpreter::MAX_RESOURCES; i++)
	{
		AgiPicture *currentPicture = AgiInterpreter::pictures[i];
		if (currentPicture != nullptr)
		{
			delete currentPicture;
			currentPicture = nullptr;
		}
	}

	for (int i = 1; i < AgiInterpreter::MAX_RESOURCES; i++) // do not delete logic 0
	{
		AgiLogic *currentLogic = AgiInterpreter::logics[i];
		if (currentLogic != nullptr)
		{
			delete currentLogic;
			currentLogic = nullptr;
		}
	}

	// AgiInterpreter::sounds // implement when sounds are added
}

void ProgramControlCommands::new_room(uint8_t roomID)
{
	exec_new_room(roomID);
}

void ProgramControlCommands::new_room_v(uint8_t variableID)
{
	uint8_t roomID = AgiInterpreter::variables[variableID];
	exec_new_room(roomID);
}
