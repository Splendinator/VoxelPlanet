#pragma once

#include "VectorPrimitiveLayer.h"

class VectorPrimitiveLayer;

/** VectorPrimitiveFactoryManager
*
* This class is used for managing all of the primitive factories and using them to read in a file.
*/
class VectorPrimitiveFactoryManager
{
public:

	// Returns a root VectorPrimitiveLayer that contains all primitives in the file
	static VectorPrimitiveLayer ReadInFile(const char* pFilePath);

protected:

private:

};
