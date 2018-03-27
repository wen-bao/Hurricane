ITopology* topology - GetTopology();

std::map<std::string, Node> managers;
std::map<std::string, Tasks> spoutTasks;
std::map<std::string, Tasks> boltTasks;
std::map<std::pair<std::string, std::string>, std::pair<Node, int> > fieldDestinations;

NetListener netListener(NIMBUS_ADDRESS);
CommandDIspatcher dispatcher;
