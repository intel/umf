/* 
 * Copyright 2015 Intel(r) Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
 
/*
* This sample shows how to use custom schema in xml file.
*/

#include <vmf/vmf.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stdexcept>

using namespace vmf;
using namespace std;
void dumpMetadataDesc(shared_ptr<MetadataDesc> spDesc)
{
	if (spDesc == nullptr)
		return;

	cout << "Metadata: " << spDesc->getMetadataName() << endl;
	vector<FieldDesc> fields = spDesc->getFields();
	for (FieldDesc field : fields)
	{
		cout << "\t[" << field.name << "]: " << Variant::typeToString(field.type) << std::endl;
	}
}
void demoLoadSchemaFromFile(const string& schemaFilePath, const string& schemaName)
{
	MetadataStream stream;

	try
	{
		// Open schema file
		std::ifstream inputFile(schemaFilePath);

		// Read schema content into memory string
		stringstream ss;
		ss << inputFile.rdbuf();

		// Parse schema content and load it into stream
        FormatXML xml;
		stream.deserialize(ss.str(), xml);

		// Get a pointer to the schema
		shared_ptr<MetadataSchema> spSchema = stream.getSchema(schemaName);
		if (spSchema == nullptr)
            throw runtime_error("Did not find schema in the xml file!");

		// Get a pointer to the metadata description structure
		shared_ptr<MetadataDesc> spMetadataDesc = spSchema->findMetadataDesc("table2");
		if (spMetadataDesc == nullptr)
            throw runtime_error("Did not find metadata <table2> in the schema.");

		// Found everything!
		std::cout << "Found schema and metadata!" << std::endl;
		std::cout << "Schema: " << spSchema->getName() << std::endl;
		dumpMetadataDesc(spMetadataDesc);

		stream.close();
	}
	catch (std::exception& e)
	{
		std::cout << "Operation fialed: " << e.what() << std::endl;
		stream.close();
	}
}

int main(int argc, char* argv[])
{
	string schemaFilePath("demo_schema.xml");
	string schemaName("vmf://ns.intel.com/vmf/demo-schema-1.0");
	
	string appPath = argv[0];
#ifdef WIN32
	char delim = '\\';
#else
	char delim = '/';
#endif
	size_t pos = appPath.find_last_of(delim);

	string workingPath;
	if (pos != string::npos)
	{
		workingPath = appPath.substr(0, pos + 1);
	}

	// Run demo
	demoLoadSchemaFromFile(workingPath + schemaFilePath, schemaName);

    return 0;
}
