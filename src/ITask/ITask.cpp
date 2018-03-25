//typedef std::vector<std::string> Fields;
class ITask{
public:
    virtual ~ITask(){}

    virtual Fields DeclareFields() const = 0;
};