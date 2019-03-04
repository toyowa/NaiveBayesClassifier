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

データの読み取り関数を見れば、フォーマットがわかると思う。MNISTのデータをこのプログラムように変形するのは、難しくない。

また、ベイズ推計をそのままやると、小さい数の大きな積になってアンダーフローを起こす心配がある。プログラム中には、自然対数で和のかたちにしてそれを回避する計算関数も入れてある。ただ、そちらを使っても計算結果は変わらず、ただ、自然対数を撮る時間が余計にかかるだけだった。この規模の計算ではどちらでも良いと思う。ただ、積にかたちの場合は、ほぼゼロに近い値で比較していることは間違いない。倍精度の浮動小数点でやっているので、なんとか持ちこたえている。

http://www.ibot.co.jp/wpibot/
