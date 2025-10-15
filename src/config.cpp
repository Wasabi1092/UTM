#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// helper to split key paths
std::vector<std::string> splitKey(const std::string &path)
{
    std::vector<std::string> keys;
    std::stringstream ss(path);
    std::string token;
    while (std::getline(ss, token, '.'))
        keys.push_back(token);
    return keys;
}

// load json file
json loadConfig(const std::string &path)
{
    std::ifstream file(path);
    json j;
    if (file.is_open())
    {
        file >> j;
        file.close();
    }
    else
    {
        std::cerr << "Could not open " << path << "\n";
    }
    return j;
}
