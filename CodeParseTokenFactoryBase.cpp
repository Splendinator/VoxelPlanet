#include "pch.h"

#include "CodeParseTokenFactoryBase.h"

bool CodeParseTokenFactoryBase::CanFactoryBeUsed(std::stack<CodeParseTokenFactoryBase*> currentStack) const
{
	if (currentStack.empty())
	{
		return pPrecedingTokenType == nullptr;
	}
	else
	{
		return pPrecedingTokenType == currentStack.top();
	}
}
