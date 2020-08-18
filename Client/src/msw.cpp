#ifdef _WIN32 || _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <iphlpapi.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#include <windows.h>
#include <Lmcons.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <psapi.h>
#include "msw.h"
#include "utils.h"
#include "jiffy.h"

#define MB 1024/1024
#define GB 1024/1024/1024

using std::cerr;

string Msw::get_os(){
	return utils::remove_char_str(utils::execute("ver"),'\n');
}

size_t Msw::get_total_ram(){
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx(&statex);
	return statex.ullTotalPhys / GB;
}

size_t Msw::get_avalabile_ram(){
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx(&statex);
	return statex.ullAvailPhys / GB;
}

size_t filetime_to_int(const FILETIME &ft){
    return (((size_t)(ft.dwHighDateTime)) << 32) | ((size_t)ft.dwLowDateTime);
}

map<string, double> Msw::get_cpu_usage(){
    map<string, double> cpu_usage;
    double usage;
    FILETIME idleTime, kernelTime, userTime;
    if(GetSystemTimes(&idleTime, &kernelTime, &userTime)){
        static size_t old_total=0;
        static size_t old_idle=0;

        size_t new_total, new_idle, dt, di;

        new_total = filetime_to_int(kernelTime)+filetime_to_int(userTime);
        new_idle = filetime_to_int(idleTime);

        dt = new_total-old_total;
        di  = new_idle-old_idle;

        old_total = new_total;
        old_idle  = new_idle;

        usage = 1.0;
        if(dt > 0){
            usage -= ((double)di)/dt;
        }
    }
    else{
        usage = -1.0;
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    cpu_usage["cpu"] = usage*100;
    return cpu_usage;
}

void Msw::shutdown(){
	utils::execute("shutdown -s -t 0");
}

void Msw::restart(){
	utils::execute("shutdown -r -t 0");
}


void setup_iftable(MIB_IFROW *if_table, DWORD&dw_size){

}

vector<string> Msw::get_network_interfaces(){
	vector<string> interfaces;
	DWORD dw_size;

    MIB_IFTABLE* if_table;
    MIB_IFROW* if_row;

    if_table = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
    if (if_table == NULL) {
        cerr << "Error: memory allocation for if_table";
        exit(EXIT_FAILURE);
    }

    dw_size = sizeof(MIB_IFTABLE);
    DWORD ret = GetIfTable(if_table, &dw_size, FALSE);
    if (ret == ERROR_INSUFFICIENT_BUFFER) {
        FREE(if_table);
        if_table = (MIB_IFTABLE*)MALLOC(dw_size);
        if (if_table == NULL) {
            cerr << "Error: memory allocation for getIfTable";
            exit(EXIT_FAILURE);
        }
    }

    if(GetIfTable(if_table, &dw_size, FALSE) == NO_ERROR) {
        for (size_t i = 0; i < if_table->dwNumEntries; i++) {
            if_row = (MIB_IFROW*)&if_table->table[i];
            string intr(reinterpret_cast<char*>(if_row->bDescr));
            interfaces.push_back(intr);
        }		
	}else{
		cerr << "Error: GetIfTable";
		exit(EXIT_FAILURE);
	}
	
	return interfaces;
}

map<string, Network_Usage> Msw::get_network_usage(){
	map<string, Network_Usage> network_usage;
    DWORD dw_size;

    MIB_IFTABLE* if_table;
    MIB_IFROW* if_row;

    if_table = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
    if (if_table == NULL) {
        cerr << "Error: memory allocation for if_table";
        exit(EXIT_FAILURE);
    }

    dw_size = sizeof(MIB_IFTABLE);
    DWORD ret = GetIfTable(if_table, &dw_size, FALSE);
    if (ret == ERROR_INSUFFICIENT_BUFFER) {
        FREE(if_table);
        if_table = (MIB_IFTABLE*)MALLOC(dw_size);
        if (if_table == NULL) {
            cerr << "Error: memory allocation for getIfTable";
            exit(EXIT_FAILURE);
        }
    }
    vector<Network_Usage> old_usage;
    if(GetIfTable(if_table, &dw_size, FALSE) == NO_ERROR) {
        for (size_t i = 0; i < if_table->dwNumEntries; i++) {
            if_row = (MIB_IFROW*)&if_table->table[i];
            Network_Usage usage(if_row->dwInOctets, if_row->dwOutOctets);
            old_usage.push_back(usage);
        }
	}else{
		cerr << "Error: GetIfTable";
		exit(EXIT_FAILURE);
	}

    std::this_thread::sleep_for (std::chrono::milliseconds(1000));

    if_table = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
    if (if_table == NULL) {
        cerr << "Error: memory allocation for if_table";
        exit(EXIT_FAILURE);
    }

    dw_size = sizeof(MIB_IFTABLE);
    ret = GetIfTable(if_table, &dw_size, FALSE);
    if (ret == ERROR_INSUFFICIENT_BUFFER) {
        FREE(if_table);
        if_table = (MIB_IFTABLE*)MALLOC(dw_size);
        if (if_table == NULL) {
            cerr << "Error: memory allocation for getIfTable";
            exit(EXIT_FAILURE);
        }
    }

    if(GetIfTable(if_table, &dw_size, FALSE) == NO_ERROR) {
        for (size_t i = 0; i < if_table->dwNumEntries; i++){
            if_row = (MIB_IFROW*)&if_table->table[i];
            string intr(reinterpret_cast<char*>(if_row->bDescr));
			Network_Usage new_usage(if_row->dwInOctets, if_row->dwOutOctets);
            std::cout << new_usage.get_rx()<<" " <<old_usage[i].get_rx()<<"\n";
            Network_Usage usage(new_usage.get_rx() - old_usage[i].get_rx(), new_usage.get_tx() - old_usage[i].get_tx());
			network_usage[intr] = usage;
        }
	}else{
		cerr << "Error: GetIfTable";
		exit(EXIT_FAILURE);
	}
	
	return network_usage;
}
double filetime_to_double(struct _FILETIME& ft) {
    unsigned long long t = ft.dwLowDateTime + ((unsigned long long)ft.dwHighDateTime << 32);
    return t / 1e7;
}

void Msw::get_proc_info(DWORD pid, Process& process) {
    HANDLE process_handle;
    PROCESS_MEMORY_COUNTERS proc_mem_counter;
    struct _FILETIME creation_time, exit_time, kernel_time, user_time;
    process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (process_handle == NULL) {
        cerr << "Can't open process: " << pid << "\n";
        return;
    }
    HMODULE handle_module;
    DWORD cbNeeded;
    TCHAR proc_name[MAX_PATH] = TEXT("<unknown>");
    if (GetProcessMemoryInfo(process_handle, &proc_mem_counter, sizeof(proc_mem_counter)) && GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time))
    {
        EnumProcessModules(process_handle, &handle_module, sizeof(handle_module), &cbNeeded);
        double cpu_usage_old;
        cpu_usage_old = filetime_to_double(user_time) + filetime_to_double(kernel_time);

        GetModuleFileNameEx(process_handle, handle_module, proc_name, sizeof(proc_name) / sizeof(TCHAR));
        //GetModuleBaseName(process_handle, handle_module, proc_name, sizeof(proc_name) / sizeof(TCHAR));
        std::wstring tmp(proc_name);
        std::string name(tmp.begin(), tmp.end());
        name = name.substr(name.rfind("\\") + 1);
        size_t p, ram;
        p = pid; 
        ram = proc_mem_counter.WorkingSetSize;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time);
        double cpu_usage_new, cpu_usage;
        cpu_usage_new = filetime_to_double(user_time) + filetime_to_double(kernel_time);
        cpu_usage = (cpu_usage_new - cpu_usage_old) * 100;
        process = Process(pid, name, ram, cpu_usage);
    }
    CloseHandle(process_handle);

}
vector<Process> Msw::get_process_list() {
    vector<Process> proc_list;
    vector<std::thread> threads;

    DWORD aProcesses[1024], cbNeeded, cProcesses;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)){
        return proc_list;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    vector<Process> tmp;
    tmp = vector<Process>(cProcesses, Process());
    
    for (size_t i = 0; i < cProcesses; ++i) {
        std::thread worker(Msw::get_proc_info, aProcesses[i], std::ref(tmp[i]));
        threads.push_back(std::move(worker));
    }

    for (auto& worker : threads) {
        worker.join();
    }

    for (auto proc : tmp) {
        if (proc.get_name() != "") {
            proc_list.push_back(proc);
        }
    }

    return proc_list;
}

std::string Msw::get_machine_id(){

	std::string serial_number = utils::execute("wmic DISKDRIVE get SerialNumber");   
    serial_number = serial_number.substr(23, 19);
	std::string motherboard_id = utils::execute("wmic csproduct get UUID");
    motherboard_id = motherboard_id.substr(40, 74);
    std::string machine_id = serial_number + "-" +motherboard_id;
    return machine_id;
}

std::string Msw::get_current_user(){
    char username_c[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username_c, &username_len);

    std::string user = std::string(username_c);
    return user;
}
#endif

