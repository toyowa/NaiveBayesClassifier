# Naive Bayes Classifier

make 
でコンパイルできる。

ただ、現状は、学習だけになっているが

void MakeProb::make(int datanum){  
    makeProbability(datanum);  
    saveProbabilities();  
    //readProbabilities();  
    //execTest();  
}  

の部分を

void MakeProb::make(int datanum){  
    //makeProbability(datanum);  
    //saveProbabilities();  
    readProbabilities();  
    execTest();  
}  

と変更すれば、テストを実行する。

なお、データファイルは、元のMNISTデータをテキストファイルに変形しているが、ここでは大きすぎて載せられないのではと思う。

データの読み取り関数を見れば、フォーマットがわかると思う。
