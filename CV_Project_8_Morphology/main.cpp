//  CV_Project_2_Avg_Median_Filter
//  Created by Daniel Rand on 9/6/19.

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class Morphology {
    
public:
    int numRowsImg, numColsImg, minImg, maxImg, numRowsStructElem, numColsStructElem,
    minStrctElem, maxStrctElem, rowOrigin, colOrigin, rowFrameSize, colFrameSize;
    // needs to dynamically allocate at run time
    // of size numRowsImg + rowFrameSize*2 by  numColsImg + colFrameSize*2.
    int ** imgAry;
    // a 2D array, need to dynamically allocate at run time
    // of size numRowsImg + rowFrameSize*2 by numColsImg + colFrameSize * 2.
    int ** morphAry;
    //a 2D array, need to dynamically allocate at run time
    // of size numRowsStructElem by numColsStructElem.
    int ** structElemAry;
    
    
public:
    
    Morphology (ifstream &input, ifstream &structElement) {
        input >> numRowsImg >> numColsImg >> minImg >> maxImg;
        structElement >> numRowsStructElem >> numColsStructElem >> minStrctElem >> maxStrctElem
        >> rowOrigin >> colOrigin;
    }
    
    void computeFrameSize() {
        rowFrameSize = numRowsImg/2;
        colFrameSize = numColsImg/2;
    }
    
    void loadImage(ifstream &input) {
        int rows = numRowsImg+(rowFrameSize*2);
        int cols = numColsImg+(colFrameSize*2);
        imgAry = new int * [rows];
        morphAry = new int * [rows];
        for (int i = 0; i < rows; i++) {
            imgAry[i] = new int [cols]();
            morphAry[i] = new int [cols]();
        }
        for (int i = 0; i < numRowsImg; i++) {
            for (int j = 0; j < numColsImg; j++) {
                int current;
                input >> current;
                imgAry[i+rowFrameSize][j+colFrameSize] = current;
                morphAry[i+rowFrameSize][j+colFrameSize] = current;
            }
        }
        //Print array
    }
    
    void loadStruct (ifstream &structEl) {
        structElemAry = new int * [numRowsStructElem];
        for (int i = 0; i < numRowsStructElem; i++)
            structElemAry[i] = new int [numColsStructElem];
        for (int i = 0; i < numRowsStructElem; i++) {
            for (int j = 0; j < numColsStructElem; j++) {
                int current;
                structEl >> current;
                structElemAry[i][j] = current;
            }
        }
    }
    
    void prettyPrintAry (int ** array, ofstream & outFile) {
        for (int i = rowFrameSize; i < rowFrameSize+numRowsImg; i++) {
            for (int j = colFrameSize; j <colFrameSize+numColsImg; j++) {
                int current = array[i][j];
                if (current > 0) outFile << current << " ";
                else outFile << "  ";
            } outFile << endl;
        }
    }
    
    void prettyPrintStruct (int ** array, ofstream & outFile) {
        for (int i = 0; i < numRowsStructElem; i++) {
            for (int j = 0; j < numColsStructElem; j++) {
                int current = structElemAry[i][j];
                if (current > 0) outFile << current << " ";
                else outFile << "  ";
            } outFile << endl;
        }
    }
    
    void dilation() {
        for (int i = rowFrameSize; i < rowFrameSize+numRowsImg; i++) {
            for (int j = colFrameSize; j <colFrameSize+numColsImg; j++) {
                if (imgAry[i][j] > 0) {
                    int structRow = 0, structCol = 0;
                    for (int row = i-rowOrigin; row < i + (numRowsStructElem-rowOrigin); row++) {
                        for (int col = j-colOrigin; col < j + (numColsStructElem-colOrigin); col++) {
                            int old = morphAry [row][col];
                            int structEl = structElemAry[structRow][structCol];
                            if (morphAry[row][col]==0)
                                morphAry[row][col] = structElemAry[structRow][structCol];
                            structCol++;
                        }
                        structCol = 0;
                        structRow++;
                    }
                }
            }
        }
        copyArrays();
    }
    
    void erosion () {
        for (int i = rowFrameSize; i < rowFrameSize+numRowsImg; i++) {
            for (int j = colFrameSize; j <colFrameSize+numColsImg; j++) {
                if (imgAry[i][j] > 0) {
                    int structRow = 0, structCol = 0;
                    for (int row = i-rowOrigin; row < i + (numRowsStructElem-rowOrigin); row++) {
                        for (int col = j-colOrigin; col < j + (numColsStructElem-colOrigin); col++) {
                            if (structElemAry[structRow][structCol] != 0 && imgAry[row][col] != structElemAry[structRow][structCol])
                                morphAry[i][j] = 0;
                            structCol++;
                        }
                        structCol = 0;
                        structRow++;
                    }
                }
            }
        }
        copyArrays();
    }
    
    void opening () {
        erosion();
        dilation();
    }
    
    void closing () {
        dilation();
        erosion();
    }
    
    void copyArrays () {
        int rows = numRowsImg+(rowFrameSize*2);
        int cols = numColsImg+(colFrameSize*2);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                imgAry[i][j] = morphAry[i][j];
    }
    /*
     
     - methods:
     - computeFrameSize(...) // compute the rowFrameSize and colFrameSize
     - loadImage  // load imgAry from input1, begins at (rowFrameSize, colFrameSize) and ends at ??
     - loadstruct  // load structElem from input2
     - zeroFrameImg // frame the input image with zero on those extra rows and extra cols.
     
     - initMorphAry( ) // initialize morphAry to zero
     
     - dilation () // as taught in class, on your own
     - erosion () // as taught in class, on your own
     - opening () // erosion follows by dilation
     - closing () // dilation follows by erosion
     - prettyPrint ()*/
};

int main(int argc, const char * argv[]) {
    ifstream inFile, structElFile;
    ofstream outFile1, outFile2, outFile3;
    inFile.open(argv[1]);
    structElFile.open(argv[2]);
    outFile1.open(argv[3]);
    outFile2.open(argv[4]);
    outFile3.open(argv[5]);
    Morphology M(inFile,structElFile);
    M.computeFrameSize();
    M.loadImage(inFile);
    M.prettyPrintAry(M.imgAry, outFile1);
    M.loadStruct(structElFile);
    M.prettyPrintStruct(M.structElemAry, outFile2);
    int operation;
    while (true) {
        cout << "1 - Dilation\n2 - Erosion\n3 - Opening\n4 - Closing\n\nWhich morphological operatoration would you like to apply to the input image? ";
        cin >> operation;
        switch (operation) {
            case 1:
                M.dilation();
                break;
            case 2:
                M.erosion();
                break;
            case 3:
                M.opening();
                break;
            case 4:
                M.closing();
                break;
            default:
                cout << "\nPlease only enter 1/2/3/4...\n\n";
                continue;
        }
        break;
    }
    M.prettyPrintAry(M.morphAry, outFile3);
    inFile.close();
    structElFile.close();
    outFile1.close();
    outFile2.close();
    outFile3.close();
    return 0;
}
