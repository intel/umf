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
* This sample shows how to use predefined STD schema and built-in predefined metadata fields.
*/

#include <vmf/vmf.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

#define VIDEO_FILE "BlueSquare.avi"
#define WORKING_FILE "sample_std-schema.avi"

using namespace vmf;

static void copyFile(const std::string& src, const std::string& dest)
{
    std::ifstream source;
    std::ofstream destination;

    source.open(src, std::ios_base::binary | std::ios_base::in);
    destination.open(dest, std::ios_base::binary | std::ios_base::out);
    if (!source.is_open() || !destination.is_open())
        VMF_EXCEPTION(vmf::Exception, "Couldn't copy " + src + " to " + dest);
    destination << source.rdbuf();
    destination.close();
    source.close();
}

int main(int argc, char* argv[])
{
    MetadataStream stream;

    try
    {
        copyFile(VIDEO_FILE, WORKING_FILE);

        if(!stream.open(WORKING_FILE, MetadataStream::Update))
            throw std::runtime_error("Can't open MetadataStream");

        //create predefined STD schema
        auto stdSchema = MetadataSchema::getStdSchema();

        stream.addSchema(stdSchema);

        auto momentsDesc = stdSchema->findMetadataDesc("moment");


        std::shared_ptr<Metadata> md_moment_1(new Metadata(momentsDesc));//generate location metadata fields
        md_moment_1->setFieldValue("title", "Mountain"); //mandatory field
        md_moment_1->setFieldValue("description", "Nice downhill");//optional field
        md_moment_1->setTimestamp(getTimestamp(), 1000);//built-in timestamp
        stream.add(md_moment_1);

        std::shared_ptr<Metadata> md_moment_2(new Metadata(momentsDesc));
        md_moment_2->setFieldValue("title", "Andrey's Birthday");
        md_moment_2->setTimestamp(getTimestamp(2014, 9, 8), 39000);
        stream.add(md_moment_2);

        std::shared_ptr<Metadata> md_moment_3(new Metadata(momentsDesc));
        md_moment_3->setFieldValue("title", "Pain");
        md_moment_3->setFieldValue("description", "I'm falling down the bike");
        md_moment_3->setTimestamp(getTimestamp(1970, 1, 2));
        stream.add(md_moment_3);

        stream.save();
        stream.close();

        //reopen stream with saved metadata
        stream.reopen();

        //load metadata in memory
        stream.load();

        //print stored info about each metadata item (each moment in this sample)
        auto items = stream.getAll();
        for(auto it = items.begin(); it != items.end(); it++)
        {
            auto item = *it;
            //print moment's title (non-optional field)
            std::cout << "Title: " << item->getFieldValue("title").toString() << std::endl;

            //print moment's description if it contains (optional field)
            if (item->hasField("description"))
                std::cout << "Description: " << item->getFieldValue("description").toString() << std::endl;
            /*
            // alternative code can be:
            vmf::Variant v = item->getFieldValue("description");
            if(! v.isEmpty())
                std::cout << "Description: " << v.toString() << std::endl;
            */

            //print timestamp (time and duration if it's avaliable)
            long long time = item->getTime(), duration = item->getDuration();
            if( time >= 0)
            {
                time_t time_in_seconds = time/1000;
#ifdef WIN32
                tm t;
                localtime_s(&t, &time_in_seconds);
                std::cout << "Time: " << time << "ms " << std::put_time(&t, "(%Y/%m/%d %H:%M:%S)") << std::endl;
#else
                //std::put_time is missed in gcc 4.9
                std::cout << "Time: " << time << "ms  that is: " << std::ctime(&time_in_seconds);
#endif
            }
            if(duration > 0)
                std::cout << "Duration: " << duration << "ms" << std::endl;

            std::cout << std::endl;
        }

        stream.remove();
        stream.save();
        stream.close();
    }
    catch(std::exception& e)
    {
        std::cout << "Operation fialed: " << e.what() << std::endl;

        stream.remove();
        stream.save();
        stream.close();

        return -1;
    }

    return 0;
}
