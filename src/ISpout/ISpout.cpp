namespace hurricane{
    namespace spout{
        class ISpout : public base::ITask{
        public:
            virtual void Open(base::OutputCollector& outputCollector) = 0;
            virtual void Close() = 0;
            virtual void Execute() = 0;

            virtual ISpout* Clone() const = 0;
        };
    }    
}