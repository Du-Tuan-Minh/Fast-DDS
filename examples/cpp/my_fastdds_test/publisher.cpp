#include "TestType.hpp"
#include "TestTypePubSubTypes.hpp"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/core/ReturnCode.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include <array>

using namespace eprosima::fastdds::dds;

int main()
{
    //create participant
    DomainParticipant* participant =
        DomainParticipantFactory::get_instance()->create_participant(
            0, PARTICIPANT_QOS_DEFAULT);

    if (!participant)
    {
        std::cerr << "Failed to create DomainParticipant" << std::endl;
        return 1;
    }

    //register type
    TypeSupport type(new TestModule::TestTypePubSubType());
    type.register_type(participant);

    //Create topic 
    Topic* topic = participant->create_topic(
        "TestTopic",
        "TestModule::TestType",
        TOPIC_QOS_DEFAULT);

    if (!topic)
    {
        std::cerr << "Failed to create Topic" << std::endl;
        return 1;
    }

    //create publisher + writer
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    DataWriter* writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT);

    if (!writer)
    {
        std::cerr << "Failed to create DataWriter" << std::endl;
        return 1;
    }

    //send data
    TestModule::TestType data;
    data.message("Hello from Fast DDS!");

    int count = 0;
    while (true)
    {
        data.seq(++count);

        auto now = std::chrono::high_resolution_clock::now();
        auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            now.time_since_epoch())
            .count();
        data.send_timestamp(now_ns);

        auto ret = writer->write(&data);

        if (ret == RETCODE_OK) 
        {
            std::cout << "Sent" << count << std::endl;
        }
        else
        {
            std::cout << "Write failed with code: " << static_cast<int>(ret) << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}