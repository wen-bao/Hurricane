class MessageLoopManager {
public:
    static MessageLoopManager& GetInstance() {
        static MessageLoopManager manager;
        return manager;
    }

    MessageLoopManager(const MessageLoopManager&) = delete;
    const MessageLoopManager& operator = (const MessageLoopManager&) = delete;

    void Register(const std::string& name, MessageLoop* loop) {
        _messageLoops.insert({ name, std::shared_ptr<MessageLoop>(loop) });
    }

    void PostMessage(const std::string& name, Message* message) {
        auto messageLoopPari = _messageLoops.find(name);
        if( messageLoopPari != _messageLoops.end() ) {
            messageLoopPari->second->PostMessage(message);
        }
    }   

private:
    MessageLoopManager() {}

    std::map<std::string, std::shared_ptr<MessageLoop> > _messageLoops;
};