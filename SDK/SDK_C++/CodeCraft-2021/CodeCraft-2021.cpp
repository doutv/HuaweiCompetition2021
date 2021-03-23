#define DEBUG

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <unordered_map>

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
	unordered_map<string, ServerInfo> server_map;
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
			server_map.emplace(server[i].server_name, server[i]);
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
		bool deploy_type;
	};

	int32_t num_of_vm;
	static const int32_t vm_max_size = 1000;
	VMInfo vm[vm_max_size];
	unordered_map<string, VMInfo> vmtype_map;
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
			vmtype_map.emplace(vm[i].vm_name, vm[i]);
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
		bool add_or_delete;
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
				string add_or_delete_str = common::get_string();
				if (add_or_delete_str == "add")
				{
					cur_req.add_or_delete = 1;
					cur_req.vm_name = common::get_string();
					cur_req.vm_id = common::read_int();
				}
				else
				{
					cur_req.add_or_delete = 0;
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
				cout << cur_req.add_or_delete << " " << cur_req.vm_name << " " << cur_req.vm_id << endl;
			}
		}
	}

	VMRequest(){};
};

class RunningVM
{
public:
	struct VMInfo
	{
		std::string vm_name;
		int32_t core;
		int32_t memory;
		bool deploy_type;
	};
	unordered_map<int32_t, VMInfo> vmid_map;

	void init()
	{
	}
	int64_t evaluate_cost()
	{
	}
	RunningVM(){};
};
int main()
{
#ifdef DEBUG
	// const char *test_file_path = "../training1.txt";
	// freopen(test_file_path, "r", stdin);
#endif
	Server server;
	server.Init();
	VirtualMachine VM;
	VM.Init();
	VMRequest Request;
	Request.Init();
#ifdef DEBUG
	server.PrintInfo();
	VM.PrintInfo();
#endif
	return 0;
}
