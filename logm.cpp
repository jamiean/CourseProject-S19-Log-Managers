//  EECS 281 Project 3 Log Manager
//
//  jamiean; JunCheng An
//
//  Created by JunCheng An on 5/26/18.
//  Copyright © 2018 JunCheng An. All rights reserved.


#include "logm.h"

// Required: argv is valid command line arguments stats
// Effect  : read in the command line argument, error checking
//           read all the inputs
void Logm::read_in(char **argv) {
    
    // COMMAND LINE read and checking
    std::string arg(argv[1]);
    if (arg == "-h" || arg == "--help") throw helperFunction();
    
    std::ifstream input(arg);
    if (!input.is_open()) throw FileNotFound();
    
    
    // log_file reading
    int cur_index = 0;
    log input_l;
    std::string input_s;
    while(getline(input, input_s, '|')) {
        
        // TIME reading
        input_l.time = input_s;
        input_l.int_time = timestamp(input_s);
        
        // CATEGORY reading
        getline(input, input_s, '|');
        input_l.cat = input_s;
        
        // MESSAGE reading
        getline(input, input_s);
        input_l.mes = input_s;
        
        // INDEX location
        input_l.index = cur_index;
        
        main_log.push_back(input_l);
        cur_index ++;
    }
    std::cout << cur_index << " entries read" << '\n';
    std::sort(main_log.begin(), main_log.end(), original_sort());
}



// Effect  : Main running functions
//           Read user command and do main things that should do
//           see specific function command for details
void Logm::run() {
    std::string command;
    bool quit = false;
    
    // sort the original vector and
    // record the position of original one
    // put the index into the map vector, with NO dupilicate
    std::sort(main_log.begin(), main_log.end(), original_sort());
    time_order.reserve(main_log.size());
    for(int i = 0; i < int(main_log.size()); i++) time_order[main_log[i].index] = i;
    store_key();
    
    
    
    while (!quit) {
        std::cout << "% ";
        std::cin >> command;
        
        try {
            switch (command[0]) {
                case 't': {
                    command_t();
                    break;
                }
                case 'm': {
                    command_m();
                    break;
                }
                case 'c': {
                    command_c();
                    break;
                }
                case 'k': {
                    command_k();
                    break;
                }
                case 'a': {
                    command_a();
                    break;
                }
                case 'r': {
                    command_r();
                    break;
                }
                case 'd': {
                    command_d();
                    break;
                }
                case 'b': {
                    command_b();
                    break;
                }
                case 'e': {
                    command_e();
                    break;
                }
                case 'p': {
                    command_p();
                    break;
                }
                case 's': {
                    command_s();
                    break;
                }
                case 'l': {
                    command_l();
                    break;
                }
                case 'g': {
                    command_g();
                    break;
                }
                case 'q': {
                    quit = true;
                    break;
                }
                case '#': {
                    getline(std::cin, command);
                    break;
                }
                default: {
                    getline(std::cin, command);
                    throw commandError();
                    break;
                }
            }
        }
        catch (commandError &c_error) {
            std::cerr << "Invalid Command line Input" << '\n';
        }
    }
}




/***************************************************************************
 *                      BELOW ARE COMMAND FUNCTIONS                        *
 ***************************************************************************/


// Modiefier: cin
// Effect   : read in a string in format: xx:xx:xx:xx:xx|xx:xx:xx:xx:xx (x ints)
//            find lower bound of first timestamp and upper bound of second
//            push the elements in between into prev_ind vector and return size
//            Error checking: check if timestamps have 14 chars
//                            change searched to true if valid
void Logm::command_t() {
    std::string time_str;
    std::string time_str_e;
    getline(std::cin, time_str, '|');
    getline(std::cin, time_str_e);
    if (time_str.length() != 15) throw commandError();
    if (time_str_e.length() != 14) throw commandError();
    searched = true;
    cated = false;
    
    unsigned long long num = timestamp_t(time_str);
    unsigned long long num_e = timestamp(time_str_e);
    
    first = std::lower_bound(main_log.begin(), main_log.end(), num, timestamp_comp());
    end = std::upper_bound(main_log.begin(), main_log.end(), num_e, timestamp_comp());
    timed = true;
    
    std::cout << "Timestamps search: " << end - first << " entries found" << '\n';
}




// Modiefier: cin
// Effect   : read in a string in format: xx:xx:xx:xx:xx (x is an int)
//            find the upper and lower bound of the specific timestamp
//            the items in between are what we want
//            push them into prev_index vector
void Logm::command_m() {
    searched = true;
    timed = true;
    cated = false;
    std::string time_str;
    getline(std::cin, time_str);
    unsigned long long time = timestamp_t(time_str);
    first = std::lower_bound(main_log.begin(), main_log.end(), time, timestamp_comp());
    end = std::upper_bound(main_log.begin(), main_log.end(), time, timestamp_comp());
    
    std::cout << "Timestamp search: " << end - first << " entries found" << '\n';
}



// Modiefier: cin
// Effect   : read in a category and turn it to lower case
//            record the category to cat string
//            change mod to be previous mode
void Logm::command_c() {
    searched = true;
    timed = false;
    cated = true;
    getline(std::cin, cat);
    cat.erase(0,1);
    to_lower_case(cat);
    catg = c_map.find(cat);
    if (catg == c_map.end()) std::cout << "Category search: 0 entries found" << '\n';
    else std::cout << "Category search: " << catg->second.size() << " entries found" << '\n';
}



// Modiefier: cin
// Effect   : read in a message and separate into substrings
//            read their maps and get intersection of the map vectors
//            record the answer to our prev_index vector
//            loop through one by one to find intersection
void Logm::command_k() {
    searched = true;
    timed = false;
    cated = false;
    prev_ind.clear();
    std::string key_mes;
    std::string sub;
    getline(std::cin, key_mes);
    size_t begin = 0, end = 0;
    
    // first vector
    while (!isalnum(key_mes[begin])) {
        begin ++;
        end ++;
    }
    while (isalnum(key_mes[end])) {
        key_mes[end] = char(::tolower(key_mes[end]));
        end ++;
    }
    sub = key_mes.substr(begin, end - begin);
    auto it = k_map.find(sub);
    if (it == k_map.end()) {
        std::cout << "Keyword search: 0 entries found" << '\n';
        return;
    }
    prev_ind.resize(it->second.size());
    copy(it->second.begin(), it->second.end(), prev_ind.begin());
    while (!isalnum(key_mes[begin])) {
        begin ++;
        end ++;
    }
    
    while (end < key_mes.length()) {
        while (isalnum(key_mes[end])) {
            key_mes[end] = char(::tolower(key_mes[end]));
            end ++;
        }
        sub = key_mes.substr(begin, end - begin);
        it = k_map.find(sub);
        if (it == k_map.end()) {
            std::cout << "Keyword search: 0 entries found" << '\n';
            prev_ind.clear();
            return;
        }
        auto itt = std::set_intersection(prev_ind.begin(), prev_ind.end(), it->second.begin(),
                                        it->second.end(), prev_ind.begin());
        prev_ind.resize(itt - prev_ind.begin());
        begin = end;
        while (!isalnum(key_mes[begin])) {
            begin ++;
            end ++;
        }
    }
    std::cout << "Keyword search: " << prev_ind.size() << " entries found" << '\n';
}



// Modiefier: cin
// Effect   : read in an interger indicating index
//            append the index in SORT containter to excerpt list
//            use the time_order to find the SORT index
//            ERROR CHECKING: entry is not valid
void Logm::command_a() {
    int entry = 0;
    std::cin >> entry;
    if (entry < 0 || entry >= int(main_log.size())) throw commandError();
    excerpt.push_back(time_order[entry]);
    std::cout << "log entry " << entry << " appended" << '\n';
}



// Effect   : check which operation has already done
//            append the previous done index to excerpt list
//            ERROR CHECKING: no previous operation done previously
void Logm::command_r() {
    size_t prev = excerpt.size();
    if (!searched) throw commandError();
    else if (timed) {
        auto it = first;
        int index = int(first - main_log.begin());
        while(it != end) {
            excerpt.push_back(index++);
            it ++;
        }
    }
    else if (cated) {
        if (catg != c_map.end()) for (int &ind: catg->second) excerpt.push_back(ind);
    }
    else for (int &ind: prev_ind) excerpt.push_back(ind);
    std::cout << excerpt.size() - prev << " log entries appended" << '\n';
}



// Modiefier: cin
// Effect   : read in an interger indicating index
//            delete the index in excerpt list
//            ERROR CHECKING: entry is not valid
void Logm::command_d() {
    int entry = 0;
    std::cin >> entry;
    if (entry < 0 || entry >= int(excerpt.size())) throw commandError();
    excerpt.erase(excerpt.begin() + entry);
    std::cout << "Deleted excerpt list entry " << entry << '\n';
}


// Modiefier: cin
// Effect   : read in an interger indicating index
//            put the index into the beginning of excerpt list
//            ERROR CHECKING: entry is not valid
void Logm::command_b() {
    int entry = 0;
    std::cin >> entry;
    if (entry < 0 || entry >= int(excerpt.size())) throw commandError();
    int index = excerpt[entry];
    excerpt.erase(excerpt.begin() + entry);
    excerpt.push_front(index);
    std::cout << "Moved excerpt list entry " << entry << '\n';
}


// Modiefier: cin
// Effect   : read in an interger indicating index
//            put the index into the back of excerpt list
//            ERROR CHECKING: entry is not valid
void Logm::command_e() {
    int entry = 0;
    std::cin >> entry;
    if (entry < 0 || entry >= int(excerpt.size())) throw commandError();
    excerpt.push_back(excerpt[entry]);
    excerpt.erase(excerpt.begin() + entry);
    std::cout << "Moved excerpt list entry " << entry << '\n';
}



// Modiefier: cout
// Effect   : sort the vector if needed
//            Print the necessary long message required
void Logm::command_s() {
    if (excerpt.empty()) std::cout << "excerpt list sorted" << '\n'
                                   << "(previously empty)"  << '\n';
    else {
        output_p = &main_log[excerpt.front()];
        std::cout << "excerpt list sorted" << '\n'
                  << "previous ordering:" << '\n'
                  << "0|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
        output_p = &main_log[excerpt.back()];
        std::cout << "..." << '\n'
                  << excerpt.size() - 1 << "|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
        std::sort(excerpt.begin(), excerpt.end());
        output_p = &main_log[excerpt.front()];
        std::cout << "new ordering:" << '\n'
                  << "0|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
        output_p = &main_log[excerpt.back()];
        std::cout << "..." << '\n'
                  << excerpt.size() - 1 << "|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
        
    }
}


// Modiefier: cout
// Effect   : clear the vector if needed
//            Print the necessary long message required
void Logm::command_l() {
    if (excerpt.empty()) std::cout << "excerpt list cleared" << '\n'
                                   << "(previously empty)"  << '\n';
    else {
        output_p = &main_log[excerpt.front()];
        std::cout << "excerpt list cleared" << '\n'
                  << "previous contents:" << '\n'
                  << "0|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
        output_p = &main_log[excerpt.back()];
        std::cout << "..." << '\n'
                  << excerpt.size() - 1 << "|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
        excerpt.clear();
    }
}


// Modiefier: cout
// Effect   : print the previously search log entries
//            Error checking: no previous search done
void Logm::command_g() {
    if (!searched) throw commandError();
    else if (timed) {
        auto it = first;
        while(it != end) {
            std::cout << it->index << "|" << it->time << "|"
                      << it->cat << "|"   << it->mes << '\n';
            ++ it;
        }
    }
    else if (cated) {
        if (catg != c_map.end()) {
            for (int &ind: catg->second) {
                output_p = &main_log[ind];
                std::cout << output_p->index << "|"
                          << output_p->time << "|"
                          << output_p->cat << "|"
                          << output_p->mes << '\n';
            }
        }
    }
    else for (int &ind: prev_ind) {
        output_p = &main_log[ind];
        std::cout << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
    }
}


// Modiefier: cout
// Effect   : print the excerpt list
void Logm::command_p() {
    int count = 0;
    for(int &ind: excerpt) {
        output_p = &main_log[ind];
        std::cout << count ++ << "|"
                  << output_p->index << "|"
                  << output_p->time << "|"
                  << output_p->cat << "|"
                  << output_p->mes << '\n';
    }
}

/***************************************************************************
 *                       BELOW ARE HELPER FUNCTIONS                        *
 ***************************************************************************/



// Requried: vector "main_log" is already sorted
// Effect  : push the index position in vector "time_order" into
//           key and category map vector
void Logm::store_key() {
    int count = 0;
    for (log& item: main_log) {
        cleanup_c(item.cat, count);
        cleanup_k(item.mes, count);
        count ++;
    }
}


// Required: word is a effective word with some space or chars in between
//           current index is the cuurent index of ORDERED vector
// Effect  : seperater word by space and non-alg-num chars
//           and push it's current index into our k(or c)_map with NO duplicate
//           we do not care about upper and lower case
void Logm::cleanup_k(std::string &word, int cur_ind) {
    size_t begin = 0, end = 0;
    std::string sub;
    while (!isalnum(word[begin])) {
        begin ++;
        end ++;
    }
    while (end < word.length()) {
        while (isalnum(word[end])) end ++;
        sub = word.substr(begin, end - begin);
        for (size_t i = 0; i < sub.size(); i ++) sub[i] = char(::tolower(sub[i]));
        it_read = k_map.find(sub);
        if (it_read == k_map.end()) k_map[sub].push_back(cur_ind);
        else if (it_read->second.back() != cur_ind) it_read->second.push_back(cur_ind);
        begin = end;
        while (end < word.length() && !isalnum(word[begin])) {
            begin ++;
            end ++;
        }
    }
}


// Required: word is a effective word with some space or chars in between
//           current index is the cuurent index of ORDERED vector
// Effect  : seperater word by space and non-alg-num chars
//           and push it's current index into our k(or c)_map with NO duplicate
//           we do not care about upper and lower case
void Logm::cleanup_c(std::string word, int cur_ind) {
    size_t begin = 0, end = 0;
    std::string sub;
    while (!isalnum(word[begin])) {
        begin ++;
        end ++;
    }
    while (end < word.length()) {
        while (isalnum(word[end])) {
            word[end] = char(::tolower(word[end]));
            end ++;
        }
        sub = word.substr(begin, end - begin);
        it_read = k_map.find(sub);
        if (it_read == k_map.end()) k_map[sub].push_back(cur_ind);
        else if (it_read->second.back() != cur_ind) it_read->second.push_back(cur_ind);
        begin = end;
        while (end < word.length() && !isalnum(word[begin])) {
            begin ++;
            end ++;
        }
    }
    c_map[word].push_back(cur_ind);
}


// Required: time is a string in the following format:
//                     xx:xx:xx:xx:xx
//           where x is a integer.
// Effect  : return the integer after eliminating all the collens
unsigned long long Logm::timestamp(std::string &time) {
    return (time[0] - '0') * 1000000000L    + (time[1] - '0') * 100000000
         + (time[3] - '0') * 10000000       + (time[4] - '0') * 1000000
         + (time[6] - '0') * 100000         + (time[7] - '0') * 10000
         + (time[9] - '0') * 1000           + (time[10] - '0')* 100
         + (time[12] - '0') * 10            + (time[13] - '0');
}



// Required: time is a string in the following format:
//                     xx:xx:xx:xx:xx
//           where x is a integer, with ONE more space in front
// Effect  : return the integer after eliminating all the collens
unsigned long long Logm::timestamp_t(std::string &time) {
    return  (time[1] - '0') * 1000000000L+ (time[2] - '0') * 100000000
          + (time[4] - '0') * 10000000   + (time[5] - '0') * 1000000
          + (time[7] - '0') * 100000     + (time[8] - '0') * 10000
          + (time[10] - '0') * 1000      + (time[11] - '0')* 100
          + (time[13] - '0') * 10        + (time[14] - '0');
}


// Effect  : change word to lower case
void Logm::to_lower_case(std::string &word) {
    int i = -1;
    while (++i < int(word.length())) word[i] = char(::tolower(word[i]));
}


// Effect  : print everything we want for debugging
void Logm::debugger() {
    for (log &output: main_log) {
        std::cout << output.time << "|" << output.int_time << "|"
                  << output.cat << "|" << output.index << "|" << output.mes << '\n';
    }
    for (int output: k_map["p14"]) {
        std::cout << output << ';';
    }
    std::cout << '\n';
    for (int output: c_map["thread"]) {
        std::cout << output << ';';
    }
    std::cout << '\n';
    for (int output: time_order) {
        std::cout << output << '\n';
    }
}



