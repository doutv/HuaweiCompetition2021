#define DEBUG

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
#include <functional>
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
		while (c == ' ' || c == ',' || c == '(')
			c = getchar();
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
	unordered_map<string, reference_wrapper<ServerInfo>> servername2info_map;
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
			servername2info_map.emplace(server[i].server_name, server[i]);
		}
	}
	ServerInfo &GetServerInfoByName(string server_name) const
	{
		return servername2info_map.at(server_name);
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
	unordered_map<string, reference_wrapper<VMInfo>> vmname2info_map;
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
			vmname2info_map.emplace(vm[i].vm_name, vm[i]);
		}
	}
	VMInfo &GetVMInfoByName(string vm_name) const
	{
		return vmname2info_map.at(vm_name);
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
	vector<vector<Req>> req_list;

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
			req_list.push_back(today_req);
		}
	}

	void PrintInfo()
	{
		for (auto day_req : req_list)
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
	unordered_map<int32_t, reference_wrapper<VirtualMachine::VMInfo>> vmid2info_map;
	set<int32_t> vmid_set;
	VirtualMachine::VMInfo &GetVMInfoById(int32_t vm_id) const
	{
		return vmid2info_map.at(vm_id);
	}
	void AddVM(int32_t vm_id, VirtualMachine::VMInfo &vm_info)
	{
		vmid_set.emplace(vm_id);
		vmid2info_map.emplace(vm_id, vm_info);
	}
	void DelVM(int32_t vm_id)
	{
		vmid_set.erase(vm_id);
	}

	RunningVM(const VirtualMachine &vm) : vm(vm) {}
};

class BoughtServer
{
	// 已经购买的 server
	// 可能处于关机状态
public:
	const Server &server;
	unordered_map<int32_t, reference_wrapper<Server::ServerInfo>> serverid2info_map;
	set<int32_t> serverid_set;
	Server::ServerInfo &GetServerInfoById(int32_t server_id) const
	{
		return serverid2info_map.at(server_id);
	}
	int64_t BuyServer(int32_t server_id, Server::ServerInfo &server_info)
	{
		serverid_set.emplace(server_id);
		serverid2info_map.emplace(server_id, server_info);
		return server_info.buy_cost;
	}
	BoughtServer(const Server &server) : server(server) {}
};
class Greedy
{
public:
	const Server &server;
	const VirtualMachine &virtual_machine;
	const VMRequest &request;
	RunningVM running_vm;
	BoughtServer bought_server;

	vector<Server::ServerInfo> server_list;
	unordered_map<int32_t, int32_t> vmid2serverid_map;
	set<int32_t> running_serverid_set;
	set<int32_t> sleeping_serverid_set;
	int64_t total_cost;
	int32_t today;

	int64_t EvaluateDailyCost()
	{
		int64_t daily_cost = 0;
		for (int32_t server_id : running_serverid_set)
		{
			daily_cost += bought_server.GetServerInfoById(server_id).daily_cost;
		}
		return daily_cost;
	}
	void AssignServer2VM(int32_t server_id, int32_t vm_id, VirtualMachine::VMInfo &vm_info)
	{
		vmid2serverid_map.emplace(vm_id, server_id);
		running_vm.AddVM(vm_id, vm_info);
	}
	bool TryAssignSleepingServer2VM(VMRequest::Req req)
	{
		VirtualMachine::VMInfo &vm_info = virtual_machine.GetVMInfoByName(req.vm_name);
		for (int32_t server_id : sleeping_serverid_set)
		{
			Server::ServerInfo &server_info = bought_server.GetServerInfoById(server_id);
			if (server_info.core >= vm_info.core && server_info.memory >= vm_info.memory)
			{
				// greedy assign sleeping server to vm
				AssignServer2VM(server_id, req.vm_id, vm_info);
				sleeping_serverid_set.erase(server_id);
				running_serverid_set.emplace(server_id);
				return true;
			}
		}
		return false;
	}
	void BuyServerBasedOnReq(VMRequest::Req req)
	{
	}
	void Buy()
	{
		// total_cost += bought_server.BuyServer();
		for (const VMRequest::Req &req : request.req_list.at(today))
		{
			// skip del type
			if (req.req_type == 0)
				continue;
			if (!TryAssignSleepingServer2VM(req))
				BuyServerBasedOnReq(req);
		}
	}
	void Migrate()
	{
	}
	void ProcessRequest()
	{
	}
	void Simulate()
	{
		// 对于每一天的操作，会先按顺序执行选手输出的购买操作，然后按顺序执行选手输出的迁移操作，最后按顺序执行当天的创建和删除操作。
		// 输入中可能存在当天创建后当天删除的虚拟机，对于这些虚拟机请求，你依然需要按顺序正常处理。
		// BuyAllServerAtFirst();
		for (today = 0; today < request.num_of_day; today++)
		{
			Buy();
			Migrate();
			ProcessRequest();
			total_cost += EvaluateDailyCost();
		}
	}
	Greedy(const Server &server, const VirtualMachine &virtual_machine, const VMRequest &request)
		: server(server), virtual_machine(virtual_machine), request(request), running_vm(virtual_machine), bought_server(server) {}
};

int main()
{
#ifdef DEBUG
	// const char *test_file_path = "../training1.txt";
	// freopen(test_file_path, "r", stdin);
#endif
	// 读取所有输入数据
	Server server;
	VirtualMachine virtual_machine;
	VMRequest request;
	server.Init();
	virtual_machine.Init();
	request.Init();
#ifdef DEBUG
	server.PrintInfo();
	virtual_machine.PrintInfo();
	request.PrintInfo();
#endif
	// 贪心
	Greedy greedy(server, virtual_machine, request);
	greedy.Simulate();
	return 0;
}
