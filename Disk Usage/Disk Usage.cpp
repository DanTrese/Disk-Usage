#include <iostream>
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <iomanip>


void consoleColor(int background, int text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

void bar(double percent) {
    const int width = 100;
    int num = int(width * percent / 100.0);

    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < num) std::cout << "#";
        else std::cout << " ";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << percent << "%";
}

void diskUsage(char drive) {
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counterDiskTime, counterAvgReadTime, counterAvgWriteTime;
    PDH_HCOUNTER counterReadSpeed, counterWriteSpeed;
    PDH_FMT_COUNTERVALUE valueDiskTime, valueAvgReadTime, valueAvgWriteTime;
    PDH_FMT_COUNTERVALUE valueReadSpeed, valueWriteSpeed;

    status = PdhOpenQuery(NULL, 0, &query);

    std::string driveLetter(1, drive);
    std::string counterPathDiskTime = "\\LogicalDisk(" + driveLetter + ":)\\% Disk Time";
    std::string counterPathAvgReadTime = "\\LogicalDisk(" + driveLetter + ":)\\Avg. Disk sec/Read";
    std::string counterPathAvgWriteTime = "\\LogicalDisk(" + driveLetter + ":)\\Avg. Disk sec/Write";
    std::string counterPathReadSpeed = "\\LogicalDisk(" + driveLetter + ":)\\Disk Read Bytes/sec";
    std::string counterPathWriteSpeed = "\\LogicalDisk(" + driveLetter + ":)\\Disk Write Bytes/sec";

    status = PdhAddCounter(query, counterPathDiskTime.c_str(), 0, &counterDiskTime);
    status = PdhAddCounter(query, counterPathAvgReadTime.c_str(), 0, &counterAvgReadTime);
    status = PdhAddCounter(query, counterPathAvgWriteTime.c_str(), 0, &counterAvgWriteTime);
    status = PdhAddCounter(query, counterPathReadSpeed.c_str(), 0, &counterReadSpeed);
    status = PdhAddCounter(query, counterPathWriteSpeed.c_str(), 0, &counterWriteSpeed);

    bool pause = true;

    while (true) {
        status = PdhCollectQueryData(query);

        if (pause) {
            pause = false;
            Sleep(1000);
            continue;
        }

        system("cls");

        status = PdhGetFormattedCounterValue(counterDiskTime, PDH_FMT_DOUBLE, NULL, &valueDiskTime);
        status = PdhGetFormattedCounterValue(counterAvgReadTime, PDH_FMT_DOUBLE, NULL, &valueAvgReadTime);
        status = PdhGetFormattedCounterValue(counterAvgWriteTime, PDH_FMT_DOUBLE, NULL, &valueAvgWriteTime);
        status = PdhGetFormattedCounterValue(counterReadSpeed, PDH_FMT_DOUBLE, NULL, &valueReadSpeed);
        status = PdhGetFormattedCounterValue(counterWriteSpeed, PDH_FMT_DOUBLE, NULL, &valueWriteSpeed);

        if (status == ERROR_SUCCESS) {
            std::cout << R"(
 .---.  .---.     ____     .-------.      ______                       ______      .-------.     .-./`)  ,---.  ,---.     .-''-.   
 |   |  |_ _|   .'  __ `.  |  _ _   \    |    _ `''.                  |    _ `''.  |  _ _   \    \ .-.') |   /  |   |   .'_ _   \  
 |   |  ( ' )  /   '  \  \ | ( ' )  |    | _ | ) _  \                 | _ | ) _  \ | ( ' )  |    / `-' \ |  |   |  .'  / ( ` )   ' 
 |   '-(_{;}_) |___|  /  | |(_ o _) /    |( ''_'  ) |                 |( ''_'  ) | |(_ o _) /     `-'`"` |  | _ |  |  . (_ o _)  | 
 |      (_,_)     _.-`   | | (_,_).' __  | . (_) `. |                 | . (_) `. | | (_,_).' __   .---.  |  _( )_  |  |  (_,_)___| 
 | _ _--.   |  .'   _    | |  |\ \  |  | |(_    ._) '                 |(_    ._) ' |  |\ \  |  |  |   |  \ (_ o._) /  '  \   .---. 
 |( ' ) |   |  |  _( )_  | |  | \ `'   / |  (_.\.' /                  |  (_.\.' /  |  | \ `'   /  |   |   \ (_,_) /    \  `-'    / 
 (_{;}_)|   |  \ (_ o _) / |  |  \    /  |       .'                   |       .'   |  |  \    /   |   |    \     /      \       /  
 '(_,_) '---'   '.(_,_).'  ''-'   `'-'   '-----'`                     '-----'`     ''-'   `'-'    '---'     `---`        `'-..-'   
                 ,---.    ,---.     ,-----.     ,---.   .--. .-./`)  ,---------.      ,-----.     .-------.                        
                 |    \  /    |   .'  .-,  '.   |    \  |  | \ .-.') \          \   .'  .-,  '.   |  _ _   \                       
                 |  ,  \/  ,  |  / ,-.|  \ _ \  |  ,  \ |  | / `-' \  `--.  ,---'  / ,-.|  \ _ \  | ( ' )  |                       
                 |  |\_   /|  | ;  \  '_ /  | : |  |\_ \|  |  `-'`"`     |   \    ;  \  '_ /  | : |(_ o _) /                       
                 |  _( )_/ |  | |  _`,/ \ _/  | |  _( )_\  |  .---.      :_ _:    |  _`,/ \ _/  | | (_,_).' __                     
                 | (_ o _) |  | : (  '\_/ \   ; | (_ o _)  |  |   |      (_I_)    : (  '\_/ \   ; |  |\ \  |  |                    
                 |  (_,_)  |  |  \ `"/  \  ) /  |  (_,_)\  |  |   |     (_(=)_)    \ `"/  \  ) /  |  | \ `'   /                    
                 |  |      |  |   '. \_/``".'   |  |    |  |  |   |      (_I_)      '. \_/``".'   |  |  \    /                     
                 '--'      '--'     '-----'     '--'    '--'  '---'      '---'        '-----'     ''-'   `'-'                      
                                                                                                                                  


                                                                                           
)";
            std::cout << "Завантаження диска " << drive << ": ";
            bar(valueDiskTime.doubleValue);
            std::cout << std::endl << std::endl;

            std::cout << "Середній час відповіді (читання): " << std::fixed << std::setprecision(1)
                << valueAvgReadTime.doubleValue * 1000 << " ms" << std::endl << std::endl;
            std::cout << "Середній час відповіді (запис): " << std::fixed << std::setprecision(1)
                << valueAvgWriteTime.doubleValue * 1000 << " ms" << std::endl << std::endl;
            std::cout << "Швидкість читання: " << static_cast<int>(valueReadSpeed.doubleValue) / 1024 << " KB/s" << std::endl << std::endl;
            std::cout << "Швидкість запису: " << static_cast<int>(valueWriteSpeed.doubleValue) / 1024 << " KB/s" << std::endl << std::endl;
            std::cout << std::endl << std::endl << std::endl << "Натисніть Ctrl+C для завершення програми.";

        }
        else {
            std::cout << "Помилка отримання значення лічильника" << std::endl;
        }


        Sleep(1000);
    }

  

    PdhCloseQuery(query);
}

int main() {

    consoleColor(3, 15);
    char drive;
    std::cout << R"(
 .---.  .---.     ____     .-------.      ______                       ______      .-------.     .-./`)  ,---.  ,---.     .-''-.   
 |   |  |_ _|   .'  __ `.  |  _ _   \    |    _ `''.                  |    _ `''.  |  _ _   \    \ .-.') |   /  |   |   .'_ _   \  
 |   |  ( ' )  /   '  \  \ | ( ' )  |    | _ | ) _  \                 | _ | ) _  \ | ( ' )  |    / `-' \ |  |   |  .'  / ( ` )   ' 
 |   '-(_{;}_) |___|  /  | |(_ o _) /    |( ''_'  ) |                 |( ''_'  ) | |(_ o _) /     `-'`"` |  | _ |  |  . (_ o _)  | 
 |      (_,_)     _.-`   | | (_,_).' __  | . (_) `. |                 | . (_) `. | | (_,_).' __   .---.  |  _( )_  |  |  (_,_)___| 
 | _ _--.   |  .'   _    | |  |\ \  |  | |(_    ._) '                 |(_    ._) ' |  |\ \  |  |  |   |  \ (_ o._) /  '  \   .---. 
 |( ' ) |   |  |  _( )_  | |  | \ `'   / |  (_.\.' /                  |  (_.\.' /  |  | \ `'   /  |   |   \ (_,_) /    \  `-'    / 
 (_{;}_)|   |  \ (_ o _) / |  |  \    /  |       .'                   |       .'   |  |  \    /   |   |    \     /      \       /  
 '(_,_) '---'   '.(_,_).'  ''-'   `'-'   '-----'`                     '-----'`     ''-'   `'-'    '---'     `---`        `'-..-'   
                 ,---.    ,---.     ,-----.     ,---.   .--. .-./`)  ,---------.      ,-----.     .-------.                        
                 |    \  /    |   .'  .-,  '.   |    \  |  | \ .-.') \          \   .'  .-,  '.   |  _ _   \                       
                 |  ,  \/  ,  |  / ,-.|  \ _ \  |  ,  \ |  | / `-' \  `--.  ,---'  / ,-.|  \ _ \  | ( ' )  |                       
                 |  |\_   /|  | ;  \  '_ /  | : |  |\_ \|  |  `-'`"`     |   \    ;  \  '_ /  | : |(_ o _) /                       
                 |  _( )_/ |  | |  _`,/ \ _/  | |  _( )_\  |  .---.      :_ _:    |  _`,/ \ _/  | | (_,_).' __                     
                 | (_ o _) |  | : (  '\_/ \   ; | (_ o _)  |  |   |      (_I_)    : (  '\_/ \   ; |  |\ \  |  |                    
                 |  (_,_)  |  |  \ `"/  \  ) /  |  (_,_)\  |  |   |     (_(=)_)    \ `"/  \  ) /  |  | \ `'   /                    
                 |  |      |  |   '. \_/``".'   |  |    |  |  |   |      (_I_)      '. \_/``".'   |  |  \    /                     
                 '--'      '--'     '-----'     '--'    '--'  '---'      '---'        '-----'     ''-'   `'-'                      
                                                                                                                                  


                                                                                           
)" << "Оберіть диск для моніторингу (C, D тощо): ";
    std::cin >> drive;

    diskUsage(drive);


    return 0;
}
