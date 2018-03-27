namespace hurricane {
    namespace bolt {
        class IBolt : public base::ITask {
        public:
            virtual void Prepare(base::OutputCollector& outputCollector) = 0;
            virtual void Cleanup() = 0;
            virtual void Execute(const base::Values& values) = 0;

            virtual IBolt* Clone() const = 0;
        };
    }
}