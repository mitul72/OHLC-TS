#include "grpc-lib.hpp"

namespace GRPCLib
{
    void RunServer()
    {
        std::string server_address("0.0.0.0:50051");
        OHLCServiceImpl service;

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        server->Wait();
    }

    void StartServer(const vector_message_list list)
    {
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
        std::unique_ptr<OHLCService::Stub> stub = OHLCService::NewStub(channel);

        google::protobuf::Empty reply;
        grpc::ClientContext context;

        grpc::Status status = stub->SendMyVectorList(&context, list, &reply);
        if (!status.ok())
        {
            std::cout << "failed: " << status.error_message() << std::endl;
            return;
        }
    }

} // namespace GRPCLib
