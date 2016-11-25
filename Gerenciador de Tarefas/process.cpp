#include "process.h"

Process::Process()
{
    pid = 0;
    ppid = 0;
    cpu = 0.0;
    memory = 0.0;
    threads = 0;
}

void Process::filemanipulation()
{
    system("ps xao pid,ppid,pcpu,pmem,nlwp,comm > ../GerenciadorTarefas/processinfo");

    std::ifstream file;
    file.open("../GerenciadorTarefas/processinfo");
    std::string line;

    info.clear();
    if (!file)
        exit(EXIT_FAILURE);
    else {
        getline(file, line);
        while(getline(file,line))
        {
            Process p;
            std::istringstream iss(line);

            iss >> p.pid;
            iss >> p.ppid;
            iss >> p.cpu;
            iss >> p.memory;
            iss >> p.threads;
            iss >> p.name;

            info[p.ppid].push_back(p);
        }
    }

    file.close();
}

void Process::createjsonfile(int filter)
{
    filemanipulation();

    int update;
    int i = 0;
    int j = 0;
    std::ofstream file;
    file.open("../GerenciadorTarefas/processos.json");

    file << "{\"name\": \"processes\", \"children\": [";
    for (std::pair<int, std::vector<Process>> map_pair : info)
    {
        std::vector<Process> p = map_pair.second;
        if(i++ == 0)
            file << "";
        else
            file << ",";

        file << "{\"name\":\"" << p[0].name << "\",\"children\":[";

        for (Process proc : map_pair.second)
        {
            if (filter == 0)
                update = (proc.cpu + 0.1)*1000;
            else if (filter == 1)
                update = proc.threads;
            else if(filter == 2)
                update = (proc.memory + 0.1) * 1000;

            if(j++ == 0)
                file << "";
            else
                file << ",";

            file << "{\"name\":\"" << proc.name << "\",\"size\":" << update << ",\"pid\":" << proc.pid << "}";
        }
        j = 0;
        file << "]}";
    }
    file << "]}";
    file.close();

}

void SignalHandler(int sig)
{
  exit(1);
}

void Process::killProc(int pid)
{
    pid_t value = pid;
    signal(SIGKILL, SignalHandler);
    kill(value, SIGKILL);
}
