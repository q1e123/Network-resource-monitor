#ifdef _WIN32 || _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "netapi32.lib")

#include <iphlpapi.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#include <windows.h>
#include <tchar.h>
#include <Lmcons.h>
#include <lm.h>

#include <thread>
#include <chrono>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include <psapi.h>
#include "msw.h"
#include "utils.h"
#include "jiffy.h"

#define MB 1024/1024
#define GB 1024/1024/1024

using std::cerr;

std::string Msw::get_os(){
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

std::map<std::string, double> Msw::get_cpu_usage(){
    std::map<std::string, double> cpu_usage;
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

std::vector<std::string> Msw::get_network_interfaces(){
	std::vector<std::string> interfaces;
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
            std::string intr(reinterpret_cast<char*>(if_row->bDescr));
            interfaces.push_back(intr);
        }		
	}else{
		cerr << "Error: GetIfTable";
		exit(EXIT_FAILURE);
	}
	
	return interfaces;
}

std::map<std::string, Network_Usage> Msw::get_network_usage(){
	std::map<std::string, Network_Usage> network_usage;
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
    std::vector<Network_Usage> old_usage;
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
            std::string intr(reinterpret_cast<char*>(if_row->bDescr));
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
        auto tmp(proc_name);
        std::string name(tmp);
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
std::vector<Process> Msw::get_process_list() {
    std::vector<Process> proc_list;
    std::vector<std::thread> threads;

    DWORD aProcesses[1024], cbNeeded, cProcesses;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)){
        return proc_list;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    std::vector<Process> tmp;
    tmp = std::vector<Process>(cProcesses, Process());
    
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

std::vector<System_User> Msw::get_user_list() {
    std::vector<System_User> user_list;

    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_0 pTmpBuf;
    DWORD dwLevel = 0;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    DWORD i;
    NET_API_STATUS nStatus;
    LPTSTR pszServerName = NULL;
    do{
        nStatus = NetUserEnum((LPCWSTR)pszServerName,
            dwLevel,
            FILTER_NORMAL_ACCOUNT,
            (LPBYTE*)&pBuf,
            dwPrefMaxLen,
            &dwEntriesRead,
            &dwTotalEntries,
            &dwResumeHandle);
        if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
            if ((pTmpBuf = pBuf) != NULL){
                for (i = 0; (i < dwEntriesRead); i++){
                    std::wstring wuser(pTmpBuf->usri0_name);
                    std::string user(wuser.begin(), wuser.end());
                    System_User sys_user;
                    sys_user.username = user;
                    sys_user.last_login = get_last_login(wuser);
                    user_list.push_back(sys_user);
                    pTmpBuf++;
                }
            }
        }

        if (pBuf != NULL)
        {
            NetApiBufferFree(pBuf);
            pBuf = NULL;
        }
    } while (nStatus == ERROR_MORE_DATA);
    if (pBuf != NULL)
        NetApiBufferFree(pBuf);
    return user_list;
}

std::time_t Msw::get_last_login(std::wstring user) {
    DWORD dwLevel = 2;

    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_4 pBuf4 = NULL;

    NET_API_STATUS nStatus;

    LPTSTR sStringSid = NULL;

    nStatus = NetUserGetInfo(L"localhost", user.c_str(), dwLevel, (LPBYTE*)&pBuf);
    
    if (nStatus == NERR_Success)
    {
        if (pBuf != NULL)
        {
            pBuf4 = (LPUSER_INFO_4)pBuf;
            return pBuf4->usri4_last_logon;
        }
    }
}

std::map<std::string, std::string> Msw::get_environment_variables() {
    std::map <std::string, std::string> environment_variables;
    LPTSTR lpszVariable;
    LPTCH lpvEnv;
    lpvEnv = GetEnvironmentStrings();
    lpszVariable = (LPTSTR)lpvEnv;

    while (*lpszVariable){
        std::string line(lpszVariable);
        std::istringstream line_iss(line);
        std::string key, value;
        getline(line_iss, key, '=');
        getline(line_iss, value);
        environment_variables[key] = value;
        lpszVariable += lstrlen(lpszVariable) + 1;
    }
    FreeEnvironmentStrings(lpvEnv);

    return environment_variables;
}

double Msw::get_avalabile_space() {
    std::vector<std::string> drive_list = get_drive_list();
    double avalabile_space = 0;
    for (auto drive : drive_list) {
        std::string cmd = "dir " + drive + " |find \"bytes free\"";
        std::string dir_result = utils::execute(cmd.c_str());
        std::string dir_str = "Dir(s)  ";
        std::string bytes_str = "bytes free";

        std::string avalabile_space_str = dir_result.substr(dir_result.find(dir_str) + dir_str.size());

        avalabile_space_str = avalabile_space_str.substr(0, avalabile_space_str.size() - bytes_str.size() - 2);
        avalabile_space_str = utils::remove_char_str(avalabile_space_str, '.');

        avalabile_space += std::stoll(avalabile_space_str) * 1.0 / 1024 / 1024 / 1024;
    }
    return avalabile_space;
}

std::vector<std::string> Msw::get_drive_list() {
    std::string fsutil_result = utils::execute("fsutil fsinfo drives");
    std::string dummy;
    std::istringstream iss(fsutil_result);
    getline(iss, dummy);
    getline(iss, dummy, ' ');
    std::string drive;
    std::vector<std::string> drive_list;
    while (getline(iss, drive, ' ')) {
        if (drive.find(":") == std::string::npos) {
            continue;
        }
        drive_list.push_back(drive);
    }
    return drive_list;
}

std::vector<std::string> Msw::get_installed_programs() {
    std::set<std::string> installed_programs_set;
    std::ofstream f("test.txt");
    HKEY hUninstKey = NULL;
    HKEY hAppKey = NULL;
    CHAR sAppKeyName[1024];
    CHAR sSubKey[1024];
    CHAR sDisplayName[1024];
    CHAR path[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    CHAR* sRoot = path;
    long lResult = ERROR_SUCCESS;
    DWORD dwType = KEY_ALL_ACCESS;
    DWORD dwBufferSize = 0;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, sRoot, 0, KEY_READ, &hUninstKey);

    for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++){
        dwBufferSize = sizeof(sAppKeyName);
        if ((lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName,
            &dwBufferSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS){
            sprintf(sSubKey, "%s\\%s", sRoot, sAppKeyName);
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_READ, &hAppKey);

            dwBufferSize = sizeof(sDisplayName);
            RegQueryValueEx(hAppKey, "DisplayName", NULL,
                &dwType, (unsigned char*)sDisplayName, &dwBufferSize);
                std::string str(sDisplayName);
                installed_programs_set.insert(str);

            RegCloseKey(hAppKey);
        }
    }

    lResult = ERROR_SUCCESS;
    CHAR path2[] = "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    sRoot = path2;

    for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++)
    {

        dwBufferSize = sizeof(sAppKeyName);
        if ((lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName,
            &dwBufferSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
        {

            wsprintf(sSubKey, "%s\\%s", sRoot, sAppKeyName);
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_READ, &hAppKey);

            dwBufferSize = sizeof(sDisplayName);
            RegQueryValueEx(hAppKey, "DisplayName", NULL, &dwType, (unsigned char*)sDisplayName, &dwBufferSize);
                std::string str(sDisplayName);
            RegCloseKey(hAppKey);
        }
    }

    RegCloseKey(hUninstKey);

    std::vector<std::string> installed_programs = std::vector(installed_programs_set.begin(), installed_programs_set.end());
    return installed_programs;
}

std::map<std::string, std::string> Msw::get_ipv4_map() {
    std::map<std::string, std::string> ipv4_map;
    
    return ipv4_map;
}

void Msw::create_error_log() {
    return;
}

#endif