// #define DEBUG_LOCAL

#include <cmath>
#include <chrono>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <random>
using namespace std;

auto time_start = chrono::steady_clock::now();
namespace common
{
	std::random_device rd;
	std::mt19937 gen(rd()); // seed the generator
	inline int32_t rand_between(int32_t left, int32_t right)
	{
		std::uniform_int_distribution<> dis(left, right);
		return dis(gen);
	}
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
		int32_t core_half;
		int32_t memory_half;

		bool operator<(const ServerInfo &other) const
		{
			return buy_cost < other.buy_cost;
		}
		bool operator>(const ServerInfo &other) const
		{
			return !(*this < other);
		}
	};

	int32_t num_of_server;
	static const int32_t server_max_size = 100;
	ServerInfo server[server_max_size];
	unordered_map<string, reference_wrapper<ServerInfo>> servername2info_map;

	class EvaluateServerValueComp
	{
	public:
		double EvaluateServerValue(ServerInfo const &x) const
		{
			// evaluate server value
			int32_t estimated_num_of_day = 500;
			return (double)((x.core + x.memory) << 19) / (double)(x.buy_cost + x.daily_cost * estimated_num_of_day);
		}
		bool operator()(ServerInfo const &x, ServerInfo const &y) const
		{
			return EvaluateServerValue(x) < EvaluateServerValue(y);
		}
	};
	priority_queue<ServerInfo, vector<ServerInfo>, EvaluateServerValueComp> evaluate_q;
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
			server[i].core_half = server[i].core >> 1;
			server[i].memory_half = server[i].memory >> 1;
			servername2info_map.emplace(server[i].server_name, server[i]);
		}
	}
	ServerInfo &GetServerInfoByName(string server_name) const
	{
		return servername2info_map.at(server_name);
	}
	void SortServer()
	{
		for (int32_t i = 0; i < num_of_server; i++)
		{
			evaluate_q.push(server[i]);
		}
	}

	Server() {}
};

class VirtualMachine
{
public:
	enum DeployType : bool
	{
		single_port,
		double_port
	};
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
	VirtualMachine::VMInfo &GetVMInfoById(int32_t vm_id) const
	{
		return vmid2info_map.at(vm_id);
	}
	void AddVM(int32_t vm_id, VirtualMachine::VMInfo &vm_info)
	{
		vmid2info_map.emplace(vm_id, vm_info);
	}
	void DelVM(int32_t vm_id)
	{
		vmid2info_map.erase(vm_id);
	}
	pair<const int32_t, reference_wrapper<VirtualMachine::VMInfo>> &GetRandomVmid2infoIter()
	{
		return *std::next(std::begin(vmid2info_map), common::rand_between(0, vmid2info_map.size() - 1));
	}
	RunningVM(const VirtualMachine &vm) : vm(vm) {}
};

class BoughtServer
{
	// 已经购买的 server
	// 可能处于关机状态
public:
	const Server &server;
	int32_t nxt_server_id;

	enum ServerStatus : int8_t
	{
		XA_XB,
		A_XB,
		XA_B,
		A_B
	};
	typedef pair<Server::ServerInfo *, ServerStatus> bought_server_t;
	unordered_map<int32_t, bought_server_t> serverid2info_map;

	Server::ServerInfo &GetServerInfoById(int32_t server_id) const
	{
		return *serverid2info_map.at(server_id).first;
	}
	ServerStatus &GetServerStatusById(int32_t server_id)
	{
		return serverid2info_map.at(server_id).second;
	}
	pair<int64_t, int32_t> BuyServer(Server::ServerInfo &server_info, int32_t num)
	{
		int64_t total_buy_cost = 0;
		int32_t st_server_id = nxt_server_id;
		nxt_server_id += num;
		for (int32_t i = 0; i < num; i++)
		{
			int32_t server_id = st_server_id + i;
			serverid2info_map.emplace(server_id, make_pair(&server_info, ServerStatus::XA_XB));
			total_buy_cost += server_info.buy_cost;
		}
		return make_pair(total_buy_cost, st_server_id);
	}
	int64_t CalculateDailyCost()
	{
		int64_t cost = 0;
		for (auto &it : serverid2info_map)
		{
			auto cur_server = it.second;
			auto server_info = cur_server.first;
			auto server_status = cur_server.second;
			if (server_status != ServerStatus::XA_XB)
				cost += server_info->daily_cost;
		}
		return cost;
	}
	BoughtServer(const Server &server) : server(server)
	{
		nxt_server_id = 0;
	}
};
class Greedy
{
public:
	Server &server;
	const VirtualMachine &virtual_machine;
	const VMRequest &request;
	RunningVM running_vm;
	BoughtServer bought_server;

	enum DeployPort : bool
	{
		portA = false,
		portB = true
	};
	typedef pair<int32_t, bool> vmid2serverid_t;
	unordered_map<int32_t, vmid2serverid_t> vmid2serverid_map;
	int64_t total_cost;
	int32_t today_id;

	unordered_map<string, int32_t> today_buy_servername2num_map;
	unordered_multimap<Server::ServerInfo *, int32_t> today_serverinfo2vmid_multimap;
	typedef pair<BoughtServer::bought_server_t, int32_t> serverinfo_id_t;
	class AssignSleepingServerComp
	{
	public:
		bool operator()(serverinfo_id_t const &x, serverinfo_id_t const &y) const
		{
			auto &x_server_info = *x.first.first;
			auto &y_server_info = *y.first.first;
			return (x_server_info.core > y_server_info.core) ||
				   (x_server_info.core == y_server_info.core && x_server_info.memory > y_server_info.memory);
		}
	};
	bool IsServerFitVm(Server::ServerInfo const &server_info, VirtualMachine::VMInfo const &vm_info) const
	{
		if (vm_info.deploy_type == virtual_machine.single_port)
		{
			if (server_info.core_half >= vm_info.core && server_info.memory_half >= vm_info.memory)
				return true;
		}
		else
		{
			if (server_info.core >= vm_info.core && server_info.memory >= vm_info.memory)
				return true;
		}
		return false;
	}
	bool IsServerFitVm(Server::ServerInfo const &server_info, BoughtServer::ServerStatus const &server_status, VirtualMachine::VMInfo const &vm_info) const
	{
		if (vm_info.deploy_type == virtual_machine.single_port)
		{
			if (server_status == BoughtServer::A_B)
				return false;
			if (server_info.core_half >= vm_info.core && server_info.memory_half >= vm_info.memory)
				return true;
		}
		else
		{
			if (server_status != BoughtServer::XA_XB)
				return false;
			if (server_info.core >= vm_info.core && server_info.memory >= vm_info.memory)
				return true;
		}
		return false;
	}
	void BuyServerOnFirstDay()
	{
		// find the day that need most servers
		int target_day = 0;
		int max_req_server = 0;
		int cur_req_server = 0;
		unordered_map<int32_t, bool> tmp_vmid2deploytype_map;
		for (int day = 0; day < request.num_of_day; day++)
		{
			for (auto const &req : request.req_list.at(day))
			{
				if (req.req_type == 0)
				{
					bool deploy_type = tmp_vmid2deploytype_map.at(req.vm_id);
					if (deploy_type == virtual_machine.single_port)
						--cur_req_server;
					else
						cur_req_server -= 2;
					tmp_vmid2deploytype_map.erase(req.vm_id);
				}
				else if (req.req_type == 1)
				{
					auto &vm_info = virtual_machine.GetVMInfoByName(req.vm_name);
					tmp_vmid2deploytype_map.emplace(req.vm_id, vm_info.deploy_type);
					if (vm_info.deploy_type == virtual_machine.single_port)
						++cur_req_server;
					else
						cur_req_server += 2;
				}
			}
			if (cur_req_server > max_req_server)
			{
				max_req_server = cur_req_server;
				target_day = day;
			}
		}

		// buy server
		max_req_server >>= 1;
		const size_t server_type_num = min(server.num_of_server, 5);
		printf("(purchase, %zu)\n", server_type_num);

		unordered_set<size_t> tmp_already_bought_server_set;
		for (size_t i = 0; i < server_type_num; i++)
		{
			size_t idx;
			do
			{
				idx = common::rand_between(0, server.num_of_server - 1);
			} while (tmp_already_bought_server_set.find(idx) != tmp_already_bought_server_set.end());
			auto &server_info = server.server[idx];
			tmp_already_bought_server_set.emplace(idx);
			int32_t order_num = common::rand_between(1, max_req_server >> 1);
			pair<int64_t, int32_t> p = bought_server.BuyServer(server_info, order_num);
			int64_t buy_cost = p.first;
			total_cost += buy_cost;
			printf("(%s, %d)\n", server_info.server_name.c_str(), order_num);
		}
		for (const VMRequest::Req &req : request.req_list.at(0))
		{
			// skip del type
			if (req.req_type == 0)
				continue;
			auto &vm_info = virtual_machine.GetVMInfoByName(req.vm_name);
			TryAssignEmptyServer2VM(req, vm_info);
		}
	}
	bool TryAssignEmptyServer2VM(VMRequest::Req req, VirtualMachine::VMInfo &vm_info)
	{
		priority_queue<serverinfo_id_t, vector<serverinfo_id_t>, AssignSleepingServerComp> q;
		if (vm_info.deploy_type == virtual_machine.single_port)
		{
			// single deploy
			// First try to find a single empty server
			// since it does not need to turn on a sleeping server
			for (auto &it : bought_server.serverid2info_map)
			{
				int32_t server_id = it.first;
				auto cur_server = it.second;
				auto server_info = cur_server.first;
				auto server_status = cur_server.second;
				if (server_status == bought_server.A_XB || server_status == bought_server.XA_B)
				{
					// single deploy only use half of the core and memory
					if (IsServerFitVm(*server_info, vm_info))
					{
						q.push(make_pair(cur_server, server_id));
					}
				}
			}
			if (!q.empty())
			{
				int32_t server_id = q.top().second;
				auto &server_status = bought_server.GetServerStatusById(server_id);
				DeployPort deploy_port;
				if (server_status == bought_server.A_XB)
					deploy_port = portB;
				else if (server_status == bought_server.XA_B)
					deploy_port = portA;
				server_status = bought_server.A_B;
				vmid2serverid_map.emplace(req.vm_id, make_pair(server_id, deploy_port));
				running_vm.AddVM(req.vm_id, vm_info);
				return true;
			}
			// no need to clear q and serverinfo_set
		}
		// double deploy or single deploy cannot find proper server
		for (auto &it : bought_server.serverid2info_map)
		{
			int32_t server_id = it.first;
			auto cur_server = it.second;
			auto server_info = cur_server.first;
			auto server_status = cur_server.second;
			if (server_status == bought_server.XA_XB)
			{
				// single deploy only use half of the core and memory
				if (IsServerFitVm(*server_info, vm_info))
				{
					q.push(make_pair(cur_server, server_id));
				}
			}
		}
		if (!q.empty())
		{
			int32_t server_id = q.top().second;
			auto &server_status = bought_server.GetServerStatusById(server_id);
			DeployPort deploy_port = portA;
			if (server_status == bought_server.XA_XB)
			{
				if (vm_info.deploy_type == virtual_machine.single_port)
				{
					deploy_port = portA;
					server_status = bought_server.A_XB;
				}
				else
					server_status = bought_server.A_B;
			}
			vmid2serverid_map.emplace(req.vm_id, make_pair(server_id, deploy_port));
			running_vm.AddVM(req.vm_id, vm_info);
			return true;
		}
		else
			return false;
	}
	void OrderServerBasedOnReq(VMRequest::Req req, VirtualMachine::VMInfo &vm_info)
	{
		priority_queue<Server::ServerInfo, vector<Server::ServerInfo>, greater<Server::ServerInfo>> q;
		for (auto server_info : server.server)
		{
			if (server_info.server_name.empty())
				break;
			if (IsServerFitVm(server_info, vm_info))
				q.push(server_info);
		}
		auto &server_info = server.GetServerInfoByName(q.top().server_name);
		++today_buy_servername2num_map[server_info.server_name];
		today_serverinfo2vmid_multimap.emplace(&server_info, req.vm_id); // bind vmid to server
		running_vm.AddVM(req.vm_id, vm_info);
	}
	void Buy()
	{
		for (const VMRequest::Req &req : request.req_list.at(today_id))
		{
			// skip del type
			if (req.req_type == 0)
				continue;
			auto &vm_info = virtual_machine.GetVMInfoByName(req.vm_name);
			if (!TryAssignEmptyServer2VM(req, vm_info))
				OrderServerBasedOnReq(req, vm_info);
		}
		// 输出购买信息
		printf("(purchase, %zu)\n", today_buy_servername2num_map.size());
		for (auto &it : today_buy_servername2num_map)
		{
			string server_name = it.first;
			int32_t num = it.second;
			auto &server_info = server.GetServerInfoByName(server_name);
			pair<int64_t, int32_t> p = bought_server.BuyServer(server_info, num);
			int64_t buy_cost = p.first;
			int32_t st_server_id = p.second;
			total_cost += buy_cost;
			auto range = today_serverinfo2vmid_multimap.equal_range(&server_info);
			for (auto each_it = range.first; each_it != range.second; ++each_it)
			{
				int32_t server_id = st_server_id++;
				int32_t vm_id = each_it->second;
				auto &vm_info = running_vm.GetVMInfoById(vm_id);
				auto &server_status = bought_server.GetServerStatusById(server_id);
				DeployPort deploy_port = portA;
				if (server_status == bought_server.XA_XB)
				{
					if (vm_info.deploy_type == virtual_machine.single_port)
					{
						deploy_port = portA;
						server_status = bought_server.A_XB;
					}
					else
						server_status = bought_server.A_B;
				}
				vmid2serverid_map.emplace(vm_id, make_pair(server_id, deploy_port));
			}
			printf("(%s, %d)\n", server_name.c_str(), num);
		}
		today_buy_servername2num_map.clear();
		today_serverinfo2vmid_multimap.clear();
	}
	void Migrate()
	{
		int32_t migration_num = floor((double)running_vm.vmid2info_map.size() * 0.001);
		printf("(migration, %d)\n", migration_num);
		while (migration_num > 0)
		{
			auto &vmid2info_it = running_vm.GetRandomVmid2infoIter();
			int32_t vm_id = vmid2info_it.first;
			auto &vm_info = vmid2info_it.second.get();
			for (auto &serverid2info_it : bought_server.serverid2info_map)
			{
				int32_t server_id = serverid2info_it.first;
				auto &cur_server = serverid2info_it.second;
				auto &server_info = *cur_server.first;
				auto &server_status = cur_server.second;
				if (vmid2serverid_map.at(vm_id).first == server_id)
					continue;
				if (IsServerFitVm(server_info, server_status, vm_info))
				{
					if (common::rand_between(0, 100) > 20)
					{
						if (vm_info.deploy_type == virtual_machine.single_port)
						{
							DeployPort deploy_port = portA;
							if (server_status == bought_server.XA_XB)
							{
								deploy_port = portA;
								server_status = bought_server.A_XB;
							}
							else if (server_status == bought_server.A_XB)
							{
								deploy_port = portB;
								server_status = bought_server.A_B;
							}
							else if (server_status == bought_server.XA_B)
							{
								deploy_port = portA;
								server_status = bought_server.A_B;
							}
							vmid2serverid_map.emplace(vm_id, make_pair(server_id, deploy_port));
							printf("(%d, %d, %c)\n", vm_id, server_id, deploy_port + 'A');
						}
						else
						{
							server_status = bought_server.A_B;
							vmid2serverid_map.emplace(vm_id, make_pair(server_id, 0));
							printf("(%d, %d)\n", vm_id, server_id);
						}
						--migration_num;
						break;
					}
				}
			}
		}
	}
	void ProcessRequest()
	{
		for (auto const &req : request.req_list.at(today_id))
		{
			if (req.req_type == 0)
			{
				// del
				auto &vm_info = running_vm.GetVMInfoById(req.vm_id);
				vmid2serverid_t &vm2server = vmid2serverid_map.at(req.vm_id);
				int32_t server_id = vm2server.first;
				bool deploy_port = vm2server.second;
				auto &server_status = bought_server.GetServerStatusById(server_id);
				if (vm_info.deploy_type == virtual_machine.single_port)
				{
					// single deploy
					if (deploy_port == portA)
					{
						// deploy on A port
						if (server_status == bought_server.A_B)
						{
							server_status = bought_server.XA_B;
						}
						else if (server_status == bought_server.A_XB)
						{
							server_status = bought_server.XA_XB;
						}
					}
					else if (deploy_port == portB)
					{
						// deploy on B port
						if (server_status == bought_server.A_B)
						{
							server_status = bought_server.A_XB;
						}
						else if (server_status == bought_server.XA_B)
						{
							server_status = bought_server.XA_XB;
						}
					}
				}
				else if (vm_info.deploy_type == virtual_machine.double_port)
				{
					// double deploy
					server_status = bought_server.XA_XB;
				}
				running_vm.DelVM(req.vm_id);
				vmid2serverid_map.erase(req.vm_id);
			}
			else
			{
				auto &vm_info = virtual_machine.GetVMInfoByName(req.vm_name);
				vmid2serverid_t &vm2server = vmid2serverid_map.at(req.vm_id);
				int32_t server_id = vm2server.first;
				bool &deploy_port = vm2server.second;
				// output based on deploy type
				if (vm_info.deploy_type == virtual_machine.single_port)
				{
					printf("(%d, %c)\n", server_id, deploy_port + 'A');
				}
				else if (vm_info.deploy_type == virtual_machine.double_port)
				{
					printf("(%d)\n", server_id);
				}
			}
		}
	}
	void Simulate()
	{
		// 对于每一天的操作，会先按顺序执行选手输出的购买操作，然后按顺序执行选手输出的迁移操作，最后按顺序执行当天的创建和删除操作。
		// 输入中可能存在当天创建后当天删除的虚拟机，对于这些虚拟机请求，你依然需要按顺序正常处理。
		for (today_id = 0; today_id < request.num_of_day; today_id++)
		{
			if (today_id == 0)
				BuyServerOnFirstDay();
			else
				Buy();
			Migrate();
			ProcessRequest();
			total_cost += bought_server.CalculateDailyCost();
		}
	}
	Greedy(Server &server, const VirtualMachine &virtual_machine, const VMRequest &request)
		: server(server), virtual_machine(virtual_machine), request(request), running_vm(virtual_machine), bought_server(server) {}
};

int main()
{
#ifdef DEBUG_LOCAL
	const char *test_file_path = "/home/jason/HuaWei_Contest/SDK/training-1.txt";
	freopen(test_file_path, "r", stdin);
	// const char *output_file_path = "/home/jason/HuaWei_Contest/SDK/training-0.out";
	// freopen(output_file_path, "w", stdout);
#endif
	// 读取所有输入数据
	Server server;
	VirtualMachine virtual_machine;
	VMRequest request;
	server.Init();
	virtual_machine.Init();
	request.Init();
	// 贪心
	Greedy greedy(server, virtual_machine, request);
	greedy.Simulate();
#ifdef DEBUG_LOCAL
	freopen("/dev/tty", "w", stdout);
	auto time_end = chrono::steady_clock::now();
	auto diff = time_end - time_start;
	cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
	fclose(stdin);
	fclose(stdout);
	return 0;
}
