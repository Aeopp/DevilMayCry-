#include "..\Header\Object.h"

USING(ENGINE)
static uint32 UniqueIDRegist = 0u;

Object::Object() :
	UniqueID { UniqueIDRegist++}
{

}

Object::Object(const Object& rOther)
	: UniqueID{ UniqueIDRegist++ }
{
}

void Object::Free()
{

}

void Object::Editor()
{
	EditName = "Edit " + GetName() + std::to_string(UniqueID);
	ImGui::Checkbox(EditName.c_str(), &bEdit);
}

std::string Object::GetName()
{
	return "Object";
}
