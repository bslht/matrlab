#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

bool readMatrix(const string& filename, vector<vector<double>>& matrix, int expectedSize) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: cannot open file " << filename << endl;
        return false;
    }

    int n;
    file >> n;
    if (n != expectedSize) {
        cerr << "Error: matrix size mismatch" << endl;
        return false;
    }

    matrix.resize(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            file >> matrix[i][j];

    file.close();
    return true;
}

void writeMatrix(const string& filename, const vector<vector<double>>& matrix) {
    ofstream file(filename);
    int n = matrix.size();
    file << n << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            file << matrix[i][j] << " ";
        file << endl;
    }
    file.close();
}

vector<vector<double>> multiplyMatrices(const vector<vector<double>>& A,
    const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];

    return C;
}

int main(int argc, char* argv[]) {
   
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <matrix_size>" << endl;
        return -1;
    }

    int N = atoi(argv[1]);

    cout << "Matrix size: " << N << "x" << N << endl;
    cout << "Total operations: " << 2.0 * N * N * N << endl;

    vector<vector<double>> A, B;

    if (!readMatrix("matrix_A.txt", A, N)) return -1;
    if (!readMatrix("matrix_B.txt", B, N)) return -1;

    cout << "Matrices loaded, starting multiplication..." << endl;

    auto start = high_resolution_clock::now();

    vector<vector<double>> C = multiplyMatrices(A, B);

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "Calculation time: " << elapsed.count() << " seconds" << endl;

    writeMatrix("result_matrix.txt", C);

    ofstream timeFile("times.txt", ios::app);
    timeFile << N << " " << elapsed.count() << endl;
    timeFile.close();

    return 0;
}