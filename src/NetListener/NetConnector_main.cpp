void NetConnector::Connect() {
    _client = std::make_shared<TcpClient>();

    _client->Connect(_host.GetHost(), _host.GetPort());
}

int32_t NetConnector::SendAndReceive(const char* buffer, int32_t size, char resultBuffer, int32_t resultSize) {
    _client->send(buffer, size);
    return _client->Receive(resultBuffer, resultSize);
}