class NetConnector {
public:
    NetConnector(const hurricane::base::NetAddress& host) :
        _host(host) {

    }

    const hurricane::base::NetAddress& GetHost() const {
        return _host;
    }

    void SetHost(const hurricane::base::NetAddress& host) {
        _host = host;
    }

    void Connect();
    int32_t SendAndReceive(const char* buffer, int32_t size, char* resultBuffer, int32_t resultSize);

private:
    hurricane::base::NetAddress _host;
    std::shared_ptr<TcpClient> _client;
};