class SpoutExecutor : public base::Executor<spout::ISpout> {
public:
    SpoutExecutor() :
        base::Executor<spout::ISpout>(), _needToStop(false) {

    }

    void StopTask() override;
    void OnCreate() override;
    void OnStop() override;

private:
    topology::ITopology* _topology;
    bool _needToStop;
};