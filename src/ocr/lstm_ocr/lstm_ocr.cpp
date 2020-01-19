/******************************************************************************
 * OpenCV + Tesseract application for Optical Character Recognition 
 *
 * Tesseract was tweaked through its API to performe OCR with the specific Page
 * Segmentation Mode (assume a single column of text of variable sizes) that 
 * showed the best results on the `tin_001.jpeg` test image. Additionally, the 
 * character classigication is set to be ran by the LSTM engine of Tesseract.
 * For more costumization tips check:
 *	 https://github.com/tesseract-ocr/tesseract/wiki/ImproveQuality
 ******************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono> 

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std::chrono;
using namespace cv;

void filterChars(string &str)
{
	/**
	 * Filters out none alphanumeric characters from the input string by 
	 * replacing them with blanks. Modifications to the string are made 
	 * inplace.
	 */
	
	replace_if(str.begin(), str.end(), 
		[](char c) {return !(isalpha(c) || isdigit(c));}, ' ');
}

string getLongestLine(string const &str, bool filter)
{
	/**
	 * Returns the longest string sequence/line in the input (multi-line) 
	 * string. Optionally, filters out none-alphanumeric characters in each 
	 * line.
	 */
	
	size_t start;
	size_t end = 0;
	
	vector<string> lines;
	string line;
	const char delim = '\n';
	string longstLine;

	while ((start = str.find_first_not_of(delim, end)) != string::npos)
	{
		end = str.find(delim, start);
		line = str.substr(start, end - start);
		
		if (filter) {
			filterChars(line);
		}
		lines.push_back(line);
	}
		
	longstLine = lines[0];
	for (auto &s: lines) {
		if (s.length() >  longstLine.length()){
			longstLine = s;
		}
	}
	
	return longstLine;
}

int main(int argc, char** argv)
{
	/**
	 * Prints the longest string detected in the passed input file path. 
	 * The core OCR operation is executed by the Tesseract module and its
	 * LSMT engine implementation. 
	 *
	 */
	
    if (argc != 2)
    {
		printf("Usage: ./basic_ocr <path/to/input/image>\n");
		return -1;
    }
	
	auto start_time = high_resolution_clock::now(); 

    // Create vars for output and input strings
    string rawText;
	string outputText;
	string imagePath = argv[1];
    string header = "\n+------ START OF DETECTED CODE ------+\n\n";  
    string footer = "\n\n+------  END OF DETECTED CODE  ------+\n\n";
	bool filter = true;
	
    Mat image = imread(imagePath, IMREAD_COLOR);

    // Instantiate tesseract object
    tesseract::TessBaseAPI *theTesseract = new tesseract::TessBaseAPI();

    // Initialize tesseract language (English) and its engine (LSTM based)
    theTesseract->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
	
    // Set the page segmentation mode to "single colum" (equivalent to --psm 4)
    theTesseract->SetPageSegMode(tesseract::PSM_SINGLE_COLUMN);

    // Process the specified image
    theTesseract->SetImage(image.data, image.cols, image.rows, 3, image.step);
    rawText = string(theTesseract->GetUTF8Text());

	// Extract the longest detected line (filter none-alphanumeric characters)
	outputText = getLongestLine(rawText, filter);
	
    cout << header << outputText << footer;

    // Cleaup
    theTesseract->End();
	
	auto end_time = high_resolution_clock::now(); 
	auto duration = duration_cast<milliseconds>(end_time - start_time); 
  	cout << "All done in " << duration.count() <<  " ms" << endl; 

    return 0;
}