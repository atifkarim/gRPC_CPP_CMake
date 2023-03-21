/**
 * @author Atif
 * @brief This file contains declaration of request function by client
 * @date 2023-03-21
 *
 */

#include <big_data.pb.h>
#include <addressbook.grpc.pb.h>
#include <collection.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>

#include <cmath>
#include <numeric>
#include <sstream>
#include <iostream>

//  Class to hold all properties related to client
class grpc_client
{
	public:
		grpc_client(std::shared_ptr<grpc::Channel> channel, unsigned int button_)
		            : stub(demo_grpc::Demo_gRPC_Service::NewStub(channel)), detected_task(task_name(button_)) , button(button_) {}

		// Function to process address request
		void Address_Request();

		// This is a dummy function which doubles the value if it is less than
		void Double_Value_Request();

		// Function to process sending stream data
		void Data_Stream_Request();

	private:
		std::unique_ptr<demo_grpc::Demo_gRPC_Service::Stub> stub;

		// This function returns string data of the desired task
		/**
		 * @param task_number
		 * @return std::string
		 */
		std::string task_name(unsigned int task_number)
		{
		    std::string tasks[] = {"Address_Book", "Check Value", "Streaming Data(Chunk)"};
		    return tasks[task_number];
		}

		// This variable use to assign desired task name
		std::string detected_task;

		// This variable's value is provided by the user
		unsigned int button;

		// Calculates the number of chunk
		/**
		 * \param [in] data_size Total size of given/provided data which will be passed as chunk. Eg: size of the vector
		 * \param [in] chunk_size Chunk size in KiloByte. This chunk size will be given by the user. Right now less than 64 KB is not allowed
		 * \param [in] sample Data sample in each chunk
		 * \param [in] total_chunk NUmber of total chunk
		 */
		void get_chunk_number(uint64_t data_size,
		                      uint64_t chunk_size,
		                      uint64_t &sample,
		                      uint32_t &total_chunk);
};
