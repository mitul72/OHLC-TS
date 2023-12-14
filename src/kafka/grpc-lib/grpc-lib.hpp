#include <grpcpp/grpcpp.h>
#include "vector_message.grpc.pb.h"
#include "vector_message.pb.h"
#include <iostream>

class OHLCServiceImpl final : public OHLCService::Service
{
    grpc::Status SendMyVectorList(grpc::ServerContext *context, const vector_message_list *list,
                                  google::protobuf::Empty *reply) override
    {
        return grpc::Status::OK;
    }
};
namespace GRPCLib
{
    void RunServer();
    void StartServer(const vector_message_list list);

} // namespace GRPCLib
