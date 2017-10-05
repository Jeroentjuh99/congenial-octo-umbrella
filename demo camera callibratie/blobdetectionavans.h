// blobdetectionavans - Blobdetection based on OpenCV 2.4.10 
// Goal: maybe easier to use alternative for SimpleBLOBDetector of OpenCV
// 
// Copyright Jan Oostindie, basic version 0.2 dd 15-9-2016. 
//
// Note: Students of Avans are free to use code in projects and for own vision competence development. Others may ask permission to use it by means 
// of sending an email to Jan Oostindie, i.e. jac.oostindie@avans.nl

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;


/*********************************** BLOB operations ****************************************/
// NB images are supposed to have 1 channel (B/W image) and depth 16 bits signed (CV_16S) 
/********************************************************************************************/

// func: labeling of all blobs in a binary image
// pre : binaryImage has depth 16 bits signed int. Contains only values 0 and 1.
// post: labeledImage: binary 8-connected pixels with value 1 in binaryImage are 
//       labeled with the number of the object they belong to.
// return_value: the total number of objects.  
int labelBLOBs(Mat binaryImage, Mat & labeledImage);

// func: labeling of all blobs in a binary image with a area in [threhAreaMin,threhAreaMax]. Default
//       threshold is [1,INT_MAX]. Alle gathered data during the labeling proces is returned, 
//       i.e. the positions of the firstpixel of each blob, the position of the blobs (i.e. the
//       centres of gravity) and the area's of all blobs.
// pre : binaryImage has depth 16 bits signed int. Contains only values 0 and 1.
// post: labeledImage: binary 8-connected pixels with value 1 in binaryImage are 
//       labeled with the number of the object they belong to.
//       areaVec: contains all area's of the blobs. The index corresponds to the number
//       of the blobs. Index 0 has no meaning.
// return_value: the total number of objects.  
int labelBLOBsInfo(Mat binaryImage, Mat & labeledImage,
	vector<Point2d *> & firstpixelVec, vector<Point2d *> & posVec,
	vector<int> & areaVec,
	int threshAreaMin = 1, int threshAreaMax = INT_MAX);


/*********************************** Some Basic operations **********************************/
// NB images are supposed to have 1 channel (B/W image) and depth 16 bits signed (CV_16S) 
/********************************************************************************************/

// func: shows a 16S image on the screen. All values mapped on the interval 0-255 
/// pre: m is a 16S image (depth 16 bits, signed)
void show16SImageStretch(Mat m, string windowTitle = "show16SImageStretch");

// func: setup a specified entry (i,j) of a matrix m with a specific value 
// pre: (i < m.rows) & (j < m.cols)
void setEntryImage(Mat m, int i, int j, _int16 value);

// func: get the value of a specified entry (i,j) of a matrix m 
// pre: (i < m.rows) & (j < m.cols)
// return: <return_value> == m(i,j)
_int16 getEntryImage(Mat m, int i, int j);

// func: searches the maximum pixel value in the image
// return: maximum pixel value
_int16 maxPixelImage(Mat m);

// func: searches the minimum pixel value in the image
// return: minimum pixel value
_int16 minPixelImage(Mat m);

// func: select and get a row of a matrix m. rowNr contains the row number
// pre: 0 < rowNr < m.rows
// return: <result matrix> contains the selected row
Mat getRowImage(Mat m, int rowNr);

// func: get a column of a matrix m. colNr contains the column number
// pre: 0 < colNr < m.cols
// return: <result matrix> contains the selected column
Mat getColImage(Mat m, int colNr);


