#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

namespace config
{
    json data = {};

    // save config file
    bool save(const std::string &filename = "config.json")
    {
        std::ofstream f(filename);
        if (!f.is_open())
        {
            std::cerr << "⚠️ Could not open file for writing: " << filename << std::endl;
            return false;
        }

        // write config file with data
        f << std::setw(4) << data;
        f.close();
        return true;
    }

    // create default config file
    void createDefault(const std::string &filename = "config.json")
    {
        data = {
            {"subjects", json::array()}};

        save(filename);
    }

    // load config file
    bool load(const std::string &filename = "config.json")
    {
        std::ifstream f(filename);
        if (!f.is_open())
        {
            std::cout << "⚠️ Config file not found. Creating a new one..." << std::endl;
            createDefault(filename);
            return true;
        }

        try
        {
            f >> data;
        }
        catch (const std::exception &e)
        {
            std::cerr << "⚠️ Error reading config: " << e.what() << std::endl;
            return false;
        }

        f.close();
        return true;
    }

    // add subject to config file
    void addSubject(const std::string &name, const std::string &colour)
    {
        if (!data.contains("subjects"))
        {
            data["subjects"] = json::array();
        }

        // check for duplicates
        for (auto &sub : data["subjects"])
        {
            if (sub["name"] == name)
            {
                std::cout << "⚠️ Subject '" << name << "' already exists." << std::endl;
                return;
            }
        }

        data["subjects"].push_back({{"name", name}, {"colour", colour}});
        save();
        std::cout << "✅ Added subject: " << name << " (" << colour << ")" << std::endl;
    }

    // print subjects
    void printSubjects()
    {
        if (!data.contains("subjects") || data["subjects"].empty())
        {
            std::cout << "No subjects defined yet" << std::endl;
            return;
        }

        std::cout << "\n📙 Subjects:\n";
        for (auto &sub : data["subjects"])
        {
            std::cout << "- " << sub["name"] << " (" << sub["colour"] << ")\n";
        }
        std::cout << std::endl;
    }

    // return list of subjects
    std::vector<std::string> getSubjects()
    {
        std::vector<std::string> list;
        if (!data.contains("subjects"))
            return list; // empty list
        for (auto &sub : data["subjects"])
        {
            list.push_back(sub["name"]);
        }
        return list;
    }

    // helper to split key paths
    std::vector<std::string>
    splitKey(const std::string &path)
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
}