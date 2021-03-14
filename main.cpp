#include <iostream>
#include <cstdio>
#include <algorithm>

class CommonFunctions
{
    public:
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

        CommonFunctions() {}
};

class Server
{
    private:
        CommonFunctions * function_set;

    public:
        struct ServerInfo
        {
            std::string server_name;
            int CPU;
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
                server[i].CPU = function_set->read_int();
                server[i].memory = function_set->read_int();
                server[i].buy_cost = function_set->read_int();
                server[i].daily_cost = function_set->read_int();
            }
        }
        
        void ShowServerInfo()
        {
        	for (int i = 0; i < num_of_server; ++i)
        	{
        		std::cout << server[i].server_name << ": ";
        		printf("%dC %dG $%d $%d\n", server[i].CPU, server[i].memory, server[i].buy_cost, server[i].daily_cost);
        	}
        }

        Server() {}
};

class VirtualMachine
{
    private:
        CommonFunctions * function_set;
    
    public:
        struct VMInfo
        {
            std::string VM_name;
            int CPU;
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
                VM[i].CPU = function_set->read_int();
                VM[i].memory = function_set->read_int();
                VM[i].deploy_type = function_set->read_int();
            }
        }
        
        void ShowVMInfo()
        {
        	for (int i = 0; i < num_of_vm; ++i)
        	{
        		std::cout << VM[i].VM_name << ": ";
        		printf("%dC %dG %d\n", VM[i].CPU, VM[i].memory, VM[i].deploy_type);
        	}
        }

        VirtualMachine() {}
};


int main()
{
    Server * server = new Server();
    server->ServerInit();
    // server->ShowServerInfo();
    VirtualMachine * VM = new VirtualMachine();
    VM->VMInit();
    // VM->ShowVMInfo();
    return 0;
}
