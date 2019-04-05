#include "stdafx.h"
#ifdef GTEST
#include "gtest/gtest.h"
#include "Log.h"
#include "Data.h"
#include "CoreAPI.h"
#include "tinyxml2/tinyxml2.h"



using namespace tinyxml2;
using namespace std;


TEST(TinyXml2, XmlClassExampleTest)
{
	EXPECT_TRUE(true);


	XMLDocument doc;
	char* name = "E:/ZMC/Code/C_program/MUL/SourceData/test.xml";
	XMLError result = doc.LoadFile(name);
	EXPECT_EQ(0, doc.ErrorID());


	XMLElement* root = doc.FirstChildElement();

	//Attribute name and value
	cout << root->Name() << ":" << root->FirstAttribute()->Name() << "=" << root->FirstAttribute()->Value() << endl;

	for (XMLElement* mod = root->FirstChildElement();mod != nullptr;mod = mod->NextSiblingElement())
	{
		cout << '\t' << mod->Name() << endl;
		for (XMLElement* card = mod->FirstChildElement();card != nullptr;card = card->NextSiblingElement())
		{
			cout << "\t\t" << card->Name() << ":" << card->FirstAttribute()->Name() << "=" << card->FirstAttribute()->Value() << endl;
			for (XMLElement*leaf = card->FirstChildElement();leaf != nullptr;leaf = leaf->NextSiblingElement())
			{
				cout << "\t\t\t" << leaf->Value() << ":" << (leaf->GetText() ? leaf->GetText() : "NotLeaf") << endl;
			}
		}
	}

	auto card = root->FirstChildElement();
	cout << card->Name();

	doc.Print();
}

TEST(TinyXml2, ConfigurationTest)
{
	char* name = "E:/ZMC/Code/C_program/MUL/SourceData/test.xml";
	ASSERT_EQ(0, Core::AnalysisConfigurationFile(name));
}

#endif