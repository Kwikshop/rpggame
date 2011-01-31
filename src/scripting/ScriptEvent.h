/*
 * ScriptEvent.h
 *
 *  Created on: Jan 31, 2011
 *      Author: bmonkey
 */

#ifndef SCRIPTEVENT_H_
#define SCRIPTEVENT_H_

#include "scripting/Script.h"

	struct SCRIPT_EVENT
	{
		SCRIPT_EVENT( const std::string& str ): name( str ) {}
		std::string name;
	};

class ScriptEvent {
public:
	ScriptEvent();
	virtual ~ScriptEvent();
	static long send(EERIE_SCRIPT * es, long msg, const std::string& params, INTERACTIVE_OBJ * io, const std::string& eventname, long info = 0);
	//extern SCRIPT_EVENT AS_EVENT[];
};

#endif /* SCRIPTEVENT_H_ */
