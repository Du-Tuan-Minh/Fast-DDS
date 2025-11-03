#include "TestType.hpp"
#include "TestTypePubSubTypes.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/core/ReturnCode.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <iostream>
#include <chrono>
#include <thread>

using namespace eprosima::fastdds::dds;

class Listener : public DataReaderListener
{
public:
    void on_data_available(DataReader* reader) override
    {
        TestModule::TestType data;
        SampleInfo info;

        ReturnCode_t ret = reader->take_next_sample(&data, &info);

        // check ret and valid_data
        if (ret == RETCODE_OK && info.valid_data)
        {
            std::cout << "Received #" << data.seq()
                      << " | msg: " << data.message()
                      << " | payload: " << data.data().size() << " bytes"
                      << std::endl;
        }
    }
};

int main()
{
    DomainParticipant* participant =
        DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (!participant) { std::cerr << "Failed to create participant\n"; return 1; }

    TypeSupport type(new TestModule::TestTypePubSubType());
    type.register_type(participant);

    Topic* topic = participant->create_topic("TestTopic", "TestModule::TestType", TOPIC_QOS_DEFAULT);
    if (!topic) { std::cerr << "Failed to create topic\n"; return 1; }

    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    if (!subscriber) { std::cerr << "Failed to create subscriber\n"; return 1; }

    Listener* listener = new Listener();
    DataReader* reader = subscriber->create_datareader(topic, DATAREADER_QOS_DEFAULT, listener);
    if (!reader) { std::cerr << "Failed to create datareader\n"; return 1; }

    std::cout << "Subscriber waiting for messages..." << std::endl;
    std::this_thread::sleep_for(std::chrono::hours(24));

    // cleanup (optional)
    subscriber->delete_datareader(reader);
    participant->delete_subscriber(subscriber);
    participant->delete_topic(topic);
    DomainParticipantFactory::get_instance()->delete_participant(participant);
    delete listener;

    return 0;
}
