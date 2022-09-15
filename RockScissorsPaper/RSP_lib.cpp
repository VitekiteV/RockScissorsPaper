#include <iostream>
#include <random>
#include "RSP_lib.h"
#include <vector>
#include <array>
#include <fstream>
#include <iomanip>

enum Result
{
	win,
	draw,
	lose
};

enum Pattern
{
	random,
	analyse_1
};


void Menu()
{
	std::array<int, 3> game_results = LoadResults();
	std::vector<char> player_choices = LoadChoices("player_choices.bin");
	std::vector<char> cpu_choices = LoadChoices("cpu_choices.bin");
	int cpu_pattern = random;

	char input = ' ';
	while (input != 'q')
	{
		std::cout << "===================\n";
		const auto str = R"(Main menu:
1 - play the game
2 - configure computer's choice pattern
3 - save statistic
4 - check winrate
q - quit application
)";
		std::cout << str;
		std::cout << "===================\n";
		std::cin >> input;
		if (input == '1')
			Play(game_results, player_choices, cpu_choices, cpu_pattern);
		else if (input == '2')
			ConfigCPU(cpu_pattern);
		else if (input == '3')
			SaveStatistic(game_results, player_choices, cpu_choices);
		else if (input == '4')
			CheckWinrate(game_results);
		else if (input != 'q')
			std::cout << "Wrong character!\n";
	}

}

void Play(std::array<int, 3>& game_results, std::vector<char>& player_choices, std::vector<char>& cpu_choices, int cpu_pattern)
{
	static int counter;
	char input = ' ';
	while (input != '0')
	{
		std::cout << "===================\n";
		const auto str = R"(Game menu:
r - chose "rock"
s - chose "scissors"
p - chose "paper"
0 - quit to main menu
)";
		std::cout << str;
		std::cout << "===================\n";
		std::cin >> input;
		if (input != '0')
		{
			if (input == 'r' || input == 's' || input == 'p')
			{
				GameRound(counter, input, game_results, player_choices, cpu_choices, cpu_pattern);
				++counter;
			}
			else
				std::cout << "Wrong character!\n";
		}
	}
}

void SaveStatistic(const std::array<int, 3>& game_results, const std::vector<char>& player_choices, const std::vector<char>& cpu_choices)
{
	ArrayToFile(game_results, "results.bin");
	ArrayToFile(player_choices, "player_choices.bin");
	ArrayToFile(cpu_choices, "cpu_choices.bin");
}

void GameRound(int turn, char choice, std::array<int, 3>& game_results, std::vector<char>& player_choices, std::vector<char>& cpu_choices, int cpu_pattern)
{
	std::cout << "Turn " << turn << "\n";
	char cpu_choice = GetCpuChoice(cpu_pattern, player_choices);
	int res = GetRoundResult(choice, cpu_choice);
	if (res == win)
		++game_results[0];
	else if (res == draw)
		++game_results[1];
	else if (res == lose)
		++game_results[2];
	player_choices.push_back(choice);
	cpu_choices.push_back(cpu_choice);

}

char GetCpuChoice(int cpu_pattern, const std::vector<char>& player_choices)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	if (cpu_pattern == analyse_1)
	{
		std::array<int, 3> rsp = { 0 };
		if (player_choices.size() > 1)
		{
			char last_symbol = player_choices[player_choices.size() - 1];
			for (auto i = 0; i < player_choices.size() - 1; ++i)
			{
				if (player_choices[i] == last_symbol)
				{
					if (player_choices[i + 1] == 'r')
						++rsp[0];
					else if (player_choices[i + 1] == 's')
						++rsp[1];
					else if (player_choices[i + 1] == 'p')
						++rsp[2];
				}
			}
		}

		int counter = 0;
		int max = 0;
		for (const auto& val : rsp)
			if (val > max)
			{
				max = val;
				counter = 1;
			}
			else if (val == max)
				++counter;
		if (counter > 1)
		{
			std::uniform_int_distribution<> distrib(1, counter);
			int res = distrib(gen);
			for (auto i = 0; i < rsp.size(); ++i)
				if (rsp[i] == max && res == 1)
					return IntToChar(i);
				else if (rsp[i] == max && res != 1)
					--res;
		}
		else
		{
			for (auto i = 0; i < rsp.size(); ++i)
				if (rsp[i] == max)
					return IntToChar(i);
		}
		return ' ';
	}
	else
	{
		std::uniform_int_distribution<> distrib(1, 3);
		int res = distrib(gen);
		if (res == 1)
			return 'r';
		else if (res == 2)
			return 's';
		else
			return 'p';
	}
}

int GetRoundResult(char choice, char cpu_choice)
{
	auto choice_str = CharToString(choice);
	auto cpu_choice_str = CharToString(cpu_choice);
	if (choice == cpu_choice)
	{
		std::cout << choice_str << " = " << cpu_choice_str;
		std::cout << "\nDRAW\n";
		return draw;
	}
	else if (choice == 'r')
	{
		if (cpu_choice == 's')
		{
			std::cout << choice_str << " > " << cpu_choice_str;
			std::cout << "\nWIN\n";
			return win;
		}
		else if (cpu_choice == 'p')
		{
			std::cout << choice_str << " < " << cpu_choice_str;
			std::cout << "\nLOSE\n";
			return lose;
		}
	}
	else if (choice == 's')
	{
		if (cpu_choice == 'p')
		{
			std::cout << choice_str << " > " << cpu_choice_str;
			std::cout << "\nWIN\n";
			return win;
		}
		else if (cpu_choice == 'r')
		{
			std::cout << choice_str << " < " << cpu_choice_str;
			std::cout << "\nLOSE\n";
			return lose;
		}
	}
	else if (choice == 'p')
	{
		if (cpu_choice == 'r')
		{
			std::cout << choice_str << " > " << cpu_choice_str;
			std::cout << "\nWIN\n";
			return win;
		}
		else if (cpu_choice == 's')
		{
			std::cout << choice_str << " < " << cpu_choice_str;
			std::cout << "\nLOSE\n";
			return lose;
		}
	}
	return 4;
}

std::string CharToString(char input)
{
	if (input == 'r')
		return "rock";
	else if (input == 's')
		return "scissors";
	else
		return "paper";
}

template<typename T>
void ArrayToFile(const T& arr, const char* file_name)
{
	std::fstream file(file_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	for (const auto val : arr)
		file.write(reinterpret_cast<const char*>(&val), sizeof(val));
	file.close();
}

std::array<int, 3> LoadResults()
{
	std::array<int, 3> res = { 0 };
	std::fstream file("results.bin");
	if (file.is_open())
	{
		for (auto& val : res)
			file.read(reinterpret_cast<char*>(&val), sizeof(val));
	}
	return res;
}

std::vector<char> LoadChoices(const char* file_name)
{
	std::vector<char> res;
	std::fstream file(file_name, std::ios_base::in);
	if (file.is_open())
	{
		char symbol = ' ';
		while (!file.eof())
		{
			file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
			res.push_back(symbol);
		}
	}
	return res;
}

void CheckWinrate(const std::array<int,3>& arr)
{
	double sum = arr[0] + arr[1] + arr[2];
	std::cout << "===================\n";
	std::cout << "Wins: " << arr[0] << "(" << std::setprecision(4) << 100*arr[0]/sum << ")\n" <<
				"Draws: " << arr[1] << "(" << std::setprecision(4) << 100*arr[1]/sum << ")\n" <<
				"Loses: " << arr[2] << "(" << std::setprecision(4) << 100*arr[2]/sum << ")\n";
	double sum_w_l = arr[0] + arr[2];
	std::cout << "Winrate: " << std::setprecision(4) << 100*arr[0]/sum_w_l << "\n";
	std::cout << "===================\n";
}

void ConfigCPU(int& cpu_pattern)
{
	char input = ' ';
	while (input != '0')
	{
		std::cout << "===================\n";
		const auto str = R"(Configuration menu:
1 - computer choice randomly(set by default)
2 - computer choice more smart"
0 - quit to main menu
)";
		std::cout << str;
		std::cout << "===================\n";
		std::cin >> input;
		if (input == '1')
		{
			cpu_pattern = random;
			break;
		}
		else if (input == '2')
		{
			cpu_pattern = analyse_1;
			break;
		}
		else
			std::cout << "Wrong character!\n";
	}
	
}

char IntToChar(int input)
{
	if (input == 0)
		return 'r';
	else if (input == 1)
		return 's';
	else
		return 'p';
}