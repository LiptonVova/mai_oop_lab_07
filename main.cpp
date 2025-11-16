#include <iostream>
#include <memory>

#include "include/core.h"
#include "include/factory_npc.h"
#include "include/observer.h"


int main() {
    // очищаю файл логов
    std::ofstream log_file("../log.txt", std::ios::out);

    std::cout << "Test factory npc\n\n";
    std::shared_ptr<Npc> dragon_ptr = FactoryNpc::create_npc(TypeNpc::dragon, "name_dragon", 0, 0);
    std::shared_ptr<Npc> frog_ptr = FactoryNpc::create_npc(TypeNpc::frog, "name_frog", 1, 1);
    std::shared_ptr<Npc> knight_errant_ptr = FactoryNpc::create_npc(TypeNpc::knight_errant, "name_knight", 1, 2);

    std::cout << "Test print npc: \n";
    dragon_ptr->print(std::cout) << '\n';
    frog_ptr->print(std::cout) << '\n';
    knight_errant_ptr->print(std::cout) << "\n\n";

    std::ifstream in;
    in.open("../input.txt", std::ios::in);

    std::cout << "Test factory from file:\n\n";
    std::shared_ptr<Npc> frog_ptr_from_file = FactoryNpc::create_npc_from_file(TypeNpc::frog, in);
    std::shared_ptr<Npc> knight_errant_ptr_from_file = FactoryNpc::create_npc_from_file(TypeNpc::knight_errant, in);
    std::shared_ptr<Npc> dragon_ptr_from_file = FactoryNpc::create_npc_from_file(TypeNpc::dragon, in);

    std::cout << "Test print npc from file: \n";

    std::cout << dragon_ptr_from_file->info() << '\n';
    std::cout << frog_ptr_from_file->info() << '\n';
    std::cout << knight_errant_ptr_from_file->info() << "\n\n";

    std::cout << "Test save npc in file\n";

    std::ofstream os("../output.txt", std::ios::out);

    dragon_ptr->save(os);
    os << '\n';
    frog_ptr->save(os);
    os << '\n';
    knight_errant_ptr->save(os);
    os << '\n';

    os.close();

    std::cout << "Npc saved in file 'output.txt'\n\n";


    std::cout << "Test fight \n";
    std::cout << "Dragon attacked dragon and he " << (dragon_ptr_from_file->accept(dragon_ptr) == true ? "killed" : "didnt kill") << '\n';
    std::cout << "Dragon attacked knight and he " << (dragon_ptr_from_file->accept(knight_errant_ptr) == true ? "killed" : "didnt kill") << "\n\n";

    auto stdin_observer = std::make_shared<StdinObserver>();
    auto log_observer = std::make_shared<FileObserver>();
    dragon_ptr->attach(stdin_observer);
    dragon_ptr->attach(log_observer);

    std::cout << "Test notify: \n";

    dragon_ptr->notify(*frog_ptr);

    std::cout << "after detach : \n";
    dragon_ptr->detach(stdin_observer);

    dragon_ptr->notify(*frog_ptr);

    std::cout << '\n';


    std::set<std::shared_ptr<Npc>> set_npc;
    set_npc.insert(dragon_ptr);
    set_npc.insert(frog_ptr);
    set_npc.insert(knight_errant_ptr);

    knight_errant_ptr->attach(stdin_observer);
    knight_errant_ptr->attach(log_observer);

    dragon_ptr_from_file->attach(stdin_observer);
    dragon_ptr_from_file->attach(log_observer);

    frog_ptr_from_file->attach(stdin_observer);
    frog_ptr_from_file->attach(log_observer);

    frog_ptr->attach(stdin_observer);
    frog_ptr->attach(log_observer);

    knight_errant_ptr->attach(stdin_observer);
    knight_errant_ptr->attach(log_observer);

    set_npc.insert(knight_errant_ptr);
    set_npc.insert(dragon_ptr_from_file);
    // set_npc.insert(frog_ptr_from_file);

    std::cout << "Test print all npc from set before fight: \n";
    print_all_npc(std::cout, set_npc) << "\n\n";


    start_fight(set_npc, 1);

    std::cout << "Test print all npc from set after fight: \n";
    print_all_npc(std::cout, set_npc) << "\n";
    return 0;
}
