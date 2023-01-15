//  EECS 281 Project 3 Log Manager
//
//  jamiean; JunCheng An
//
//  Created by JunCheng An on 5/26/18.
//  Copyright © 2018 JunCheng An. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include "string.h"
#include <unordered_map>
#include <unordered_set>

class helperFunction{};
class FileNotFound{};
class commandError{};

class Logm {
public:
    
    // Required: argv is valid command line arguments stats
    // Effect  : read in the command line argument, error checking
    //           read all the inputs
    void read_in(char* argv[]);
    
    // Effect  : Main running functions
    //           Read user command and do main things that should do
    //           see specific function command for details
    void run();
    
    
    // Effect  : print everything we want for debugging
    void debugger();


private:
    
    struct log {
        unsigned long long int_time;
        std::string time;
        std::string cat;
        std::string mes;
        int index;
    };
    
    
    
    
    std::vector<int> prev_ind;
    std::vector<log> main_log;
    std::deque<int> excerpt;
    std::vector<int> time_order;
    std::unordered_map<std::string, std::vector<int>> k_map;
    std::unordered_map<std::string, std::vector<int>> c_map;
    
    
    bool searched = false;
    bool timed = false;
    bool cated = false;
    bool sorted = true;
    
    
    std::vector<log>::iterator first;
    std::vector<log>::iterator end;
    std::string cat;
    log* output_p;
    std::unordered_map<std::string, std::vector<int>>::iterator catg;
    std::unordered_map<std::string, std::vector<int>>::iterator it_read;

    
    
/***************************************************************************
 *                       BELOW ARE HELPER FUNCTIONS                        *
 ***************************************************************************/
    
    // Required: lhs and rhs are valid log struct
    // Effect  : Compaire priority: timestamp -> category -> index
    //           if lhs is lower, return true; otherwise, false
    //           if it's a long long int, only compare with int_time
    struct original_sort {
        bool operator()(const log &lhs, const log& rhs) {
            if (lhs.int_time != rhs.int_time) return lhs.int_time < rhs.int_time;
            helper = strcasecmp(lhs.cat.c_str(),rhs.cat.c_str());
            if (helper < 0)  return true;
            else if (helper > 0) return false;
            return lhs.index < rhs.index;
        }
        
        int helper = 0;
    };
    
    
    // Required: lhs and rhs one long long timestamp and one log
    // Effect  : if lhs is lower, return true; otherwise, false
    //           if it's a long long int, only compare with int_time
    class timestamp_comp {
    public:
        bool operator()(const unsigned long long lhs, log &rhs) const {
            return lhs < rhs.int_time;
        }
        bool operator()(log &lhs, const unsigned long long rhs) const {
            return lhs.int_time < rhs;
        }
    };
    
    
    // Requried: vector "main_log" is already sorted
    // Effect  : push the index position in vector "time_order" into
    //           key and category map vector
    void store_key();
    
    
    // Required: word is a effective word with some space or chars in between
    //           current index is the cuurent index of ORDERED vector
    // Effect  : seperater word by space and non-alg-num chars
    //           and push it's current index into our k(or c)_map with NO duplicate
    //           we do not care about upper and lower case
    void cleanup_c(std::string word, int cur_ind);
    
    
    // Required: word is a effective word with some space or chars in between
    //           current index is the cuurent index of ORDERED vector
    // Effect  : seperater word by space and non-alg-num chars
    //           and push it's current index into our k(or c)_map with NO duplicate
    //           we do not care about upper and lower case
    void cleanup_k(std::string &word, int cur_ind);
    
    // Required: time is a string in the following format:
    //                     xx:xx:xx:xx:xx
    //           where x is a integer.
    // Effect  : return the integer after eliminating all the collens
    unsigned long long timestamp(std::string &word);
    
    // Required: time is a string in the following format:
    //                     xx:xx:xx:xx:xx
    //           where x is a integer, with ONE more space in front
    // Effect  : return the integer after eliminating all the collens
    unsigned long long timestamp_t(std::string &word);
    
    
    // Effect  : change word to lower case
    void to_lower_case(std::string &word);
    
    
/***************************************************************************
 *                      BELOW ARE COMMAND FUNCTIONS                        *
 ***************************************************************************/
    
    // Modiefier: cin
    // Effect   : read in a string in format: xx:xx:xx:xx:xx|xx:xx:xx:xx:xx (x ints)
    //            find lower bound of first timestamp and upper bound of second
    //            push the elements in between into prev_ind vector and return size
    //            Error checking: check if timestamps have 14 chars
    //                            change searched to true if valid
    void command_t();
    
    
    // Required : comp is a valid comparator
    // Modiefier: cin
    // Effect   : read in a string in format: xx:xx:xx:xx:xx (x is an int)
    //            find the upper and lower bound of the specific timestamp
    //            the items in between are what we want
    //            push them into prev_index vector
    void command_m();
    
    
    // Modiefier: cin
    // Effect   : read in a category and turn it to lower case
    //            record the category to cat string
    //            change mod to be previous mode
    void command_c();
    
    
    // Modiefier: cin
    // Effect   : read in a message and separate into substrings
    //            read their maps and get intersection of the map vectors
    //            record the answer to our prev_index vector
    //            loop through one by one to find intersection
    void command_k();
    
    
    // Modiefier: cin
    // Effect   : read in an interger indicating index
    //            append the index in SORT containter to excerpt list
    //            use the time_order to find the SORT index
    //            ERROR CHECKING: entry is not valid
    void command_a();
    
    
    // Effect   : check which operation has already done
    //            append the previous done index to excerpt list
    //            ERROR CHECKING: no previous operation done previously
    void command_r();
    
    
    // Modiefier: cin
    // Effect   : read in an interger indicating index
    //            delete the index in excerpt list
    //            ERROR CHECKING: entry is not valid
    void command_d();
    
    
    // Modiefier: cin
    // Effect   : read in an interger indicating index
    //            put the index into the beginning of excerpt list
    //            ERROR CHECKING: entry is not valid
    void command_b();
    
    
    // Modiefier: cin
    // Effect   : read in an interger indicating index
    //            put the index into the back of excerpt list
    //            ERROR CHECKING: entry is not valid
    void command_e();
    
    
    // Modiefier: cout
    // Effect   : sort the vector if needed
    //            Print the necessary long message required
    void command_s();
    
    
    // Modiefier: cout
    // Effect   : sort the vector if needed
    //            Print the necessary long message required
    void command_l();
    
    
    // Modiefier: cout
    // Effect   : print the previously search log entries
    //            Error checking: no previous search done
    void command_g();

    
    // Modiefier: cout
    // Effect   : print the excerpt list
    void command_p();
    
    
};



