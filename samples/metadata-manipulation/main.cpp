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
 * This sample shows how to read from and write metadata to a file
 * using Video Metadata Framework. This application interactively
 * provides options to display metadata structure, dump metadata items
 * and remove a part or all the metadata from the specified file.
 */

#include <stdexcept>
#include <iostream>
#include "vmf/vmf.hpp"

using namespace std;
using namespace vmf;

void printMetadataStructure(const string& path)
{
    cout << "\nPrinting metadata structure.\n" << "[stream]: " << path << endl;
    MetadataStream ms;
    if (!ms.open(path, MetadataStream::ReadOnly))
        throw std::runtime_error("Can't open MetadataStream");
    auto sNames = ms.getAllSchemaNames();
    for (unsigned int sNum = 0; sNum < sNames.size(); sNum++)
    {
        auto sName = sNames[sNum];
        cout << "\t* (" << sNum << ") [schema]: " << sName << endl;
        if(!ms.load(sName)) // we need to load schema to get items count
            throw std::runtime_error(string("Error loading schema: ") + sName);
        auto mDescs = ms.getSchema(sName)->getAll();
        for (unsigned int setNum = 0; setNum < mDescs.size(); setNum++)
        {
            auto mDesc = mDescs[setNum];
            string setName = mDesc->getMetadataName();
            auto setSize = ms.queryByName(setName).size();
            string fieldsString;
            const char * separator = "";
            auto fields = mDesc->getFields();
            for (auto fd = fields.begin(); fd != fields.end(); fd++)
            {
                fieldsString += separator + fd->name;
                separator = ",";
            }
            cout << "\t\t* (" << sNum << "." << setNum << ") [set]: " << setName << "(" << setSize << " items)" << " {" << fieldsString << "}" << endl;
        }
    }
}

// setName == "" means all sets in the specified schema
// schemaName == "" means all schemas (i.e. all metadata)
void dumpMetadata(const string& path, const string& schemaName = "", const string& setName = "")
{
    cout << "\nDumping metadata: " 
         << (schemaName.empty() ? string("*") : schemaName + '/' + 
                                               (setName.empty() ? string("*") : setName) ) 
         << endl;
    MetadataStream ms;
    if (!ms.open(path, MetadataStream::ReadOnly))
        throw std::runtime_error("Can't open MetadataStream");
    vector<string> schemas;
    if(schemaName.empty()) ms.getAllSchemaNames().swap(schemas);
    else schemas.push_back(schemaName);
    for (unsigned int sNum = 0; sNum < schemas.size(); sNum++)
    {
        auto sName = schemas[sNum];
        cout << "* (" << sNum << ") [schema]: " << sName << endl;
        if(!ms.load(sName))
            throw std::runtime_error(string("Error loading schema: " + sName).c_str());
        vector<shared_ptr<MetadataDesc>>mDescs;
        if(setName.empty()) ms.getSchema(sName)->getAll().swap(mDescs);
        else mDescs.push_back(ms.getSchema(sName)->findMetadataDesc(setName));
        for (unsigned int setNum = 0; setNum < mDescs.size(); setNum++)
        {
            auto mDesc = mDescs[setNum];
            string setName = mDesc->getMetadataName();
            MetadataSet set = ms.queryByName(setName);
            cout << "\t* (" << sNum << "." << setNum << ") [set]: " << setName << "(" << set.size() << " items)" << endl;
            if(set.empty()) continue;
            vector<string> fields(set[0]->getFieldNames());
            int itemNum = 0;
            for (auto item = set.begin(); item != set.end(); item++)
            {
                cout << "\t\t* (" << sNum << "." << setNum << "." << ++itemNum << ") { ";
                const char * separator = "";
                for (auto f = fields.begin(); f != fields.end(); f++)
                {
                    cout << separator << *f << "=";
                    try { cout << (*item)->getFieldValue(*f).toString(); }
                    catch(vmf::Exception& e) { cout << '<' << e.what() << '>'; }
                    separator = ", ";
                }
                cout << " }" << endl;
            }
        }
    }
}

// schemaName == "" means all schemas (i.e. all metadata in the stream)
// setName == "" means the whole schema with descriptions
// schemaName and setName specified means removal all the metadata items from the specified set
void removeMetadata(const string& path, const string& schemaName = "", const string& setName = "")
{
    cout << "\nRemoving metadata: " 
         << (schemaName.empty() ? string("*") : schemaName + '/' + 
                                               (setName.empty() ? string("*") : setName) ) 
         << endl;
    MetadataStream ms;
    if (!ms.open(path, MetadataStream::Update))
        throw std::runtime_error("Can't open MetadataStream");
    vector<string> schemas;
    if(schemaName.empty())
    {
        // remove all metadata
        ms.remove();
    }
    else
    {
        schemas.push_back(schemaName);
        for (unsigned int sNum = 0; sNum < schemas.size(); sNum++)
        {
            auto sName = schemas[sNum];
            cout << "\t* (" << sNum << ") [schema]: " << sName << endl;
            if(!ms.load(sName))
                throw std::runtime_error(string("Error loading schema: ") + sName);
            vector<shared_ptr<MetadataDesc>>mDescs;
            if(setName.empty())
            {
                // remove the whole schema with descriptions
                ms.remove( ms.getSchema(sName) );
            }
            else
            {
                mDescs.push_back(ms.getSchema(sName)->findMetadataDesc(setName));
                for (unsigned int setNum = 0; setNum < mDescs.size(); setNum++)
                {
                    auto mDesc = mDescs[setNum];
                    string setName = mDesc->getMetadataName();
                    MetadataSet set = ms.queryByName(setName);
                    cout << "\t\t* (" << sNum << "." << setNum << ") [set]: " << setName << "(" << set.size() << " items)" << endl;
                    ms.remove(set);
                    set.clear();
                }
            }
        }
    }
    cout << "Saving stream..." << endl;
    ms.save();
    cout << "Done." << endl;
}


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " video_file_path" << endl;
        return -1;
    }

    string filePath(argv[1]);
    cout << "Working with " << filePath << endl;

    long choice;
    do
    {
        cout << "\nPlease make your choice:"
             << "\n1 - print metadata structure"
             << "\n2 - dump metadata items"
             << "\n3 - remove ALL metadata items and descriptions"
             << "\n0 - exit program" << endl;
        cout << "choice: ";
        string s;
        getline(cin, s);
        try { choice = stol(s); }
        catch(...) { choice = -1; }

        try
        {
            switch(choice)
            {
                case 1: printMetadataStructure(filePath); break;
                case 2: dumpMetadata(filePath); break;
                case 3: removeMetadata(filePath, ""); break;
            }
        }
        catch(std::exception& e)
        {
            cout << "\nOperation failed: " << e.what() << endl;
        }
    } while(choice!=0);

    cout << "Good bye!" << endl;
    //cin.ignore(); //wait for press enter
    return 0;
}
