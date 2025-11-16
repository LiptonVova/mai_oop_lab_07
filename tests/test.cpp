#include <gtest/gtest.h>
#include <memory>
#include <set>
#include <fstream>
#include <filesystem>

#include "../include/core.h"
#include "../include/factory_npc.h"
#include "../include/observer.h"
#include "../include/visitor_simulate_fight.h"

class RPGTest : public ::testing::Test {
protected:
    void SetUp() override {
        dragon = FactoryNpc::create_npc(TypeNpc::dragon, "name_dragon");
        frog = FactoryNpc::create_npc(TypeNpc::frog, "name_frog");
        knight = FactoryNpc::create_npc(TypeNpc::knight_errant, "name_knight_errant");
    }
    
    void TearDown() override {
        // Очистка файлов после тестов
        if (std::filesystem::exists("test_save.txt")) {
            std::filesystem::remove("test_save.txt");
        }
        if (std::filesystem::exists("log.txt")) {
            std::filesystem::remove("log.txt");
        }
    }
    
    std::shared_ptr<Npc> dragon;
    std::shared_ptr<Npc> frog;
    std::shared_ptr<Npc> knight;
};

// Тест фабрики NPC
TEST_F(RPGTest, FactoryCreatesCorrectTypes) {
    EXPECT_NE(dragon, nullptr);
    EXPECT_NE(frog, nullptr);
    EXPECT_NE(knight, nullptr);
    
    // Проверяем, что созданы правильные типы через info()
    EXPECT_TRUE(dragon->info().find("dragon") != std::string::npos);
    EXPECT_TRUE(frog->info().find("frog") != std::string::npos);
    EXPECT_TRUE(knight->info().find("knight") != std::string::npos);
}

// Тест координат
TEST_F(RPGTest, CoordinatesAreValid) {
    EXPECT_GE(dragon->get_x(), 0);
    EXPECT_LE(dragon->get_x(), 500);
    EXPECT_GE(dragon->get_y(), 0);
    EXPECT_LE(dragon->get_y(), 500);
    
    EXPECT_GE(frog->get_x(), 0);
    EXPECT_LE(frog->get_x(), 500);
    EXPECT_GE(frog->get_y(), 0);
    EXPECT_LE(frog->get_y(), 500);
}

// Тест уникальных имен
TEST_F(RPGTest, UniqueNames) {
    EXPECT_FALSE(dragon->get_name().empty());
    EXPECT_FALSE(frog->get_name().empty());
    EXPECT_FALSE(knight->get_name().empty());
    
    // Имена должны быть уникальными
    EXPECT_NE(dragon->get_name(), frog->get_name());
    EXPECT_NE(dragon->get_name(), knight->get_name());
    EXPECT_NE(frog->get_name(), knight->get_name());
}

// Тест боевых взаимодействий (Visitor pattern)
TEST_F(RPGTest, BattleInteractions) {
    EXPECT_FALSE(dragon->accept(frog));
    EXPECT_TRUE(dragon->accept(knight));

    EXPECT_TRUE(frog->accept(knight));
    EXPECT_TRUE(frog->accept(frog));
    EXPECT_TRUE(frog->accept(dragon));


    EXPECT_TRUE(knight->accept(dragon));
    EXPECT_FALSE(knight->accept(frog));
}

// Тест Observer pattern
TEST_F(RPGTest, ObserverPattern) {
    testing::internal::CaptureStdout(); // Перехватываем stdout
    
    auto stdin_observer = std::make_shared<StdinObserver>();
    frog->attach(stdin_observer);
    
    frog->notify(*dragon);
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("kill") != std::string::npos);
}

// Тест симуляции боя
TEST_F(RPGTest, BattleSimulation) {
    std::set<std::shared_ptr<Npc>> npcs;
    
    // Создаем небольшую группу NPC
    npcs.insert(FactoryNpc::create_npc(TypeNpc::dragon, "name_dragon"));
    npcs.insert(FactoryNpc::create_npc(TypeNpc::frog, "name_frog"));
    npcs.insert(FactoryNpc::create_npc(TypeNpc::knight_errant, "name_knight_errant"));
    
    size_t initial_size = npcs.size();
    
    // Запускаем бой с большой дистанцией
    start_fight(npcs, 1000);
    
    // После боя должно остаться меньше NPC
    EXPECT_LT(npcs.size(), initial_size);

    bool frog_survived = false;
    for (const auto& npc : npcs) {
        if (npc->info().find("frog") != std::string::npos) {
            frog_survived = true;
            break;
        }
    }
    EXPECT_TRUE(frog_survived);
}


// Тест детача наблюдателей
TEST_F(RPGTest, ObserverDetach) {
    testing::internal::CaptureStdout();
    
    auto stdin_observer = std::make_shared<StdinObserver>();
    dragon->attach(stdin_observer);
    
    // Первое уведомление - должно быть в stdout
    dragon->notify(*frog);
    std::string output1 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output1.empty());
    
    // Детачим наблюдателя
    dragon->detach(stdin_observer);
    
    testing::internal::CaptureStdout();
    // Второе уведомление - не должно быть в stdout
    dragon->notify(*frog);
    std::string output2 = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output2.empty());
}

// Тест множества NPC
TEST_F(RPGTest, MultipleNPCManagement) {
    std::set<std::shared_ptr<Npc>> npc_set;
    
    // Добавляем несколько NPC
    for (int i = 0; i < 5; ++i) {
        npc_set.insert(FactoryNpc::create_npc(TypeNpc::dragon, "name_dragon"));
        npc_set.insert(FactoryNpc::create_npc(TypeNpc::frog, "name_frog"));
    }
    
    EXPECT_EQ(npc_set.size(), 10);
    
    // Проверяем печать всех NPC
    testing::internal::CaptureStdout();
    print_all_npc(std::cout, npc_set);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("dragon") != std::string::npos);
    EXPECT_TRUE(output.find("frog") != std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}