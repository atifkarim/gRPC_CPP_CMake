#include <big_data.pb.h>
#include <addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>

#include <cmath>
#include <numeric>
#include <sstream>
#include <iostream>

class grpc_client
{
	public:
		grpc_client(std::shared_ptr<grpc::Channel> channel, unsigned int button_)
		            : stub(demo_grpc::AddressBook::NewStub(channel)), detected_area_name(area_mame(button_)) , button(button_) {}

		void address_request();
		void check_value_request();
		void data_chunk_stream_request();

	private:
		std::unique_ptr<demo_grpc::AddressBook::Stub> stub;
		std::string area_mame(unsigned int area_index){
		    std::string area_list[] = {"Address_Book",
		                               "Check Value",
		                               "Streaming Data(Chunk)"};
		                               return area_list[area_index];
		}
		std::string detected_area_name;
		unsigned int button;
		void get_chunk_number(uint64_t data_size,
		                      uint64_t chunk_size,
		                      uint64_t &sample,
		                      uint32_t &total_chunk);
};