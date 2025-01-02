#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace std;
using namespace rapidjson;

// Function to convert a string in a given base to a number
long long decodeBaseValue(const string& encodedValue, int base) {
    return stoll(encodedValue, nullptr, base);
}

// Function to find the constant term using Lagrange interpolation
long long performLagrangeInterpolation(const vector<pair<int, long long>>& points, int k) {
    long long result = 0;

    for (int i = 0; i < k; ++i) {
        long long term = points[i].second;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= -points[j].first;
                term /= (points[i].first - points[j].first);
            }
        }
        result += term;
    }

    return result;
}

void processTestCase(const char* filename) {
    // Open the JSON file
    FILE* filePointer = fopen(filename, "r");
    if (!filePointer) {
        cerr << "Error: Can't open " << filename << endl;
        return;
    }

    // Read and parse the JSON file
    char buffer[65536];
    FileReadStream inputStream(filePointer, buffer, sizeof(buffer));
    Document jsonDocument;
    jsonDocument.ParseStream(inputStream);
    fclose(filePointer);

    // Get the number of points and polynomial degree
    const Value& keyData = jsonDocument["keys"];
    int numPoints = keyData["n"].GetInt();
    int polynomialDegree = keyData["k"].GetInt();

    // Prepare the points for interpolation
    vector<pair<int, long long>> points;

    for (int i = 1; i <= numPoints; ++i) {
        string index = to_string(i);
        int xValue = i;
        const Value& rootData = jsonDocument[index.c_str()];
        int base = rootData["base"].GetInt();
        string encodedValue = rootData["value"].GetString();
        long long decodedY = decodeBaseValue(encodedValue, base);
        points.emplace_back(xValue, decodedY);
    }

    // Find the constant term using Lagrange interpolation
    long long constantTerm = performLagrangeInterpolation(points, polynomialDegree);

    // Print the constant term
    cout << "The secret (constant term c) for " << filename << " is: " << constantTerm << endl;
}

int main() {
    // Process each test case
    processTestCase("input1.json");
    processTestCase("input2.json");

    return 0;
}
