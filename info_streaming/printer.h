#ifndef PRINT_PRINTER_H
#define PRINT_PRINTER_H

#include <iostream>
#include <chrono>

/**
 * @brief Color codes for printing in different colors
 * 
 */
#define BLACK "\033[30m"                /* Black */
#define RED "\033[31m"                  /* Red */
#define GREEN "\033[32m"                /* Green */
#define YELLOW "\033[33m"               /* Yellow */
#define BLUE "\033[34m"                 /* Blue */
#define MAGENTA "\033[35m"              /* Magenta */
#define CYAN "\033[36m"                 /* Cyan */
#define WHITE "\033[37m"                /* White */
#define RESET_COLOR "\033[0m"           /* Reset */

/**
 * @brief Printing macro to print the message in the specified color with timestamp
 * 
 */
#define PRINT(msg, colour) __TIME_STAMP(), std::cout << colour << msg << RESET_COLOR << std::endl;

/**
 * @brief Printing macro to print the message in the specified color with timestamp in printf style
 * 
 */
#define print_log(COLOR, f_, ...) printf(COLOR), __TIME_STAMP(), printf((f_), ##__VA_ARGS__), printf("\n"), printf(RESET_COLOR)

/**
 * @brief Macros to print info, warning and error messages in the specified color using info stream
 * 
 */
#define PRINT_INFO_STREAM(msg) PRINT(msg, WHITE);
#define PRINT_WARN_STREAM(msg) PRINT(msg, YELLOW);
#define PRINT_ERROR_STREAM(msg) PRINT(msg, RED);

/**
 * @brief Macro to print info message using printf style \n
 * Example: PRINT_INFO("Hello %s", "World");
 * specifiers: %s for string 
 * %d for integer 
 * %f for float
 * %c for character
 * %p for pointer
 * %u for unsigned integer 
 */
#define PRINT_INFO(msg, ...) print_log(WHITE, msg, ##__VA_ARGS__);

/**
 * @brief Macro to print info message using printf style \n
 * Example: PRINT_INFO("Hello %s", "World");
 * specifiers: %s for string 
 * %d for integer 
 * %f for float
 * %c for character
 * %p for pointer
 * %u for unsigned integer 
 */
#define PRINT_WARN(msg, ...) print_log(YELLOW, msg, ##__VA_ARGS__);

/**
 * @brief Macro to print info message using printf style \n
 * Example: PRINT_INFO("Hello %s", "World");
 * specifiers: %s for string 
 * %d for integer 
 * %f for float
 * %c for character
 * %p for pointer
 * %u for unsigned integer 
 */
#define PRINT_ERROR(msg, ...) print_log(RED, msg, ##__VA_ARGS__);

/**
 * @brief Macro to print assert message
 * 
 */
#define PRINT_ASSERT(Expr, Msg) __PRINT_ASSERT(#Expr, Expr, __FILE__, __LINE__, Msg)

/**
 * @brief Macro to print the file and line number for debugging
 * 
 */
#define PRINT_HOOK std::cout << __FILE__ << " Line " << __LINE__ << std::endl;

/**
 * @brief Macro to print the file and line number for debugging with custom message
 * 
 */
#define PRINT_HOOK_MSG(MSG) std::cout << __FILE__ << " Line " << __LINE__ << " [Message: " << MSG << "]" << std::endl;


/**
 * @brief Debug stream which prints in cyan color only when debug mode is on.
 * Else the function is overwritten with empty function
 */
#ifdef NDEBUG
#define PRINT_DEBUG_STREAM(msg) PRINT(msg, CYAN);
#else
#define PRINT_DEBUG_STREAM(msg)
#endif



/**
 * @brief Function to print assert message
 * 
 * @param expr_str assert expression
 * @param expr assert boolean
 * @param file file name in which assert is called
 * @param line file line number in which assert is called
 * @param msg custom message to print
 */
inline void __PRINT_ASSERT(const char *expr_str, bool expr, const char *file, int line, const char *msg)
{
    if (!expr)
    {
        std::cerr << RED << "Assert failed:\t" << msg << "\n"
                  << "Returned:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << RESET_COLOR;
        abort();
    }
}

/**
 * @brief Printing of the current time in the format [HH:MM:SS.NS]
 * 
 */
inline void __TIME_STAMP(){
   // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    // Get current nanoseconds
    auto now_timepoint = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    auto epoch = now_timepoint.time_since_epoch();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count() % 1000000000;

    printf("[%02i:%02d:%02d.%09ld] ", now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec, nanoseconds);
}


#endif // PRINT_PRINTER_H