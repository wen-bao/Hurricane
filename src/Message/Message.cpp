class Message {
public:
    struct Type {
        enum {
            Stop = 0;
        };
    };

    Message (int32_t type) : _type(type) {

    }

    virtual ~Message() {

    }

    int32_t GetType() const {
        return _type;
    }

    void SetType(int32_t type) {
        _type = type;
    }

    private:
        int32_t _type;
};