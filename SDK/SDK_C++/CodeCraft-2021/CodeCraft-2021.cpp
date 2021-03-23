#define DEBUG

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;
namespace common
{
	inline int32_t read_int()
	{
		char c = std::getchar();
		while (!isdigit(c))
			c = getchar();
		int32_t x = 0;
		while (isdigit(c))
		{
			x = x * 10 + c - '0';
			c = std::getchar();
		}
		return x;
	}
	inline string get_string()
	{
		string res;
		char c = std::getchar();
		while (c != ',')
		{
			res += c;
			c = std::getchar();
		}
		return res;
	}
	inline void remove_lparen()
	{
		char c = std::getchar();
		while (c != '(')
			c = std::getchar();
	}
};

class Server
{
public:
	struct ServerInfo
	{
		std::string server_name;
		int32_t core;
		int32_t memory;
		int32_t buy_cost;
		int32_t daily_cost;
	};

	int32_t num_of_server;
	static const int32_t server_max_size = 100;
	ServerInfo server[server_max_size];
	unordered_map<string, ServerInfo *> servername2info_map;
	void Init()
	{
		cin >> num_of_server;
		for (int32_t i = 0; i < num_of_server; ++i)
		{
			common::remove_lparen();
			server[i].server_name = common::get_string();
			server[i].core = common::read_int();
			server[i].memory = common::read_int();
			server[i].buy_cost = common::read_int();
			server[i].daily_cost = common::read_int();
			servername2info_map.emplace(server[i].server_name, &server[i]);
		}
	}

	void PrintInfo()
	{
		for (int32_t i = 0; i < num_of_server; ++i)
		{
			std::cout << server[i].server_name << ": ";
			printf("%dC %dG $%d $%d\n", server[i].core, server[i].memory, server[i].buy_cost, server[i].daily_cost);
		}
	}

	Server() {}
};

class VirtualMachine
{
public:
	struct VMInfo
	{
		std::string vm_name;
		int32_t core;
		int32_t memory;
		bool deploy_type; // 0 -> 单节点  1 -> 双节点
	};

	int32_t num_of_vm;
	static const int32_t vm_max_size = 1000;
	VMInfo vm[vm_max_size];
	unordered_map<string, VMInfo *> vmname2info_map;
	void Init()
	{
		cin >> num_of_vm;
		for (int32_t i = 0; i < num_of_vm; ++i)
		{
			common::remove_lparen();
			vm[i].vm_name = common::get_string();
			vm[i].core = common::read_int();
			vm[i].memory = common::read_int();
			vm[i].deploy_type = common::read_int();
			vmname2info_map.emplace(vm[i].vm_name, &vm[i]);
		}
	}

	void PrintInfo()
	{
		for (int32_t i = 0; i < num_of_vm; ++i)
		{
			std::cout << vm[i].vm_name << ": ";
			printf("%dC %dG %d\n", vm[i].core, vm[i].memory, vm[i].deploy_type);
		}
	}

	VirtualMachine() {}
};

class VMRequest
{
public:
	int32_t num_of_day;
	struct Req
	{
		bool req_type; // 0 -> delete 1 -> add
		std::string vm_name;
		int32_t vm_id;
	};
	vector<vector<Req>> req;

	void Init()
	{
		cin >> num_of_day;
		for (int32_t i = 0; i < num_of_day; i++)
		{
			vector<Req> today_req;
			int32_t today_num_of_req;
			cin >> today_num_of_req;
			for (int32_t j = 0; j < today_num_of_req; j++)
			{
				Req cur_req;
				common::remove_lparen();
				string req_type_str = common::get_string();
				if (req_type_str == "add")
				{
					cur_req.req_type = 1;
					cur_req.vm_name = common::get_string();
					cur_req.vm_id = common::read_int();
				}
				else
				{
					cur_req.req_type = 0;
					cur_req.vm_id = common::read_int();
				}
				today_req.push_back(cur_req);
			}
			req.push_back(today_req);
		}
	}

	void PrintInfo()
	{
		for (auto day_req : req)
		{
			for (auto cur_req : day_req)
			{
				cout << cur_req.req_type << " " << cur_req.vm_name << " " << cur_req.vm_id << endl;
			}
		}
	}

	VMRequest(){};
};

class RunningVM
{
	// 当前在运行的 vm 序列
public:
	const VirtualMachine &vm;
	unordered_map<int32_t, VirtualMachine::VMInfo *> vmid2info_map;
	set<int32_t> vmid_set;
	const VirtualMachine::VMInfo GetVMInfoById(int32_t vm_id)
	{
		return *vmid2info_map.at(vm_id);
	}
	void AddVM(int32_t vm_id, VirtualMachine::VMInfo *vm_info)
	{
		vmid_set.emplace(vm_id);
		vmid2info_map.emplace(vm_id, vm_info);
	}
	void DelVM(int32_t vm_id)
	{
		vmid_set.erase(vm_id);
	}

	RunningVM(VirtualMachine &vm_) : vm(vm_) {}
};

class BoughtServer
{
	// 已经购买的 server
	// 可能处于关机状态
public:
	const Server &server;
	unordered_map<int32_t, Server::ServerInfo *> serverid2info_map;
	set<int32_t> serverid_set;
	const Server::ServerInfo GetServerInfoById(const int32_t server_id)
	{
		return *serverid2info_map.at(server_id);
	}
	void AddServer(const int32_t server_id, const Server::ServerInfo *server_info)
	{
		serverid_set.emplace(server_id);
		serverid2info_map.emplace(server_id, server_info);
	}
	int64_t EvaluateBuyingCost()
	{
		int64_t buying_cost = 0;
		for (const int32_t &server_id : serverid_set)
		{
			buying_cost += GetServerInfoById(server_id).buy_cost;
		}
		return buying_cost;
	}
	BoughtServer(Server &server_) : server(server_) {}
};
class Greedy
{
public:
	const Server &server;
	const VirtualMachine &vm;
	const VMRequest &request;
	RunningVM &running_vm;
	BoughtServer &bought_server;
	vector<Server::ServerInfo> server_list;
	unordered_map<int32_t, Server::ServerInfo *> vm2server_map;
	set<int32_t> running_serverid_set;

	
	void buy_all_server_at_first()
	{
		for (auto day_req : request.req)
		{
			for (auto each_req : day_req)
			{
				// skip del type
				if (each_req.req_type == 0)
					continue;
				const VirtualMachine::VMInfo cur_vm = *vm.vmname2info_map.at(each_req.vm_name);
			}
		}
	}
	void simulate()
	{

		buy_all_server_at_first();
	}
	Greedy(const Server &server_, const VirtualMachine &vm_, const VMRequest &request_,
		   RunningVM &rvm_, BoughtServer &rserver_)
		: server(server_), vm(vm_), request(request_), run_vm(rvm_), bought_server(rserver_) {}
};

int main()
{
#ifdef DEBUG
	// const char *test_file_path = "../training1.txt";
	// freopen(test_file_path, "r", stdin);
#endif
	// 读取所有输入数据
	Server server;
	VirtualMachine vm;
	VMRequest request;
	server.Init();
	vm.Init();
	request.Init();
#ifdef DEBUG
	server.PrintInfo();
	vm.PrintInfo();
	request.PrintInfo();
#endif
	// 贪心
	RunningVM running_vm(vm);
	BoughtServer bought_server(server);
	Greedy greedy(server, vm, request, running_vm, bought_server);
	greedy.simulate();
	return 0;
}
