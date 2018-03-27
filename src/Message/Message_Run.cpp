MessageLoop::MessageLoop() {
    _threadId = GetCurrentThreadId();
}

void MessageLoop::Run() {
    MSG msg;

    while( GetMessage(&msg, 0, 0, 0) ) {
        auto handler = _messageHandlers.find(msg.message);

        if( handler != _messageHandlers.end() ) {
            handler->second( (Message*)(msg.wParam));
        }

        DispatchMessage(&msg);

        if( msg.message == Message::Type::Stop) {
            break;
        }
    }
}