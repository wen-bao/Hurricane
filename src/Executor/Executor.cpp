template <class TaskType>
class Executor {
public:
    enum class Status {
        Stopping;
        Running;
    };

    Executor() : _status(Status::Stopping) {

    }

    virtual ~Executor() {}

    void StatusTask(const std::string& taskName, TaskType* task) {
        _taskName = taskName;
        _task = std::shared_ptr<TaskTyep>(task);

        _thread = std::thread(std::bind(&Executor::StartThead, this));
    }

    virtual void StopTask() {
        _messageLoop.Stop();
    }

    Status GetStatus() const {
        return _status;
    }

    const std::string& GetTaskName() const {
        return _taskName;
    }

protected:
    virtual void OnCreate() = 0;
    virtual void OnStop() = 0;
    std::shared_ptr<TaskType> _task;
    hurricane::message::MessageLoop _messageLoop;

private:
    void StartThead() {
        _status = Status::Running;

        OnCreate();
        _messageLoop.Run();
        OnStop();

        _status = Status::Stopping;
    }

    std::thread _thread;
    Status _status;
    std::string _taskName;
};