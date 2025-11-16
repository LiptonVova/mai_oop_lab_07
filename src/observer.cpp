#include "../include/observer.h"


void FileObserver::update(const Npc &attacker, const Npc &defender) {
    log_file.open("../log.txt", std::ios::app);

    if (log_file.is_open()) {
        log_file << "[file observer]: ";
        attacker.save(log_file);
        log_file << "kill ";
        defender.save(log_file);
        log_file << '\n';
    }
    else {
        throw std::logic_error("File could not be opened");
    }
    log_file.close();
}

void StdinObserver::update(const Npc &attacker, const Npc &defender) {
    std::cout << "[stdin observer]:";
    attacker.print(std::cout) << ' ';
    std::cout << "kill ";
    defender.print(std::cout);
    std::cout << '\n';
}
