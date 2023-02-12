#pragma once

class CodeParseTokenBase;

/** CodeParseTokenFactoryBase
*
* This class is used for reading in code files. It is used to create tokens like "Class" or "Scope" from the code file.
* See CodeParseTokenBase for more information on tokens.
*/
class CodeParseTokenFactoryBase
{
public:

	// Should be overridden to return whether the keyword is a match for the token type. (i.e "class" or "enum")
	virtual bool IsKeyword(const std::string& keyword) = 0;

	// Override to populate the token with metadata. The input ifstream will be start after the keyword. 
	// Used to create and populate a token. can return nullptr if there was an error creating a token to log an error.
	virtual CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream) = 0;

	// Optionally override to define an "end string". Token types with this are considered "scoped" tokens and will remain valid and on the stack until the end string is found.
	// Used for things like enums and classes.
	virtual std::string GetEndString() const { return ""; }

	// See pPreceedingTokens
	void SetRequiredPrecedingToken(CodeParseTokenFactoryBase* pInPrecedingTokenType) { pPrecedingTokenType = pInPrecedingTokenType; };

	// Returns whether this token type can be created based on the current stack. For example, only if the previous token was a class token, then a property token can be created.
	bool CanFactoryBeUsed(std::stack<CodeParseTokenFactoryBase*> currentStack) const;

	// Returns whether this is a "scoped" token type and should be added to the stack. See GetEndString()
	bool IsScopedTokenType() const { return GetEndString() != ""; }

private: 

	// Token that must be on the top of the stack before this token can be parsed. (i.e "enum" must be on the stack before "enum value" can be parsed)
	CodeParseTokenFactoryBase* pPrecedingTokenType = nullptr;
};