#define DEBUG

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
namespace common
{
	inline int read_int()
	{
		char c = std::getchar();
		while (!isdigit(c))
			c = getchar();
		int x = 0;
		while (isdigit(c))
		{
			x = x * 10 + c - '0';
			c = std::getchar();
		}
		return x;
	}
	inline std::string get_string()
	{
		std::string res;
		char c = std::getchar();
		while (c == ' ')
			c = std::getchar();
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
		int core1, core2;
		int memory1, memory2;
		int core, memory;
		int buy_cost;
		int daily_cost;
		int id;
	};

	int num_of_server;
	static const int server_max_size = 100;
	ServerInfo server[server_max_size];
	std::unordered_map<std::string, ServerInfo *> servername2info_map;
	void Init()
	{
		std::cin >> num_of_server;
		for (int i = 0; i < num_of_server; ++i)
		{
			common::remove_lparen();
			server[i].server_name = common::get_string();
			server[i].core1 = common::read_int() / 2;
			server[i].core2 = server[i].core1;
			server[i].core = server[i].core1 + server[i].core2;
			server[i].memory1 = common::read_int() / 2;
			server[i].memory2 = server[i].memory1;
			server[i].memory = server[i].memory1 + server[i].memory2;
			server[i].buy_cost = common::read_int();
			server[i].daily_cost = common::read_int();
			servername2info_map.emplace(server[i].server_name, &server[i]);
		}
	}
	const ServerInfo &GetServerInfoByName(const std::string &server_name) const
	{
		return *servername2info_map.at(server_name);
	}
	void PrintInfo()
	{
		for (int i = 0; i < num_of_server; ++i)
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
		int core;
		int memory;
		bool deploy_type; // 0 -> 单节点  1 -> 双节点
		bool operator == (const VMInfo & next) const
		{
			return core < next.core;
		}
	};

	int num_of_vm;
	static const int vm_max_size = 1000;
	VMInfo vm[vm_max_size];
	std::unordered_map<std::string, VMInfo *> vmname2info_map;
	void Init()
	{
		std::cin >> num_of_vm;
		for (int i = 0; i < num_of_vm; ++i)
		{
			common::remove_lparen();
			vm[i].vm_name = common::get_string();
			vm[i].core = common::read_int();
			vm[i].memory = common::read_int();
			vm[i].deploy_type = common::read_int();
			vmname2info_map.emplace(vm[i].vm_name, &vm[i]);
		}
	}
	const VMInfo &GetVMInfoByName(const std::string &vm_name) const
	{
		return *vmname2info_map.at(vm_name);
	}
	void PrintInfo()
	{
		for (int i = 0; i < num_of_vm; ++i)
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
	int num_of_day;
	struct Req
	{
		bool req_type; // 0 -> delete 1 -> add
		std::string vm_name;
		int vm_id;
	};
	std::vector<std::vector<Req>> req;

	void Init()
	{
		std::cin >> num_of_day;
		for (int i = 0; i < num_of_day; i++)
		{
			std::vector<Req> today_req;
			int today_num_of_req;
			std::cin >> today_num_of_req;
			for (int j = 0; j < today_num_of_req; j++)
			{
				Req cur_req;
				common::remove_lparen();
				std::string req_type_str = common::get_string();
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
				std::cout << cur_req.req_type << " " << cur_req.vm_name << " " << cur_req.vm_id << std::endl;
			}
		}
	}

	VMRequest(){};
};

class BoughtServer
{
	public:
		int num;
		std::vector<Server::ServerInfo> serverinfo;

		Server::ServerInfo * GetServerInfoById(const int server_id)
		{
			return &serverinfo[server_id];
		}

		int getNum()
		{
			return num;
		}

		Server::ServerInfo * BuyServer(Server::ServerInfo server_info)
		{
			++num;
			serverinfo.push_back(server_info);
			return &serverinfo[num - 1];
		}

		BoughtServer() : num(0) {}
};

class FirstFitAlgorithm
{
	public:
		struct Request {
			int delta_core1, delta_core2, delta_memory1, delta_memory2;
			Server::ServerInfo * server_info;
			Request(int __delta_core1, int __delta_core2, int __delta_memory1, int __delta_memory2, Server::ServerInfo * __server_info)
			{
				delta_core1 = __delta_core1;
				delta_core2 = __delta_core2;
				delta_memory1 = __delta_memory1;
				delta_memory2 = __delta_memory2;
				server_info = __server_info;
			}
		};
		const Server &server;
		const VirtualMachine &virtual_machine;
		const VMRequest &request;
		BoughtServer bought_server;
		std::vector<Server::ServerInfo> server_list;
		long long total_cost;
		int today;
		int daily_cost;
		int purchase_num;
		std::unordered_map<std::string, int> purchase_machine;
		std::unordered_map<int, Request *> AddRequest;

		bool isSleeping(Server::ServerInfo * server_info)
		{
			if (server_info->core == server_info->core1 + server_info->core2 && \
				server_info->memory == server_info->memory1 + server_info->memory2)
				return true;
			return false;
		}

		bool TryAssignSleepingServer2VM(const VirtualMachine::VMInfo vm, int id)
		{
			for (int server_id = 0; server_id < bought_server.getNum(); ++server_id)
			{
				Server::ServerInfo * server_info = bought_server.GetServerInfoById(server_id);
				if (vm.deploy_type)
				{
					if (server_info->core1 >= vm.core / 2 && server_info->memory1 >= vm.memory / 2 && \
						server_info->core2 >= vm.core / 2 && server_info->memory2 >= vm.memory / 2)
					{
						if (isSleeping(server_info))
							daily_cost += server_info->daily_cost;
						server_info->core1 -= vm.core / 2;
						server_info->memory1 -= vm.memory / 2;
						server_info->core2 -= vm.core / 2;
						server_info->memory2 -= vm.memory / 2;
						AddRequest[id] = new Request(vm.core / 2, vm.core / 2, vm.memory / 2, vm.memory / 2, server_info);
						return true;
					}
				}
				else
				{
					if (server_info->core1 >= vm.core && server_info->memory1 >= vm.memory)
					{
						if (isSleeping(server_info))
							daily_cost += server_info->daily_cost;
						server_info->core1 -= vm.core;
						server_info->memory1 -= vm.memory;
						AddRequest[id] = new Request(vm.core, 0, vm.memory, 0, server_info);
						return true;
					}
					if (server_info->core2 >= vm.core && server_info->memory2 >= vm.memory)
					{
						if (isSleeping(server_info))
							daily_cost += server_info->daily_cost;
						server_info->core2 -= vm.core;
						server_info->memory2 -= vm.memory;
						AddRequest[id] = new Request(0, vm.core, 0, vm.memory, server_info);
						return true;
					}
				}
			}
			return false;
		}

		void BuyServerBasedOnVM(const VirtualMachine::VMInfo &vm, int id)
		{
			for (int i = 0; i < server.num_of_server; ++i)
			{
				Server::ServerInfo server_info = server.server[i];
				if (vm.deploy_type)
				{
					if (server_info.core1 >= vm.core / 2 && server_info.memory1 >= vm.memory / 2 && \
						server_info.core2 >= vm.core / 2 && server_info.memory2 >= vm.memory / 2)
					{
						server_info.core1 -= vm.core / 2;
						server_info.memory1 -= vm.memory / 2;
						server_info.core2 -= vm.core / 2;
						server_info.memory2 -= vm.memory / 2;
						AddRequest[id] = new Request(vm.core / 2, vm.core / 2, vm.memory / 2, vm.memory / 2, bought_server.BuyServer(server_info));
						if (purchase_machine.find(server_info.server_name) == purchase_machine.end())
						{
							++purchase_num;
							purchase_machine[server_info.server_name] = 1;
						}
						else ++purchase_machine[server_info.server_name];
						total_cost += server_info.buy_cost;
						daily_cost += server_info.daily_cost;
						return;
					}
				}
				else
				{
					if (server_info.core1 >= vm.core && server_info.memory1 >= vm.memory)
					{
						server_info.core1 -= vm.core;
						server_info.memory1 -= vm.memory;
						AddRequest[id] = new Request(vm.core, 0, vm.memory, 0, bought_server.BuyServer(server_info));
						if (purchase_machine.find(server_info.server_name) == purchase_machine.end())
						{
							++purchase_num;
							purchase_machine[server_info.server_name] = 1;
						}
						else ++purchase_machine[server_info.server_name];
						total_cost += server_info.buy_cost;
						daily_cost += server_info.daily_cost;
						return;
					}
					if (server_info.core2 >= vm.core && server_info.memory2 >= vm.memory)
					{
						server_info.core2 -= vm.core;
						server_info.memory2 -= vm.memory;
						AddRequest[id] = new Request(0, vm.core, 0, vm.memory, bought_server.BuyServer(server_info));
						if (purchase_machine.find(server_info.server_name) == purchase_machine.end())
						{
							++purchase_num;
							purchase_machine[server_info.server_name] = 1;
						}
						else ++purchase_machine[server_info.server_name];
						total_cost += server_info.buy_cost;
						daily_cost += server_info.daily_cost;
						return;
					}
				}
			}
		}

		void Release(int id)
		{
			Server::ServerInfo * server_info = AddRequest[id]->server_info;
			server_info->core1 += AddRequest[id]->delta_core1;
			server_info->core2 += AddRequest[id]->delta_core2;
			server_info->memory1 += AddRequest[id]->delta_memory1;
			server_info->memory2 += AddRequest[id]->delta_core2;
			if (isSleeping(server_info))
				daily_cost -= server_info->daily_cost;
		}

		void Process()
		{
			for (const VMRequest::Req &cur_req : request.req.at(today))
			{
				int cur_id = cur_req.vm_id;
				if (cur_req.req_type == 0)
				{
					Release(cur_id);
					continue;
				}
				const VirtualMachine::VMInfo &cur_vm = virtual_machine.GetVMInfoByName(cur_req.vm_name);
				if (!TryAssignSleepingServer2VM(cur_vm, cur_id))
					BuyServerBasedOnVM(cur_vm, cur_id);
			}
		}

		void Simulate()
		{
			for (today = 0; today < request.num_of_day; today++)
			{
				purchase_machine.clear();
				purchase_num = 0;
				Process();
				total_cost += daily_cost;
				/*
				std::cout << "(purchase, " << purchase_num << ")" << std::endl;
				for (map<std::string, int>::iterator it = purchase_machine.begin(); it != purchase_machine.end(); ++it)
					std::cout << "(" << it->first << ", " << it->second << ")" << std::endl;
				std::cout << "(migration, 0)" << std::endl;
				*/
			}
			std::cout << total_cost << std::endl;
		}

		FirstFitAlgorithm(const Server &server, const VirtualMachine &virtual_machine, const VMRequest &request)
			: server(server), virtual_machine(virtual_machine), request(request) {}
};

int main()
{
	freopen("training-1.txt", "r", stdin);
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
	// server.PrintInfo();
	// virtual_machine.PrintInfo();
	// request.PrintInfo();
#endif
	FirstFitAlgorithm firstfit(server, virtual_machine, request);
	firstfit.Simulate();
	return 0;
}
