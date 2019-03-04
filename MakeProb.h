/* 
 * File:   MakeProb.h
 * Author: washida
 * http://http://www.ibot.co.jp/wpibot/
 *
 * Created on 2019/02/27
 */

#ifndef MAKEPROB_H
#define MAKEPROB_H
#include <string>
#include<vector>

using namespace std;

class MakeProb {
public:
    MakeProb();
    void makeProbability(int datanum);
    void execTest();
    void make(int datanum);
    void getTrainingData(int number);
    void getTestData();
    int bayes(int x[]);
    int bayes_withlog(int x[]);
    void saveProbabilities();
    void readProbabilities();
    virtual ~MakeProb();

private:
    string trainingData;
    string testData;
    unsigned char* trainingdata;
    unsigned char* traininglabel;
    unsigned char* testdata;
    unsigned char* testlabel;
    double* probability;
    double* probtotal;
};

#endif /* MAKEPROB_H */

