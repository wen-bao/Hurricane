Dispatcher
.OnCommand(Command::Type::Join,
    [&](hurricane::base::Variants args, std::shared_ptr<TcpConnection> src) -> void {
        std::string managerName = args[0].GetStringValue();

        //Create manager node
        Node manager(managerName, SUPERVISOR_ADDRESSES.at(managerName));
        manager.SetStatus(Node::Status::Alived);
        manager[managerName] = manager;

        //Create empty tasks
        spoutTask[managerName] = Task(EXECUTOR_CAPACITY);
        boltTasks[managerName] = Task(EXECUTOR_CAPACITY);

        Command command(Command::Type::Response, {
            std::string("president")
        });

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(commandBytes.data(), commandBytes.size());

        if( manager.size() == SUPERVISOR_ADDRESSES.size() ) {
            std::cout << "All managers started" << std::endl;
            dispatchTasks(managers, spoutTask, boltTasks, topology);
        }
    }
);

.OnCommand(Command::Type::Alived, 
    [&](hurricane::base::Variants args, std::shared_ptr<TcpConnection> src) -> void {
        std::string managerName = args[0].GetStringValue();
        managers[managerName].Alived();

        Command command(Command::Type::Response, {
            std::string("president")
        });
        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(commandBytes.data(), commandBytes.size());
    }
);

netListener.OnData([&](std::shared_ptr<TcpConnection> connection, 
    const char* buffer, int32_t size) -> void {
        ByteArray receivedData(buffer, size);
        DataPackage receivedPackage;
        receivedPackage.Deserialize(receivedData);

        Command command(receivedPackage);
        command.SetSrc(connection);

        dispatcher.Dispatch(command);
    }
);

netListener.StartListen();

//Manager节点初始化
ITopology* topology = GetTopology();

std::string managerName(argv[1]);
std::cerr << "Manager " << managerName << " started" << std::endl;

std::thread aliveThread(AlivedThreadMain, managerName);
aliveThread.detach();

Manager manager(topology);

netListener netListener(SUPERVISOP_ADDRESSES.at(managerName));
CommandDispatcher dispatcher;


void AliveThreadMain(const std::string& name) {
    ManagerCommander commander(NIMBUS_ADDRESS, name);
    commander.Join();

    while( 1 ) {
        commander.Alive();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


//Manager对StartBolt命令的处理

dispatcher
.OnCommand(Command::Type::StartBolt, 
    [&](hurricane::base::Variants args, std::shared_ptr<TcpConnection> src) -> void {
        Command command(Command::Type::Response, {
            std::strign(managerName)
        });

        std::string taskName = args[0].GetStringValue();
        int executorIndex = args[1].GetIntValue();

        std::cout << "Start Bolt" << std::endl;
        std::cout << "Bolt name: " << taskName << std::endl;
        std::cout << "Executor index: " << executorIndex << std::endl;

        manager.StartBolt(taskName, executorIndex);

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(commandBytes.data(), commandBytes.size());
    }
);

//Manager对StartSpout命令的处理

.OnCommand(Command::Type::StartSpout, 
    [&](hurricane::base::Variants args, std::shared_ptr<TcpConnection> src) -> void {
        Command command(Command::Type::Response, {
            std::string(managerName)
        });

        std::string taskName = args[0].GetStringValue();
        int executorIndex = args[1].GetIntValue();

        std::cout << "Start Spout" << std::endl;
        std::cout << "Spout name: " << taskName << std::endl;
        std::cout << "Executor index: " << executorIndex << std::endl;

        manager.StartSpout(taskName, executorIndex);

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(commandBytes.data(), commandBytes());
    }
)

//Manager处理数据元组

.OnCommand(Command::Type::Data,
    [&](Variants args, std::shared_ptr<TcpConnection> src) -> void {
        
        std::string srcManagerName = args[0].GetStringValue();
        args.erase(args.begin());
        int32_t taskIndex = args[0].GetIntValue();
        args.erase(args.begin());

        Values values;
        for ( auto& arg : args ) {
            values.push_back(Valuse::FromVariant(arg));
        }

        manager.PostValues(taskIndex, values);

        Command command(Command::Type::Response,{
            std::string(managerName)
        });

        ByteArray commandBytes = command.ToDataPackage().Serialize();
        src->Send(commandBytes.data(), commandBytes.size());
    }
);
