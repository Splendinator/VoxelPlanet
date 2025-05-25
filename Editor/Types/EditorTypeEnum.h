#pragma once

#include "EditorTypeBase.h"

struct EnumValueNameToValue
{
	std::string valueName; // "Red"
	int value; // "3"
};

/** EditorTypeEnum
 *
 * Holds an enum, used to map string names of enum values to their integer C++ value "EMyEnum::Red" -> 2
 */
class EditorTypeEnum : public EditorTypeBase
{
public:
	
	std::vector<EnumValueNameToValue> valueNamesToValues;
};
