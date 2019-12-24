#pragma once

#include <string>

/**
 * ANSI escape codes for terminal colours
 */

/*
 * Windows 10
 * Change Terminal Colors of Console Window
 * 1. Right-click on the title bar of its window and select Properties from the context menu.
 * 2. Switch to the Terminal tab.
 * 3. Under Terminal Colors, enable the option Use Separate Foreground to change the text color.
 * 4. Fill in the Red, Green, and Blue boxes for the desired color value.
 */

const std::string RESET =   "\033[0m";
const std::string BLACK =   "\033[30m";     /* Black */
const std::string RED   =   "\033[31m";     /* Red */
const std::string GREEN =   "\033[32m";     /* Green */
const std::string YELLOW=   "\033[33m";     /* Yellow */
const std::string BLUE  =   "\033[34m";     /* Blue */
const std::string MAGENTA = "\033[35m";     /* Magenta */
const std::string CYAN  =   "\033[36m";     /* Cyan */
const std::string WHITE =   "\033[37m";     /* White */
const std::string BOLDBLACK =   "\033[1m\033[30m";     /* Bold Black */
const std::string BOLDRED   =   "\033[1m\033[31m";     /* Bold Red */
const std::string BOLDGREEN =   "\033[1m\033[32m";     /* Bold Green */
const std::string BOLDYELLOW=   "\033[1m\033[33m";     /* Bold Yellow */
const std::string BOLDBLUE  =   "\033[1m\033[34m";     /* Bold Blue */
const std::string BOLDMAGENTA = "033[1m\033[35m";      /* Bold Magenta */
const std::string BOLDCYAN  =   "\033[1m\033[36m";     /* Bold Cyan */
const std::string BOLDWHITE =   "\033[1m\033[37m";     /* Bold White */
