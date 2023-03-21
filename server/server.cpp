/**
 * @file server.cpp
 * @author MD Atif Bin Karim
 * @brief This file process all request to server
 * @date 21-03-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>

#include <big_data.pb.h>
#include <addressbook.grpc.pb.h>
#include <number_data.grpc.pb.h>
#include <collection.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

#include "address_response.h"
#include "value_response.h"
#include "stream_response.h"
#include "random_function.h"
#include "data_chunk_stream_response.h"

class All_gRPC_Service final : public demo_grpc::Demo_gRPC_Service::Service
{
	public:
		virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
		{
			std::cout << "GetAddress RPC is executed" << std::endl;
			std::cout << "--------------------------" << std::endl;
			switch (request->choose_area())
			{
			case 0:
				try
				{
					Set_Value_Address(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;

			case 2:
				try
				{
					set_stream_value(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;
			}

			std::cout << "Information of " << request->choose_area() << " is sent to Client" << std::endl;
			return grpc::Status::OK;
		}

	// function to work with ProcessNumber service

	virtual ::grpc::Status ProcessNumber(::grpc::ServerContext* context, const ::demo_grpc::Number_Request* request, ::demo_grpc::Number_Response* response)
		{
			std::cout << "ProcessNumber RPC is executed" << std::endl;
			std::cout << "--------------------------" << std::endl;
			try
			{
				Double_Value(request, response);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
			}

			std::cout << "Information of " << request->name() << " is sent to Client" << std::endl;
			return grpc::Status::OK;
		}

	// function to work with Steam data using chunk
	virtual ::grpc::Status Stream_Chunk_Service(::grpc::ServerContext* context,
	                                            ::grpc::ServerReaderWriter< ::demo_grpc::Large_Data_Response, ::demo_grpc::Large_Data_Request>* stream)
	{
		std::cout << "Stream chunk RPC is executed" << std::endl;
		std::cout << "----------------------------" << std::endl;
		Data_Stream_Response(stream);

		return grpc::Status::OK;
	}
};

void RunServer()
{
	std::cout << "Server is running" << std::endl;
	std::cout << "grpc Version: " << grpc::Version() << std::endl;
	std::string server_address = "localhost:50051";
	std::cout << "Address of server: " << server_address << std::endl;

	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	All_gRPC_Service my_service;
	builder.RegisterService(&my_service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();
}

int main(int argc, char* argv[])
{
	RunServer();
	return 0;
}
