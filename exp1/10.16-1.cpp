#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// �����ඨ��
class Complex {
private:
    double real;  // ʵ��
    double imag;  // �鲿

public:
    // ���캯��
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // ��ȡʵ��
    double getReal() const { return real; }

    // ��ȡ�鲿
    double getImag() const { return imag; }

    // ���㸴����ģ
    double getModulus() const {
        return sqrt(real * real + imag * imag);
    }

    // ������������
    bool operator==(const Complex& other) const {
        return (real == other.real && imag == other.imag);
    }

    // ���ز��������
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // ��ʾ����
    void display() const {
        if (imag >= 0)
            cout << real << " + " << imag << "i";
        else
            cout << real << " - " << -imag << "i";
    }
};
//
// ���������������
vector<Complex> generateRandomComplexVector(int size, double minVal, double maxVal) {
    vector<Complex> vec;
    srand(time(0));  // �������������

    for (int i = 0; i < size; ++i) {
        double real = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        double imag = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        vec.push_back(Complex(real, imag));
    }

    return vec;
}

// ��������
void shuffleVector(vector<Complex>& vec) {
    srand(time(0));
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(vec[i], vec[j]);
    }
}

// ����Ԫ�أ�ʵ�����鲿����ͬ��
int findElement(const vector<Complex>& vec, const Complex& target) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return i;  // �ҵ���������
        }
    }
    return -1;  // δ�ҵ�����-1
}

// ����Ԫ��
void insertElement(vector<Complex>& vec, int index, const Complex& elem) {
    if (index >= 0 && index <= vec.size()) {
        vec.insert(vec.begin() + index, elem);
    }
}

// ɾ��Ԫ��
bool deleteElement(vector<Complex>& vec, const Complex& elem) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it == elem) {
            vec.erase(it);
            return true;  // ɾ���ɹ�
        }
    }
    return false;  // δ�ҵ�Ԫ��
}

// ����Ψһ����ȥ���ظ��
void uniqueVector(vector<Complex>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = i + 1; j < vec.size(); ++j) {
            if (vec[i] == vec[j]) {
                vec.erase(vec.begin() + j);
                j--;  // ������������Ϊɾ����һ��Ԫ��
            }
        }
    }
}

// �ȽϺ������Ȱ�ģ�Ƚϣ�ģ��ͬ��ʵ���Ƚ�
bool compareByModulus(const Complex& a, const Complex& b) {
    if (a.getModulus() != b.getModulus()) {
        return a.getModulus() < b.getModulus();
    } else {
        return a.getReal() < b.getReal();
    }
}

// ��������
void bubbleSort(vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (!compareByModulus(vec[j], vec[j + 1])) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// �鲢����������
void merge(vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Complex> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareByModulus(L[i], R[j])) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

// �鲢����
void mergeSort(vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        merge(vec, left, mid, right);
    }
}

// ������ң�����ģ����[m1, m2)������Ԫ��
vector<Complex> rangeSearch(const vector<Complex>& vec, double m1, double m2) {
    vector<Complex> result;
    for (const auto& elem : vec) {
        double mod = elem.getModulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(elem);
        }
    }
    return result;
}

// ��ӡ����
void printVector(const vector<Complex>& vec, const string& title) {
    cout << title << " (" << vec.size() << " 元素):" << endl;
    for (const auto& elem : vec) {
        elem.display();
        cout << " (ģ: " << elem.getModulus() << "), ";
    }
    cout << endl << endl;
}

// ���������㷨Ч��
void testSortingEfficiency() {
    const int size = 10000;  // ������С
    vector<Complex> original = generateRandomComplexVector(size, 0, 100);
    
    // �������ֲ�ͬ״̬������
    vector<Complex> sortedVec = original;
    sort(sortedVec.begin(), sortedVec.end(), compareByModulus);
    
    vector<Complex> reversedVec = sortedVec;
    reverse(reversedVec.begin(), reversedVec.end());
    
    vector<Complex> randomVec = original;

    // ������������
    clock_t start, end;
    double timeUsed;

    // �����������
    vector<Complex> testVec = sortedVec;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "������������: " << timeUsed << " ��" << endl;

    // �����������
    testVec = randomVec;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "������������: " << timeUsed << " ��" << endl;

    // �����������
    testVec = reversedVec;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "������������: " << timeUsed << " ��" << endl << endl;

    // ���Թ鲢����
    // �����������
    testVec = sortedVec;
    start = clock();
    mergeSort(testVec, 0, testVec.size() - 1);
    end = clock();
    timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "�鲢��������: " << timeUsed << " ��" << endl;

    // �����������
    testVec = randomVec;
    start = clock();
    mergeSort(testVec, 0, testVec.size() - 1);
    end = clock();
    timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "�鲢��������: " << timeUsed << " ��" << endl;

    // �����������
    testVec = reversedVec;
    start = clock();
    mergeSort(testVec, 0, testVec.size() - 1);
    end = clock();
    timeUsed = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "�鲢��������: " << timeUsed << " ��" << endl;
}

int main() {
    // ���������������
    vector<Complex> complexVec = generateRandomComplexVector(10, 0, 10);
    printVector(complexVec, "��ʼ�������");

    // �������Ҳ���
    shuffleVector(complexVec);
    printVector(complexVec, "���Һ������");

    // ���Բ��Ҳ���
    if (!complexVec.empty()) {
        Complex target = complexVec[3];
        cout << "����Ԫ��: ";
        target.display();
        cout << endl;
        
        int index = findElement(complexVec, target);
        if (index != -1) {
            cout << "�ҵ�Ԫ�أ�����Ϊ: " << index << endl << endl;
        } else {
            cout << "δ�ҵ�Ԫ��" << endl << endl;
        }
    }

    // ���Բ������
    Complex insertElem(100, 200);
    insertElement(complexVec, 2, insertElem);
    printVector(complexVec, "����Ԫ�غ������");

    // ����ɾ������
    if (!complexVec.empty()) {
        Complex deleteElem = complexVec[5];
        cout << "ɾ��Ԫ��: ";
        deleteElem.display();
        cout << endl;
        
        bool deleted = deleteElement(complexVec, deleteElem);
        if (deleted) {
            printVector(complexVec, "ɾ��Ԫ�غ������");
        } else {
            cout << "ɾ��ʧ�ܣ�δ�ҵ�Ԫ��" << endl << endl;
        }
    }

    // ����Ψһ������
    // ����һЩ�ظ�Ԫ���Ա����
    if (!complexVec.empty()) {
        complexVec.push_back(complexVec[0]);
        complexVec.push_back(complexVec[1]);
        printVector(complexVec, "�����ظ�Ԫ�غ������");
        
        uniqueVector(complexVec);
        printVector(complexVec, "Ψһ���������");
    }

    // �������������ں������ԣ�
    sort(complexVec.begin(), complexVec.end(), compareByModulus);
    printVector(complexVec, "����������");

    // �����������
    double m1 = 2.0, m2 = 8.0;
    vector<Complex> rangeResult = rangeSearch(complexVec, m1, m2);
    cout << "ģ���� [" << m1 << ", " << m2 << ") ��Ԫ��:" << endl;
    printVector(rangeResult, "������ҽ��");

    // ���������㷨Ч��
    cout << "\n�����㷨Ч�ʱȽϣ�������С: 10000��:" << endl;
    testSortingEfficiency();

    return 0;
}

