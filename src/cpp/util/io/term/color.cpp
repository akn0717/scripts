#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#define EXECNAME "color"

const std::unordered_map<std::string, std::string> colors = {
    { "black", "\x1b[0;30m" },   { "red", "\x1b[0;31m" },    { "green", "\x1b[0;32m" },
    { "orange", "\x1b[0;33m" },  { "blue", "\x1b[0;34m" },   { "purple", "\x1b[0;35m" },
    { "cyan", "\x1b[0;36m" },    { "lgray", "\x1b[0;37m" },  { "lgrey", "\x1b[0;37m" },
    { "dgray", "\x1b[1;30m" },   { "dgrey", "\x1b[1;30m" },  { "lred", "\x1b[1;31m" },
    { "lgreen", "\x1b[1;32m" },  { "yellow", "\x1b[1;33m" }, { "lblue", "\x1b[1;34m" },
    { "lpurple", "\x1b[1;35m" }, { "lcyan", "\x1b[1;36m" },  { "white", "\x1b[1;37m" },
};

const std::string clear_color = "\x1b[0m";

int main(const int argc, const char* argv[])
{
    if (argc < 2) {
        std::cout << EXECNAME ": no argument given" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        std::cout << colors.at(argv[1]);
    } catch (const std::out_of_range& e) {
        std::cout << EXECNAME ": incorrect color given. color must be one of: ";

        std::vector<std::string> color_names;
        color_names.reserve(colors.size());
        for (const auto& [colorName, color] : colors)
            color_names.push_back(colorName);

        std::sort(color_names.begin(), color_names.end());

        for (auto it = color_names.begin(); it < color_names.end() - 1; it++)
            std::cout << *it << ", ";
        std::cout << color_names.back() << '.' << std::endl;

        return EXIT_FAILURE;
    }

    if (argc == 2)
        std::cout << std::cin.rdbuf();
    else
        for (auto const arg : std::views::counted(argv + 2, argc - 2))
            std::cout << arg << '\n';

    std::cout << clear_color;

    return EXIT_SUCCESS;
}
