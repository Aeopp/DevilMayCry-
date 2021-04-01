#include "stdafx.h"
#include "..\Header\TestObject.h"
#include <fstream>
#include <ostream>

void TestObject::Free()
{
}

TestObject* TestObject::Create()
{
	return new TestObject{};
}

HRESULT TestObject::Ready()
{
	auto _StaticMesh = Resources::Load<StaticMesh>(TEXT("../../Resource/TestMesh/Move.fbx"));

	using namespace rapidjson;
	std::filesystem::path TargetPath = "..\\..\\Resource\\Dummy\\TestJson.json";
	StringBuffer StrBuf;
	PrettyWriter<StringBuffer> Writer(StrBuf);
	//  Cell Information Write...
	Writer.StartObject();
	Writer.Key("Test Json");
	Writer.StartArray();
	{
		{
			Writer.StartObject();
			{
				Writer.Key("Index");
				Writer.Uint(777u);

				Writer.Key("Name");
				Writer.String("Nero");

				Writer.Key("Value_1");
				Writer.Double(123.456f);

				Writer.Key("Value 2");
				Writer.Double(999.999f);
			}
			Writer.EndObject();
		}
		Writer.EndArray();

		Writer.EndObject();
		TargetPath.replace_extension("json");
		std::ofstream Of{ TargetPath };
		Of << StrBuf.GetString();
	};

	return S_OK;
};

HRESULT TestObject::Awake()
{
	return S_OK;
}

HRESULT TestObject::Start()
{
	return S_OK;
}

UINT TestObject::Update(const float _fDeltaTime)
{
	ImGui::Begin("ewqe");
	ImGui::Text("Hello ! ");
	ImGui::End();

	return 0;
}

UINT TestObject::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT TestObject::Render()
{

	return S_OK;
}

void TestObject::OnEnable()
{
}

void TestObject::OnDisable()
{
}
