#define DEBUG

#include <iostream>
#include <cstdio>
#include <algorithm>

namespace common
{
	inline int read_int()
	{
		char c = std::getchar();
		int y = 1;
		while (!isdigit(c))
		{
			if (c == '-')
				y = -1;
			c = std::getchar();
		}
		int x = 0;
		while (isdigit(c))
		{
			x = x * 10 + c - '0';
			c = std::getchar();
		}
		return x * y;
	}
};

class Server
{
public:
	struct ServerInfo
	{
		std::string server_name;
		int core;
		int memory;
		int buy_cost;
		int daily_cost;
	};

	int num_of_server;
	ServerInfo server[100];

	void ServerInit()
	{
		scanf("%d", &num_of_server);
		for (int i = 0; i < num_of_server; ++i)
		{
			char c = std::getchar();
			while (c != '(')
				c = std::getchar();
			c = std::getchar();
			server[i].server_name = "";
			while (c != ',')
			{
				server[i].server_name += c;
				c = std::getchar();
			}
			server[i].core = common::read_int();
			server[i].memory = common::read_int();
			server[i].buy_cost = common::read_int();
			server[i].daily_cost = common::read_int();
		}
	}

	void ShowServerInfo()
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
		std::string VM_name;
		int core;
		int memory;
		bool deploy_type;
	};

	int num_of_vm;
	VMInfo VM[1000];

	void VMInit()
	{
		scanf("%d", &num_of_vm);
		for (int i = 0; i < num_of_vm; ++i)
		{
			char c = std::getchar();
			while (c != '(')
				c = std::getchar();
			c = std::getchar();
			VM[i].VM_name = "";
			while (c != ',')
			{
				VM[i].VM_name += c;
				c = std::getchar();
			}
			VM[i].core = common::read_int();
			VM[i].memory = common::read_int();
			VM[i].deploy_type = common::read_int();
		}
	}

	void ShowVMInfo()
	{
		for (int i = 0; i < num_of_vm; ++i)
		{
			std::cout << VM[i].VM_name << ": ";
			printf("%dC %dG %d\n", VM[i].core, VM[i].memory, VM[i].deploy_type);
		}
	}

	VirtualMachine() {}
};

int main()
{
	Server server;
	server.ServerInit();
#ifdef DEBUG
	server.ShowServerInfo();
#endif
	VirtualMachine VM;
	VM.VMInit();
#ifdef DEBUG
	VM.ShowVMInfo();
#endif
	return 0;
}
