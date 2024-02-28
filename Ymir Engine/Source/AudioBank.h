#pragma once

#ifndef __AUDIOBANK_H__
#define __AUDIOBANK_H__

#include <string>
#include <map>

#include "Globals.h"

typedef struct
{
	std::string	bank_name;
	bool loaded_in_heap;
	std::map<uint64, std::string> events;  // Sound and FXs
	std::map<uint64, std::string> actions; // Other events

} AudioBank;

#endif // __AUDIOBANK_H__