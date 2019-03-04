/*
 * C++ Version
 * Bayes estimation for MNIST
 * 2019/2/27
 * Toyoaki WASHIDA
 * http://http://www.ibot.co.jp/wpibot/
 */


#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "MakeProb.h"

using namespace std;

/*
 * main funtion
 */
int main(int argc, char** argv) {
    int totaldata;
    if(argc == 1){
        totaldata = 60000;
    }else{
        totaldata = atoi(argv[1]);
    }
    MakeProb* mkp = new MakeProb();
    mkp->make(totaldata);
    free(mkp);
}
