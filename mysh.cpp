#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>

std::vector<std::string> historyVector;

// ===============================================================================

void printDebug(std::string const str = "Debug")
{
    std::cout << str << std::endl;
}

void writeHistory()
{
    // Create/open history file
    std::ofstream historyFile("mysh.history");

    // Loop through history vector and write each command to file
    for (int i = 0; i < historyVector.size(); i++)
    {
        historyFile << historyVector[i] << std::endl;
    }

    historyFile.close();
}

void readHistory()
{
    // Open history file
    std::ifstream historyFile("mysh.history");
    std::string line;

    // Read each line and add to historyVector
    while (std::getline(historyFile, line))
    {
        historyVector.push_back(line);
    }

    historyFile.close();
}

void printHistory(std::vector<std::string> const &tokens)
{
    int count = 0;
    for (int i = historyVector.size() - 1; i >= 0; i--)
    {

        std::cout << std::to_string(count) + ": " + historyVector[i] << std::endl;
        count++;
    }
}

std::vector<std::string> tokenize(std::string const &command)
{
    std::vector<std::string> tokens;

    std::string arg = "";
    for (int i = 0; i < command.length(); i++)
    {
        if (command[i] != ' ')
        {
            // If current character is not a space, add the character to the argument string
            arg += command[i];

            // Check if on last character of string
            if (i == command.length() - 1)
            {
                tokens.push_back(arg);
            }
        }
        else
        {
            // If current character IS a space, add current arg string to tokens vector and clear the args string
            if (arg != "")
            {
                tokens.push_back(arg);
                arg = "";
            }
        }
    }
    return tokens;
}

bool isValidNumber(std::string const &numString)
{
    for (char const &c : numString)
    {
        if (std::isdigit(c) == 0)
            return false;
    }

    return true;
}

// ===============================================================================

void handleCommand(std::string const &command)
{
    // Add command to history
    historyVector.push_back(command);

    // Tokenize
    std::vector<std::string> tokens = tokenize(command);

    // Byebye
    if (tokens[0] == "byebye")
    {
        // Incorrect usage of 'byebye' command
        if (tokens.size() > 1)
        {
            std::cout << "Incorrect usage of command byebye. Correct usage: byebye" << std::endl;
        }
        // Write to history file and terminate program
        else
        {
            writeHistory();
            exit(EXIT_SUCCESS);
        }
    }
    // History
    else if (tokens[0] == "history")
    {
        // If no args passed, print history
        if (tokens.size() == 1)
        {
            printHistory(tokens);
        }
        // If "-c" flag passed, clear history
        else if (tokens.size() == 2 && tokens[1] == "-c")
        {
            historyVector.clear();
        }
        // Else, incorrect usage of command
        else
        {
            std::cout << "Incorrect usage of command history. Correct usage: history [-c]" << std::endl;
        }
    }
    // Replay
    else if (tokens[0] == "replay")
    {
        // Check if correct usage
        if (tokens.size() == 2)
        {
            // Check second arg for parsable num/num within size of token vector
            if (isValidNumber(tokens[1]) && std::stoi(tokens[1]) < tokens.size())
            {
                int commandNumber = std::stoi(tokens[1]) + 1;

                // Grab the command from the history vector and execute recursively
                std::string command = historyVector[historyVector.size() - 1 - commandNumber];
                handleCommand(command);
            }
            else
            {
                std::cout << "Incorrect usage of command replay. Please provide a valid number" << std::endl;
            }
        }
        else
        {
            std::cout << "Incorrect usage of command replay. Correct usage: replay number" << std::endl;
        }
    }
    // Start
    else if (tokens[0] == "start")
    {
        // Correct use validation
        if (tokens.size() >= 2)
        {
            const char *c_string = tokens[1].c_str();

            printf("%s\n", c_string);

            char *args[] = {(char *)c_string, NULL};
            execv(c_string, args);
            // if (fork() == 0)
            // {
            // }
        }
        // Incorrect use
        else
        {
            std::cout << "Incorrect usage of command start. Correct usage: start program [parameters]" << std::endl;
        }
    }
}

// ===============================================================================

int main()
{
    // Load history file
    readHistory();

    while (true)
    {
        // Prompt #
        std::cout << "# ";

        // Read in command
        std::string command;
        std::getline(std::cin, command);

        // Interpret/Execute the command
        handleCommand(command);
    }

    return 0;
}