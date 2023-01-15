//  EECS 281 Project 3 Log Manager
//
//  jamiean; JunCheng An
//
//  Created by JunCheng An on 5/26/18.
//  Copyright © 2018 JunCheng An. All rights reserved.

#include "logm.h"


int main(int argc, char* argv[]) {
    
    std::ios_base::sync_with_stdio(false);

    /*
    // for running w/ xcode
    #ifdef __APPLE__
    if(!freopen("sample-t-cmds.txt", "r", stdin)) {
        std::cerr << "THERE IS NO INPUT!" << std::endl;
        exit(1);
    }
    //freopen("myoutput.txt", "w", stdout);
    #endif
    */
    
    
    
     
    
    try {
        Logm log;
        
        log.read_in(argv);
        
        log.run();

        //log.debugger();
        
    }
    catch(helperFunction &help) {
        std::cout << "Helper function called !" << '\n';
        exit(0);
    }
    catch(FileNotFound &file) {
        std::cout << "File not found !" << '\n';
        argc++;
        return 0;
    }
    
    

    
}
