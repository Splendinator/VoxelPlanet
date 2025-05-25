#pragma once

#include "Game.h"

#include "Editor/Types/Properties/EditorTypePropertyBool.h"
#include "Editor/Types/Properties/EditorTypePropertyClass.h"
#include "Editor/Types/Properties/EditorTypePropertyEnum.h"
#include "Editor/Types/Properties/EditorTypePropertyFloat.h"
#include "Editor/Types/Properties/EditorTypePropertyInt.h"
#include "Editor/Types/Properties/EditorTypePropertyString.h"
#include "Editor/Types/Properties/EditorTypePropertyStruct.h"

class EditorTypePropertyBase;

namespace DataCompositePropertyUtils
{
	EditorTypePropertyBase* GetPropertyFromString(std::string value);
}

// This is supposed to be used with EDITORPROPERTY().
// It allows you to select a property from a EditorAssetDataComposite using drop down menus in the editor
// and then at asset load time this will resolve to the actual property, using template specialisations to grab the correct data type.
template<typename T>
class DataCompositeProperty
{
public:
	DataCompositeProperty() = default;

	~DataCompositeProperty() = default;
	~DataCompositeProperty() requires std::is_pointer_v<T>;

	// This can't be marked as explicit, but don't call this. It's just used when populating from the editor assets.
	// If you somehow call this is shouldn't matter because it will likely error anyway.
	// We use requires to specialise each type
	DataCompositeProperty(const std::string& inValue) requires std::is_pointer_v<T>;
	DataCompositeProperty(const std::string& inValue) requires std::is_enum_v<T>;
	DataCompositeProperty(const std::string& inValue) requires std::is_integral_v<T> && !std::is_same_v<T, bool>;
	DataCompositeProperty(const std::string& inValue) requires std::is_floating_point_v<T>;
	DataCompositeProperty(const std::string& inValue) requires std::is_same_v<T, std::string>;
	DataCompositeProperty(const std::string& inValue) requires std::is_same_v<T, bool>;
	DataCompositeProperty(const std::string& inValue);

protected:

	T value;
	bool bResponsibleForCleanup = false;
};

template <typename T>
DataCompositeProperty<T>::~DataCompositeProperty() requires std::is_pointer_v<T>
{
	if (bResponsibleForCleanup)
	{
		delete value;
	}
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue) requires std::is_pointer_v<T>
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		if (EditorTypePropertyClass* pClassProperty = dynamic_cast<EditorTypePropertyClass*>(pProperty))
		{
			value = static_cast<T>(pClassProperty->GetValue());

			// We need to clean up instanced assets as they are deep copied in GetValue()
			bResponsibleForCleanup = Game::GetAssetManager().FindClassTemplateType(pClassProperty->GetClassName())->HasMetadataFlag(EClassMetadataFlags::Instanced);
		}
	}
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue) requires std::is_enum_v<T>
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		if (EditorTypePropertyEnum* pEnumProperty = dynamic_cast<EditorTypePropertyEnum*>(pProperty))
		{
			value = static_cast<T>(pEnumProperty->GetValue());
		}
		else
		{
			DOMLOG_ERROR("Unsupported type for DataCompositeProperty", inValue);
		}
	}	
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue) requires std::is_integral_v<T> && !std::is_same_v<T, bool>
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		EditorTypePropertyInt* pIntProperty = dynamic_cast<EditorTypePropertyInt*>(pProperty);
		if (pIntProperty)
		{
			value = pIntProperty->GetValue();
		}
		else
		{
			DOMLOG_ERROR("Expected EditorTypePropertyInt for", inValue);
		}
	}
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue) requires std::is_floating_point_v<T>
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		EditorTypePropertyFloat* pFloatProperty = dynamic_cast<EditorTypePropertyFloat*>(pProperty);
		if (pFloatProperty)
		{
			value = pFloatProperty->GetValue();
		}
		else
		{
			DOMLOG_ERROR("Expected EditorTypePropertyFloat for", inValue);
		}
	}
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue) requires std::is_same_v<T, std::string>
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		EditorTypePropertyString* pStringProperty = dynamic_cast<EditorTypePropertyString*>(pProperty);
		if (pStringProperty)
		{
			value = pStringProperty->GetValue();
		}
		else
		{
			DOMLOG_ERROR("Expected EditorTypePropertyString for", inValue);
		}
	}
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue) requires std::is_same_v<T, bool>
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		EditorTypePropertyBool* pBoolProperty = dynamic_cast<EditorTypePropertyBool*>(pProperty);
		if (pBoolProperty)
		{
			value = pBoolProperty->GetValue();
		}
		else
		{
			DOMLOG_ERROR("Expected EditorTypePropertyBool for", inValue);
		}
	}
}

template <typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue)
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		EditorTypePropertyStruct* pStructProperty = dynamic_cast<EditorTypePropertyStruct*>(pProperty);
		if (pStructProperty)
		{
			T* pStructDeepCopy = static_cast<T*>(pStructProperty->GetValue());
			value = *pStructDeepCopy;
			delete pStructDeepCopy;
		}
		else
		{
			DOMLOG_ERROR("Expected EditorTypePropertyStruct for", inValue);
		}
	}
}

// We use template specializations for the different types we support.
//template<typename T>
//DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue)
//{
//	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
//	{
//		if (EditorTypePropertyStruct* pStructProperty = dynamic_cast<EditorTypePropertyStruct*>(pProperty))
//		{
//			T* pStructDeepCopy = pStructProperty->GetValue();
//			value = *pStructDeepCopy;
//			delete pStructDeepCopy;
//		}
//	}
//}
/*
template<typename T>
DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue, 
    std::enable_if_t<std::is_pointer_v<T>, U>*)
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		if (EditorTypePropertyEnum* pEnumProperty = dynamic_cast<EditorTypePropertyEnum*>(pProperty))
		{
			value = static_cast<T>(pEnumProperty->GetValue());
		}
		else
		{
			DOMLOG_ERROR("Unsupported type for DataCompositeProperty", inValue);
		}
	}
}

// Specializations
template<typename T, typename std::enable_if<std::is_pointer<T>::value>>
inline DataCompositeProperty<T>::DataCompositeProperty(const std::string& inValue)
{
	if (EditorTypePropertyBase* pProperty = DataCompositePropertyUtils::GetPropertyFromString(inValue))
	{
		if (EditorTypePropertyClass* pClassProperty = dynamic_cast<EditorTypePropertyClass*>(pProperty))
		{
			value = pClassProperty->GetValue();
		}
	}
}
*/
