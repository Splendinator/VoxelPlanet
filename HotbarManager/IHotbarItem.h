#pragma once

class ActionDeciderBase;
class DirectoryData;

struct OnHotbarItemUsedParams
{
	
};

// Anything that wants to be assigned to a hotbar slot should implement this interface
EDITORCLASS(Abstract)
class IHotbarItem
{
	EDITORBODY()
public:

	// Override these to get the file + icon layer combo of the icon we want to show when in the hotbar
	virtual std::string GetHotbarItemIconFile(DirectoryData& directoryData) const { PUREVIRTUAL() return ""; }
	virtual std::string GetOptionalHotbarItemIconLayer() const { return ""; }


	// Override this to do something when used on the hotbar, optionally returning an action for the player entity to take
	virtual ActionDeciderBase* OnHotbarItemUsed(const OnHotbarItemUsedParams& params) const { PUREVIRTUAL() return nullptr; }
	
	
};
