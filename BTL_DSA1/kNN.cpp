#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
template<typename T>
List_data<T>::Node::Node(T value, Node* next):value(value), next(next) {}
template<typename T>
List_data<T>::List_data() {
    head = tail = nullptr;
    count = 0;
}
template<typename T>
List_data<T>::~List_data() {
    clear();
}
template<typename T>
void List_data<T>::push_back(T value) {
    Node* tmp = new Node(value, nullptr);
    if (count == 0) {
        head = tail = tmp;
    }
    else {
        tail->next = tmp;
        tail = tmp;
    }
    count++;
}
template<typename T>
void List_data<T>::push_front(T value) {
    Node* tmp = new Node(value, nullptr);
    if (count == 0) {
        head = tail = tmp;
    }
    else {
        tmp->next = head;
        head = tmp;
    }
    count++;
}
template<typename T>
void List_data<T>::insert(int index, T value) {
    if (index < 0 || index > count) {
        return;
    }
    Node* newNode = new Node(value, nullptr);
    if (head == nullptr) {
        head = tail = newNode; 
    }
    else if (index == 0) {
        newNode->next = head;
        head = newNode;
    }
    else if (index == count) {
        push_back(value);
        return;
    }
    else {
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    count++;
}
template<typename T>
void List_data<T>::remove(int index) {
    if (index < 0 || index >= count)
        return;
    Node* tmp = head;
    if (index == 0) {
        head = head->next;
    }
    else if (index == count - 1) {
        for (int i = 0; tmp->next != tail; i++) {
            tmp = tmp->next;
        }
        tail = tmp;
        tmp = tmp->next;
        tail->next = nullptr;
    }
    else {
        Node* cur = head;
        for (int i = 0; i < index - 1; i++) {
            cur = cur->next;
        }
        tmp = cur->next;
        cur->next = tmp->next;
    }
    delete tmp;
    count--;
}
template<typename T>
T& List_data<T>::get(int index) const {
    if (index < 0 || index >= count) {
        //cout << count<<"xxx";
        throw std::out_of_range("get(): Out of range");
    }
    Node* tmp = head;
    for (int i = 0; i < index; i++) {
        tmp = tmp->next;
    }
    return tmp->value;

}
template<typename T>
int List_data<T>::length() const {
    return count;
}
template<typename T>
void List_data<T>::clear() {
    Node* tmp = head;
    while (head != nullptr) {
        head = head->next;
        delete tmp;
        tmp = head;
    }
    count = 0;
    tail = nullptr;
}
template<typename T>
void List_data<T>::print() const {
    Node* tmp = head;
    for (int i = 0; i < count; i++) {
        cout << tmp->value;
        tmp = tmp->next;
        if (i != count - 1)
            cout << " ";
    }
}
template<typename T>
void List_data<T>::reverse() {
    tail = head;
    Node* prev = nullptr;
    Node* cur = head;
    Node* next = nullptr;
    while (cur != nullptr) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    head=prev;
}
Dataset::Dataset() {
    Row = Col = 0; 
    col_name = new List_data<string>();
    data = new List_data<List<int>*>();
}
Dataset::~Dataset() {
    /*if (data != nullptr)
        delete data;
    if (col_name != nullptr)
        delete col_name;
    data = nullptr;
    col_name = nullptr;*/
}
Dataset::Dataset(const Dataset& other) {//dôdodododododododdodoododododododododdodododododododo
    other.getShape(Row, Col); 
    col_name = new List_data<string>();
    data = new List_data<List<int>*>();
    for (int i = 0; i < Row; i++) {
        data->push_back(new List_data<int>());
        for (int j = 0; j < Col; j++) {
            data->get(i)->push_back(other.data->get(i)->get(j));
        }
    }
    for (int i = 0; i < Col; i++) {
        col_name->push_back(other.getCol_name()->get(i));
    }
}
Dataset& Dataset::operator=(const Dataset& other) {///dôdododododododododoododod
    /*other.getShape(Row, Col);
    col_name = new List_data<string>();
    data = new List_data<List<int>*>();
    for (int i = 0; i < Row; i++) {
        col_name->push_back(other.getCol_name()->get(i));
        data->push_back(new List_data<int>());
        for (int j = 0; j < Col; j++) {
            data->get(i)->push_back(other.data->get(i)->get(j));
        }
    }*/
   // return *this;
    data = other.getData();
    col_name = other.getCol_name();
    other.getShape(Row, Col); //cout << col_name->length() << "x" << Col;
    return *this;
}
bool Dataset::loadFromCSV(const char* fileName) {
    ifstream file;
    file.open(fileName);
    if (!file.is_open()) {
        return false;
    }
    string s;
    int n; 
    for (int i = 1; i <= 784; i++) {
        getline(file, s, ','); 
        col_name->push_back(s);
    }
    getline(file, s);
    col_name->push_back(s); //785 cột kể cả cột nhãn
    for (int i = 1; i <= 199; i++) {//199 dòng trừ dòng lable
        data->push_back(new List_data<int>());
        for (int j = 1; j <= 784; j++) {
            getline(file, s, ',');
            n = stoi(s); 
            data->get(i-1)->push_back(n);
        }
        getline(file, s);
        n = stoi(s);
        data->get(i - 1)->push_back(n);
    }
    file.close();
    Row = data->length();
    Col = data->get(0)->length();
    return true;
}
void Dataset::printHead(int nRows, int nCols) const {
    if (nRows <= 0 || nCols <= 0)
        return;
    int row = 0, col = 0;
    getShape(row, col);
    if (nRows < row) {
        row = nRows;
    }
    if (nCols < col) {
        col = nCols;
    }
    for (int i = 1; i <= col; i++) {
        cout << col_name->get(i - 1); 
        if (i != col) cout << " ";
    }
    cout << endl;
    for (int i = 1; i <= row; i++) {
        for (int j = 1; j <= col; j++) {
            cout << data->get(i - 1)->get(j - 1);
            if (j != col)
                cout << " ";
        }
        if(i!=row) cout << endl;
    }
}
void Dataset::printTail(int nRows, int nCols) const {
    if (nRows <= 0 || nCols <= 0)
        return;
    int row = 0, col = 0;
    getShape(row, col);
    if (nRows < row) {
        row = nRows;
    }
    if (nCols < col) {
        col = nCols;
    }
    int Row, Col;
    getShape(Row, Col);
    for (int i = Col - col + 1; i <= Col; i++) {
        cout << col_name->get(i - 1);
        if (i != Col) cout << " ";
    }
    cout << endl;
    for (int i = Row-row+1; i <= Row; i++) {
        for (int j = Col-col+1; j <= Col; j++) {
            cout << data->get(i - 1)->get(j - 1);
            if (j != Col)
                cout << " ";
        }
        if(i!=Row) cout << endl;
    }
}
void Dataset::getShape(int& nRows, int& nCols) const {
    nRows = Row;
    nCols = Col;
}
void Dataset::columns() const {
    col_name->print();
}
bool Dataset::drop(int axis, int index, std::string columns) {
    if (axis == 0) {
        if (index < 0 || index >= Row)
            return false;
        data->remove(index);
        Row--;
        return true;
    }
    else if (axis == 1) {
        int index_col = -1;
        string str_cmp;
        for (int i = 0; i < Col; i++) {
            str_cmp = col_name->get(i); 
            if (str_cmp.compare(columns)==0) {
                index_col = i; 
                break;
            }
        }
        if (index_col == -1) return false;
        for (int i = 0; i < Row; i++) {
            data->get(i)->remove(index_col);
        }
        return true;
    }
    return false;
}
void Dataset::setShape() {
    Row = data->length();
    Col = data->get(0)->length();
}
Dataset Dataset::extract(int startRow , int endRow, int startCol, int endCol) const {
    Dataset dataset_new = Dataset();
    List<List<int>*>* tmp1 = dataset_new.getData();
    List<string>* tmp2 = dataset_new.getCol_name();
    if (endRow == -1) endRow = Row - 1; //cout << "xxx" << Row << "XXXX";
    if (endCol == -1) endCol = Col - 1;
    for (int i = startRow; i <= endRow; i++) {
        tmp1->push_back(new List_data<int>());
        for (int j = startCol; j <= endCol; j++) {
            tmp1->get(i - startRow)->push_back(data->get(i)->get(j));//xem xest anhr huongwr cuar T&
        }
    }//cout << startCol << "x" << endCol << " ";
    for (int j = startCol; j <= endCol; j++) {
        tmp2->push_back(getCol_name()->get(j));//xem xest anhr huongwr cuar T&
    }
    dataset_new.setShape(); //dataset_new.printHead(10, 10);
    //cout << "XX"<<dataset_new->getData()->length() << "XX";
   // cout<<dataset_new->getCol_name()->get(20);
    return dataset_new;
   /* Dataset* dataset_new = new Dataset();
    List<List<int>*>* tmp1 = dataset_new->getData();
    List<string>* tmp2 = dataset_new->getCol_name();
    if (endRow == -1) endRow = Row-1; //cout << "xxx" << Row << "XXXX";
    if (endCol == -1) endCol = Col-1; 
    for (int i = startRow; i <= endRow; i++) {
        tmp1->push_back(new List_data<int>());
        for (int j = startCol; j <= endCol; j++) {
            tmp1->get(i - startRow)->push_back(data->get(i)->get(j));//xem xest anhr huongwr cuar T&
        }
    }//cout << startCol << "x" << endCol << " ";
    for (int j = startCol; j <= endCol; j++) {
        tmp2->push_back(getCol_name()->get(j));//xem xest anhr huongwr cuar T&
    }
    dataset_new->setShape(); dataset_new->printHead(10,10); 
    //cout << "XX"<<dataset_new->getData()->length() << "XX";
   // cout<<dataset_new->getCol_name()->get(20);
    return *dataset_new;*/
}
List<List<int>*>* Dataset::getData() const {
    return data;
}
List<string>* Dataset::getCol_name() const {
    return col_name;
}
kNN::kNN(int k) {
    this->k = k;
    
}
void kNN::fit(const Dataset& X_train, const Dataset& y_train) {
    this->X_train = X_train;
    this->y_train = y_train;// this->X_train.printHead();
}
Dataset kNN::predict(const Dataset& X_test) {
    Dataset result = Dataset();
    List<List<int>*>* tmp1 = result.getData();
    List<string>* tmp2 = result.getCol_name();
    tmp2->push_back("label");
    int Row = 0, Col = 0;
    X_test.getShape(Row, Col);
    int** matrix_diff = new int*[Row];
    int size_train = 0, tmp = 0;
    y_train.getShape(size_train, tmp);
    for (int i = 0; i < Row; i++) {
        matrix_diff[i] = new int[size_train];//với mỗi hàng là một mảng lưu khoảng cách của ảnh đó với mỗi ảnh trong X_train
    }
    List<List<int>*>* data_test = X_test.getData();
    List<List<int>*>* data_train = X_train.getData(); 
    //cout << "row so anh test" << Row << endl << "so anh train" << size_train << endl;
    for (int i = 0; i < Row; i++) {
        List<int>* picture_test = data_test->get(i);
        for (int j = 0; j < size_train; j++) {
            int Sdiff = 0;//khoảng cách giữa hai ảnh bình phương
            List<int>* picture_train = data_train->get(j);
            for (int h = 0; h < 784; h++) {
                Sdiff = Sdiff + pow(picture_test->get(h) - picture_train->get(h), 2);
            }
            matrix_diff[i][j] = Sdiff;
        }
    }//cout << "hellol";
    int* arr_y_train = new int[size_train];
    for (int i = 0; i < size_train; i++) {
        arr_y_train[i] = y_train.getData()->get(i)->get(0);
    }
    int** matrix_predict = new int* [Row];
    for (int i = 0; i < Row; i++) {
        matrix_predict[i] = new int[10];
    }
    for (int i = 0; i < Row; i++) {
        for (int j = 0; j < 10; j++) {
            matrix_predict[i][j] = 0;
        }
    }
    int* arr_y_train_tmp = new int[size_train]; //cout << matrix_diff[1][4];
    for (int i = 0; i < Row; i++) {
        for (int l = 0; l < size_train; l++) {
            arr_y_train_tmp[l] = arr_y_train[l];
        }//cout << matrix_diff[1][4]<<" ";
        for (int h = 0; h < k; h++) {
          //  cout << i << "x" << h << "x" << matrix_diff[1][4] << " ";
            int min = matrix_diff[i][h]; //cout << i << "x" << h << "x" << min << " ";
            int min_index = h;
            for (int j = h; j < size_train; j++) {
                if (matrix_diff[i][j] < min) {
                    min = matrix_diff[i][j];
                    min_index = j;
                }
            }
            matrix_diff[i][min_index] = matrix_diff[i][h];
            matrix_diff[i][h] = min;
            int tmp = arr_y_train_tmp[min_index];
            arr_y_train_tmp[min_index] = arr_y_train_tmp[h];
            arr_y_train_tmp[h] = tmp; 
            matrix_predict[i][arr_y_train_tmp[h]]= matrix_predict[i][arr_y_train_tmp[h]]+1;//++ hay + 1
            /*int tmp = arr_y_train[min_index];
            arr_y_train[min_index] = arr_y_train[h];
            arr_y_train[h] = arr_y_train[min_index];*/
        }
        for (int i = 0; i < 5; i++) {
            cout << arr_y_train_tmp[i] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < Row; i++) {
        int max = matrix_predict[i][0];
        int index_max = 0;
        for (int j = 1; j < 10; j++) {
            if (matrix_predict[i][j] > max) {
                max = matrix_predict[i][j];
                index_max = j;
            }
        }
        tmp1->push_back(new List_data<int>());
        tmp1->get(i)->push_back(index_max);
    }
    delete[] arr_y_train; arr_y_train = nullptr;
    delete[] arr_y_train_tmp; arr_y_train_tmp = nullptr;
    for (int i = 0; i < Row; i++) {
        delete[] matrix_predict[i];
    }
    delete[] matrix_predict; matrix_predict = nullptr; 
    for (int i = 0; i < Row; i++) {
        delete[] matrix_diff[i];//với mỗi hàng là một mảng lưu khoảng cách của ảnh đó với mỗi ảnh trong X_train
    }
    delete[] matrix_diff; matrix_diff = nullptr; 
    result.setShape(); 
    return result;
}
double kNN::score(const Dataset& y_test, const Dataset& y_pred) {
    List<List<int>*>* test = y_test.getData();
    List<List<int>*>* pred = y_pred.getData();
    int t = 0, f = 0;// cout << test->length();
    for (int i = 0; i < test->length(); i++) {
        if (test->get(i)->get(0) == pred->get(i)->get(0)) {
            t = t + 1;
        }
        else f = f + 1;
    }
   // cout << t<<"xx" << f;
    double result = static_cast<double>(t)/(f+t);
    return result;
}
void train_test_split(Dataset& X, Dataset& y, double test_size,
    Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test) {
    int Row, Col; 
    y.getShape(Row, Col); 
    int nTrain = Row - Row * test_size;
    int nTest = Row-nTrain;
    X_train = X.extract(0, Row - nTest - 1, 0, -1);
    X_test = X.extract(Row - nTest, -1, 0, -1);
    y_train = y.extract(0, Row - nTest - 1, 0, 0);
    y_test = y.extract(Row - nTest, -1, 0, 0);
}