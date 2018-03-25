class Values : public std::vector<Value> {
public:
    Value() = default;
    Value(std::initializer_list<Value> values) : std::vector<Value>(values) {

    }

    Value& operator[] (size_t index) {
        return std::vector<Value>::operator[](index);
    }

    const Value& operator[] (size_t index) const {
        return std::vector<Value>::operator[](index);
    }
};