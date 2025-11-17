#include "../include/core.h"

#include <thread>

#include "../include/visitor_simulate_fight.h"

FightFunctor::FightFunctor(std::shared_ptr<bool> is_work_thread,
        std::shared_ptr<std::shared_mutex> mtx,
        std::shared_ptr<std::mutex> mtx_cout) :
    is_work_thread(is_work_thread), mtx(mtx), mtx_cout(mtx_cout) {
    events = std::make_shared<std::queue<FightEvent>>();
}
void FightFunctor::add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    events->emplace(attacker, defender);
}

FightFunctor::FightFunctor(const FightFunctor &other) {
    events = other.events;
    is_work_thread = other.is_work_thread;
    mtx = other.mtx;
    mtx_cout = other.mtx_cout;
}

bool FightFunctor::throw_the_dice() {
    unsigned int power_attack = (std::rand() % 6) + 1;
    unsigned int power_defender = (std::rand() % 6) + 1;

    if (power_attack > power_defender) {
        return true;
    }

    return false;
}

void FightFunctor::operator()() {
    while (*this->is_work_thread) {
        // брать из очереди ивент
        // если произошло убийство
        // то надо поменять в defender поле alive на false

        while (!events->empty()) {
            FightEvent current_event = events->front();
            events->pop();

            std::lock_guard<std::shared_mutex> lock(*mtx);
            std::shared_ptr<Npc>& attacker = current_event.attacker;
            std::shared_ptr<Npc>& defender = current_event.defender;
            if (attacker->is_alive()) {
                if (defender->is_alive()) {
                    if (attacker->accept(defender)) {
                        if (throw_the_dice()) {
                            defender->make_dead();
                            std::lock_guard<std::mutex> lock(*mtx_cout);
                            attacker->notify(*defender);
                        }
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool is_murder_available(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    if (attacker == defender) {
        return false;
    }

    const unsigned int max_x = std::max(attacker->get_x(), defender->get_x());
    const unsigned int min_x = std::min(attacker->get_x(), defender->get_x());
    const unsigned int max_y = std::max(attacker->get_y(), defender->get_y());
    const unsigned int min_y = std::min(attacker->get_y(), defender->get_y());

    const unsigned int distance_in_a_square = (max_x - min_x) * (max_x - min_x) + (max_y - min_y) * (max_y - min_y);
    if (distance_in_a_square > (attacker->get_distance_kill() * attacker->get_distance_kill())) {
        return false;
    }

    return true;
}

void MoveFunctor::operator()() {
    while (*this->is_work_thread) {
        // постоянно проходиться по сету npc
        // если кто-то умер, но он все равно в сете - удалить
        // если позволяет дистанция, то создать ивент в FightFunctor

        for (auto &attacker : set_npc) {
            std::lock_guard<std::shared_mutex> lock(*mtx);

            if (!attacker->is_alive()) {
                set_npc.erase(attacker);
                continue;
            }

            // логика шага
            int is_negative_x = std::rand() % 2;
            int is_negative_y = std::rand() % 2;

            int shift_x = attacker->get_distance_step();
            int shift_y = attacker->get_distance_step();

            if (is_negative_x) shift_x = -shift_x;
            if (is_negative_y) shift_y = -shift_y;

            attacker->move(shift_x, shift_y, MAX_VALUE);

            for (auto &defender : set_npc) {
                if (!defender->is_alive()) {
                    set_npc.erase(defender);
                    continue;
                }

                if (is_murder_available(attacker, defender)) {
                    // какая-то логика убийства
                    fight_functor_ptr->add_event(attacker, defender);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::set<std::shared_ptr<Npc> > generate_npc(const int MAX_VALUE) {
    std::set<std::shared_ptr<Npc> > set_npc;

    auto stdin_observer = std::make_shared<StdinObserver> ();
    auto log_file_observer = std::make_shared<FileObserver> ("../log.txt");

    const int N = 50;
    for (int i = 0; i < N; i++) {
        const int rand_type = std::rand() % 3;
        TypeNpc type = static_cast<TypeNpc>(rand_type);
        std::string name = "";
        switch (rand_type) {
            case 0:
                name = "dragon";
                break;
            case 1:
                name = "frog";
                break;
            case 2:
                name = "knight errant";
                break;
        }

        int x = std::rand() % MAX_VALUE;
        int y = std::rand() % MAX_VALUE;
        std::shared_ptr<Npc> npc = FactoryNpc::create_npc(type, name, x, y);
        npc->attach(stdin_observer);
        npc->attach(log_file_observer);
        set_npc.insert(npc);
    }

    return set_npc;
}

void print_alive_npc(const std::set<std::shared_ptr<Npc> > &set_npc) {
    std::cout << "Alive npc:\n";
    for (auto &npc :set_npc) {
        if (npc->is_alive()) {
            npc->print(std::cout) << '\n';
        }
    }
}



void start_programm() {
    // основной поток
    // сгенерировать npc
    // запустить два потока FightFunctor и MoveFunctor
    // генерировать карту через определенные промежутки времени

    const int MAX_VALUE = 100;

    std::set<std::shared_ptr<Npc> > set_npc = generate_npc(MAX_VALUE);

    std::shared_ptr <bool> is_work_thread = std::make_shared<bool>(true);
    std::shared_ptr<std::shared_mutex> mtx_ptr = std::make_shared<std::shared_mutex>();
    std::shared_ptr<std::mutex> mtx_cout_ptr = std::make_shared<std::mutex>();

    FightFunctor fight_functor(is_work_thread, mtx_ptr, mtx_cout_ptr);
    std::shared_ptr<FightFunctor> fight_functor_ptr = std::make_shared<FightFunctor>(fight_functor);
    MoveFunctor move_functor(set_npc, fight_functor_ptr, MAX_VALUE, is_work_thread, mtx_ptr, mtx_cout_ptr);

    std::thread fight_thread(std::ref(fight_functor));
    std::thread move_thread(std::ref(move_functor));

    auto begin = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

    std::vector<std::vector <char> > grid(MAX_VALUE + 1, std::vector(MAX_VALUE + 1, '.'));
    while (elapsed_time.count() < 5) {
        // логика генерации карты
        grid.assign(MAX_VALUE + 1, std::vector(MAX_VALUE + 1, '.'));

        {
            std::shared_lock<std::shared_mutex> lock(*mtx_ptr);
            for (auto &npc : set_npc) {
                if (!npc->is_alive()) continue;
                const unsigned int x = npc->get_x();
                const unsigned int y = npc->get_y();
                grid[x][y] = npc->info()[0] + ('A' - 'a');
            }
        }
        {
            std::lock_guard<std::mutex> lock(*mtx_cout_ptr);
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                    std::cout << grid[i][j];
                }
                std::cout << '\n';
            }
            std::cout << "\n\n\n\n\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        end = std::chrono::steady_clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    }

    *is_work_thread = false;

    move_thread.join();
    fight_thread.join();

    print_alive_npc(set_npc);
}