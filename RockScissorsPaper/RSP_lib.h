#pragma once
#include <string>
#include <vector>
#include <array>

void Menu();
void Play(std::array<int,3>& game_results, std::vector<char>& player_choices, std::vector<char>& cpu_choices, int cpu_pattern);
void SaveStatistic(const std::array<int, 3>& game_results, const std::vector<char>& player_choices, const std::vector<char>& cpu_choices);
void GameRound(int turn, char pick, std::array<int, 3>& game_results, std::vector<char>& player_choices, std::vector<char>& cpu_choices, int cpu_pattern);
char GetCpuChoice(int cpu_pattern, const std::vector<char>& player_choices);
int GetRoundResult(char choice, char cpu_choice);
std::string CharToString(char input);
template<typename T>
void ArrayToFile(const T& array, const char* file_name);
std::array<int, 3> LoadResults();
std::vector<char> LoadChoices(const char* file_name);
void CheckWinrate(const std::array<int, 3>& arr);
void ConfigCPU(int& cpu_pattern);
char IntToChar(int input);