#include "kNN.hpp"

void tc1(){
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    dataset.printHead();
    //dataset.columns();
    dataset.printTail();
    int nRows, nCols;
    dataset.getShape(nRows, nCols);
    cout << "Shape: " << nRows << "x" << nCols << endl;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    //(dataset.extract(0, -1, 1, -1)).printHead(); cout << "chay dc";
    //int a = 0, b = 0;
    //cout<<(dataset.extract(0, -1, 1, -1)).getCol_name()->length();
    //(dataset.extract(0, -1, 1, -1)).getShape(a, b);
    //cout << "xxx" << a << " " << b << "xxx";

    Dataset feature = dataset.extract(0, -1, 1, -1);
    //cout<<feature.getCol_name()->length();
    //feature.getShape(a,b);
    //feature.printHead(); cout << "hello";
    Dataset label = dataset.extract(0, -1, 0, 0);   // cout << "hello";
    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
  /*  X_train.printHead();
    X_test.printHead();
    y_train.printHead();
    y_test.printHead();*/
    knn.fit(X_train, y_train); 
    Dataset y_pred = knn.predict(X_test);  
    double accuracy = knn.score(y_test, y_pred);
    cout << "Accuracy: " << accuracy << endl;
  
}
void tc1014()
{
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    List<int>* row = dataset.getData()->get(0);

    row->push_front(0);
    row->push_front(2);

    cout << row->length() << endl;
    row->print();
}
void tc1147()
{
    int nRows, nCols;
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    Dataset dataset2 = Dataset(dataset);
    //dataset2.printHead(10, 10); cout << "hell";
    bool result = dataset2.drop(1, 0, "label");
    cout << "Drop(1, \"label\") result: " << result << endl;
}
void tc1149()
{
    int nRows, nCols;
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    Dataset dataset2 = dataset;
    bool result = dataset2.drop(1, 0, "28x28");
    cout << "Drop(1, \"28x28\") result: " << result << endl;
}
void tc1158() {
    int nRows, nCols;
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    Dataset dataset_extract = dataset.extract(0, -2, 0, -1); //cout << "hell";
    Dataset dataset_extract2 = Dataset(dataset_extract);
    dataset_extract2.getShape(nRows, nCols);
    cout << "Dataset extract shape: " << nRows << "x" << nCols << endl;
}
void tc_knn_score(int k, int size_X) {
    int nRows, nCols;
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");

    kNN knn = kNN(k);
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, size_X, 1, -1);
    Dataset label = dataset.extract(0, size_X, 0, 0);

    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);
    knn.fit(X_train, y_train);X_test.printTail();
    Dataset y_pred = knn.predict(X_test);
    double accuracy = knn.score(y_test, y_pred);
    cout << "Accuracy: " << accuracy << endl;
}
int main() {
   // tc1();
   // tc1014(); cout << "a";
   // tc1147();
   // tc1158();
    tc_knn_score(5, 10);
    return 0;
}