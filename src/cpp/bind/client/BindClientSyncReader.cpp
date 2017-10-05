#include "BindClientSyncReader.h"

#include "impl/GetTimeoutMs.h"

#include <grpc_cb_core/client/client_sync_reader.h>  // for ClientSyncReader
#include <LuaIntf/LuaIntf.h>

#include <functional>  // for std::bind()
#include <string>

using namespace grpc_cb_core;
using namespace LuaIntf;

namespace {

ClientSyncReader GetClientSyncReader(const ChannelSptr& pChannel,
    const std::string& sMethod, const std::string& sRequest,
    const LuaRef& timeoutSec)
{
    int64_t nTimeoutMs = impl::GetTimeoutMs(timeoutSec);
    return ClientSyncReader(pChannel, sMethod, sRequest, nTimeoutMs);
}

// return string|nil, nil means error or end
LuaRef ReadOne(const ClientSyncReader* pReader, lua_State* L)
{
    assert(pReader);
    assert(L);
    std::string sMsg;
    if (pReader->ReadOne(&sMsg))
        return LuaRef::fromValue(L, sMsg);
    return LuaRef(L, nullptr);
}

}  // namespace

namespace bind {

void BindClientSyncReader(const LuaRef& mod)
{
    lua_State* L = mod.state();
    assert(L);
    LuaBinding(mod).beginClass<ClientSyncReader>("ClientSyncReader")
        .addFactory(&GetClientSyncReader)
        .addFunction("read_one",
            [L](const ClientSyncReader* pReader) {
                return ReadOne(pReader, L);
            })
    .endClass();
}  // ClientSyncReader()

}  // namespace bind