const int DATA_BUFFER_SIZE = 65535;

void NetListener::StartListen() {
    _server = std::make_shared<TcpServer>();

    _server->Listen(_host.GetHost(), _host.GetPort());

    while(1) {
        std::shared_ptr<TcpConnection> connection = std::shared_ptr<TcpConnection>(_server->Accept());

        std::cerr << "A client is connected" << std::endl;

        std::thread dataThread(std::bind(&NetListener::DataThreadMain, this, std::placeholders::_1), connection);
        dataThread.detach(); 
    }
}

void NetListener::DataThreadMain(std::shared_ptr<TcpConnection> connection) {
    std::cerr << connection << std::endl;
    int32_t = _lostTime = 0;

    try {
        char buffer[DATA_BUFFER_SIZE];
        while(1) {
            int32_t length = connection->Receive(buffer, DATA_BUFFER_SIZE);

            if( !length ) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                _lostTime ++;
                if( _lostTime < 10 ) {
                    continue;
                } else {
                    break;
                }
            } else {
                _lostTime = 0;
            }

            _receiver(connection, buffer, length);
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
} 
