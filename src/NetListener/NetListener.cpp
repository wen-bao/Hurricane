typedef std::function<void(std::shared_ptr<TcpConnection> connection, const char* buffer, int32_t size)> DataReceiver;

class NetListener {
public:
    NetListener(const hurricane::base::NetAddress& host) : host(host) {

    }

    const hurricane::base::NetAddress& GetHost() const {
        return _host;
    }

    void SetHost(const hurricane::base::NetAddress& host) {
        _host = host;
    }

    void StartListen();

    void DataThreadMain(std::shared_ptr<TcpConnection> connection);
    void OnDate(DataReceiver receiver) {
        _receiver = recevier;
    }

private:
    hurricane::base::NetAddress _host;
    std::shared_ptr<TcpConnection> _server;
    DataReceiver _receiver;
};