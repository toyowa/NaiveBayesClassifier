/* 
 * Test MNIST using Bayes Theorem
 * File:   MakeProb.cpp
 * Author: Toyoaki Washida
 * http://http://www.ibot.co.jp/wpibot/
 *
 * Created on 2019/02/27
 */
#include "MakeProb.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

#define NEARLY_ZERO 1.0e-200

MakeProb::MakeProb() {
    trainingData = "./data/trainingData.txt";
    testData = "./data/testData.txt";
    trainingdata = (unsigned char*)malloc(60000*784);
    traininglabel = (unsigned char*)malloc(60000);
    testdata = (unsigned char*)malloc(10000*784);
    testlabel = (unsigned char*)malloc(10000);
    probability = (double*)malloc(784*10*sizeof(double));
    probtotal = (double*)malloc(10*sizeof(double));
}

void MakeProb::makeProbability(int datanum){
    getTrainingData(datanum);
    unsigned char val;
    // ラベル（各数字）の出現総数を数える
    for(int i=0;i<10;i++){
        probtotal[i] = 0.0;
    }
    memset(probtotal, 0, 10*sizeof(double));
    memset(probability, 0, 784*10*sizeof(double));
    //
    for(int d=0;d<datanum;d++){
        unsigned int label = (unsigned int)traininglabel[d];
        probtotal[label] += 1;
        for(int n=0;n<784;n++){
            val = trainingdata[d*784+n];
            //cout << "traininglabel[d] = " << (unsigned int)traininglabel[d] << " setLabel = " << label << " val = " << (unsigned int)val << endl; 
            if((unsigned int)val != 0){
                // ラベルがlabelのときに、第nピクセルが1である総数を数えている
                probability[10*n+label] += 1;
            }
        }
        //if(d == 10) return;
    }
    //cout << "各ピクセルについて、ラベルごとの1になる確率" << endl;
    for(int n=0;n<784;n++){
        //cout << "Pixel: " << n << " -> ";
        for(int i=0;i<10;i++){
            probability[n*10+i] /= probtotal[i];
            //cout << probability[n*10+i] << " ";
        }
        //cout << endl;
    }
    // ピクセルにかかわらず、全体の中でのラベルの出現確率を計算する
    double tp = 0.0;
    for(int i=0;i<10;i++){
        probtotal[i] /= datanum;
        //cout << i << ": " << probtotal[i] << " ";
        //tp += probtotal[i];
    }
    //cout << "Total = " << tp << endl;
}

void MakeProb::execTest(){
    getTestData();
    int x[784];
    int totalcorrect = 0;
    int numericalerror = 0;
    for(int d=0;d<10000;d++){
    //for(int d=0;d<10;d++){  // used for check
        for(int n=0;n<784;n++){
            x[n] = testdata[d*784+n];
            if(x[n] > 0) x[n] = 1.0;
        }
        int estimated = bayes(x);
        // 対数和に変換する場合
        if(estimated < 0){
            // Error: bayes calculation!!
            numericalerror++;
            continue;
        }
        int correct;
        if(estimated == (int)testlabel[d]){
            correct = 1;
            totalcorrect++;
        }else{
            correct = 0;
        }
        cout << "Actual label = " << (int)testlabel[d] << " Estimated label = " << estimated << endl;
    }
    cout << "total correct = " << totalcorrect << " ratio = " << (double)totalcorrect/10000 
    << " Numerical Calculation Error = " << numericalerror
    << endl;
}

void MakeProb::make(int datanum){
    makeProbability(datanum);
    saveProbabilities();
    //readProbabilities();
    //execTest();
}

int MakeProb::bayes(int x[]){
    long double prob[10];
    for(int j=0;j<10;j++){
        prob[j] = 1;
    }
    // 数字jのもとで、ピクセルベクトルｘが出現する確率の計算　P(x|j)
    for(int j=0;j<10;j++){
        for(int i=0;i<784;i++){
            prob[j] = prob[j]*pow(probability[i*10+j],x[i])*pow(1-probability[i*10+j],1-x[i]);
            //cout << "probability[" << i << "*10+" << j << "] = " << probability[i*10+j] << " x[" << i << "] = " << x[i] << endl;
        }
        //cout << "P(x|" << j << ") = " << prob[j] << endl;
    }
    
    long double sum = 0;
    for(int i=0;i<10;i++){
        // P(x|i)P(i)のiに関する合計
        sum += prob[i]*probtotal[i];
    }
    
    // sum of P(x|i)P(i) for number i
    //cout << "P(x|i)P(i)のiに関する合計 = " << sum << endl;
    if(sum < NEARLY_ZERO){
        cout << "ERROR: zero for sum of probability!!" << endl;
        return -1;
    }
    
    cout << "Bayes estimation" << endl; 
    for(int i=0;i<10;i++){
        prob[i] = prob[i]*probtotal[i]/sum;
        cout << i << ": " << prob[i] << " ";
    }
    cout << endl;
    int maxlabel = -1;
    double maxprob = -1.0;
    for(int j=0;j<10;j++){
        if(prob[j] > maxprob){
            maxlabel = j;
            maxprob = prob[j];
        }
    }
    cout << "Number with maximum probability = " << maxlabel << " P(x|i)P(i) = " << maxprob << endl;     
    return maxlabel;
}

// 積を使わずに、自然対数で和に変換した場合
int MakeProb::bayes_withlog(int x[]){
    long double prob[10];
    for(int j=0;j<10;j++){
        prob[j] = 1;
    }
    // 数字jのもとで、ピクセルベクトルｘが出現する確率の計算　P(x|j)
    for(int j=0;j<10;j++){
        for(int i=0;i<784;i++){
            prob[j] += (log(pow(probability[i*10+j],x[i]))+log(pow(1-probability[i*10+j],1-x[i])));
            //cout << "probability[" << i << "*10+" << j << "] = " << probability[i*10+j] << " x[" << i << "] = " << x[i] << endl;
        }
        prob[j] += log(probtotal[j]);
        //cout << "P(x|" << j << ") = " << prob[j] << endl;
    }
    cout << "Bayes estimation ln(P(x|i)P(i))" << endl; 
    for(int i=0;i<10;i++){
        cout << i << ": " << prob[i] << " ";
    }
    cout << endl;
    
    int maxlabel = -1;
    double maxprob = -1.0e100;
    for(int j=0;j<10;j++){
        if(prob[j] > maxprob){
            maxlabel = j;
            maxprob = prob[j];
        }
    }
    cout << "Number with maximum probability = " << maxlabel << endl;     
    return maxlabel;
}

/*
 * 確率データを書き込みます バイナリデータ
 * データファイルサイズは62800バイトになる
 */
void MakeProb::saveProbabilities(){
    ofstream fout;
    fout.open("./data/probabilities.bin", ios::out|ios::binary|ios::trunc);
    if (!fout) {
        cout << "cannot open ./data/probabilities.bin" << endl;
        return;
    }
    for(int i=0;i<784*10;i++){
        fout.write((char*)&probability[i],sizeof(double));
    }
    for(int i=0;i<10;i++){
        fout.write((char*)&probtotal[i],sizeof(double));
    }
    fout.close();    
}

/*
 * 確率データを読み込みます バイナリデータ
 */
void MakeProb::readProbabilities(){
    ifstream fin("./data/probabilities.bin", ios::in | ios::binary );
    if (!fin){
        cout << "cannot open ./data/probabilities.bin" << endl;
        return;
    }
    double d;
    long count = 0;
    for(int i=0;i<784*10;i++){
        if(fin.eof()){
            cout << "予定のデータを取り込めません file = " << "./data/probabilities.bin" << endl;
            return;
        }
        fin.read((char*)&d,sizeof(double));
        probability[count] = d; 
        count++;
    }
    cout << "ピクセル単位にラベルの出現確率P(x|As)を読み込みました = " << count << endl;
    count = 0;
    for(int i=0;i<10;i++){
        if(fin.eof()){
            cout << "予定のデータを取り込めません file = " << "./data/probabilities.bin" << endl;
            return;
        }
        fin.read((char*)&d,sizeof(double));
        probtotal[count] = d; 
        count++;
    }
    cout << "ラベルの周辺的な出現確率(P(As))を読み込みました = " << count << endl;
    fin.close();
}

/*
 * Read MNIST data
 */
 
 /*
 training data
 */
void MakeProb::getTrainingData(int number){
    cout << "Read training data" << endl;
    ifstream mniststream;
    mniststream.open(trainingData.c_str()); 
    string line;
    string label;
    int count = 0;
    int j;
    string val;
    while(!mniststream.eof()){
        getline(mniststream, line);
        stringstream ss(line);
        string term;
        ss >> term;
        if (term.compare("out:") == 0) {
            ss >> label;
            traininglabel[count] = (unsigned char)stoi(label);
            //cout << "traininglabel[count] = " << (unsigned int)traininglabel[count] << endl;
            count++;
            if(count > number) break;
        }else if(term.compare("in:") == 0){
            j = 0;
            while (!ss.eof()) {
                ss >> val;
                trainingdata[count*784+j] = (unsigned char)stoi(val);
                j++;
            }
        }else{
            cout << "found bad data, cout = " << count << " line = " << line << endl;
        }
        //if(count == 10) return;
    }
    cout << "Total data number for learning = " << count << endl;
    mniststream.close();
}

 /*
 Test data
 */
void MakeProb::getTestData(){
    cout << "Read test data" << endl;
    ifstream mniststream;
    mniststream.open(testData.c_str()); 
    string line;
    string label;
    int count = 0;
    int j;
    string val;
    //int lineno = 0;
    while(!mniststream.eof()){
        getline(mniststream, line);
        //if(lineno == 10) break; // テスト用
        //lineno++;
        stringstream ss(line);
        string term;
        ss >> term;
        if (term.compare("out:") == 0) {
            ss >> label;
            testlabel[count] = (unsigned char)stoi(label);
            //cout << "testlabel[count] = " << (unsigned int)testlabel[count] << endl;
            count++;
        }else if(term.compare("in:") == 0){
            j = 0;
            while (!ss.eof()) {
                ss >> val;
                testdata[count*784+j] = (unsigned char)stoi(val);
                j++;
            }
        }else{
            cout << "Bad data found, cout = " << count << " line = " << line << endl;
        }
        //if(count == 10) return;
    }
    mniststream.close();
    cout << "Total test data = " << count << endl;
}


MakeProb::~MakeProb() {
    free(trainingdata);
    free(traininglabel);
    free(testdata);
    free(testlabel);
    free(probability);
    free(probtotal);
}


