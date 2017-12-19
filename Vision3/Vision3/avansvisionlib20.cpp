// avansvisionlib - Growing Visionlibrary of Avans based on OpenCV 2.4.10 
// Goal: deep understanding of vision algorithms by means of developing own (new) algorithms.
//       deep understanding of neural networks
// 
// Copyright Jan Oostindie, version 2.0 dd 5-12-2016 (= Neural Network (BPN) added to version 1.0 dd 5-11-2016.) 
//      Contains basic functions to perform calculations on matrices/images of class Mat. Including BLOB labeling functions 
//      Contains a BPN neural network. 
// Note: Students of Avans are free to use this library in projects and for own vision competence development. Others may ask permission to use it by means 
// of sending an email to Jan Oostindie, i.e. jac.oostindie@avans.nl

#include "stdafx.h"
#include "avansvisionlib20.h"
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <opencv2/imgproc.hpp>

// pre: (i < m.rows) & (j < m.cols)
// Mat is call by reference
void setEntry( Mat m, int i, int j, double value ) {
	int index = i * m.cols + j;
	double* p = m.ptr<double>( 0 );
	p[index] = value;
} //  setEntry

// pre: (i < m.rows) & (j < m.cols)
double getEntry( Mat m, int i, int j ) {
	int index = i * m.cols + j;
	double* p = m.ptr<double>( 0 );
	return *( p + index );
} //  getEntry

// func: calculate product of a row and column of equal length  
// pre: (row.cols == col.rows) 
double inproduct( Mat row, Mat col ) {
	double* p1 = row.ptr<double>( 0 );
	double* p2 = col.ptr<double>( 0 );
	double sumproducts = 0;
	for ( int i = 0; i < row.cols; i++ )
		sumproducts += p1[i] * p2[i];
	return sumproducts;
} // inproduct

void printMatrix( Mat m ) {
	for ( int i = 0; i < m.rows; i++ ) {
		for ( int j = 0; j < m.cols; j++ )
			cout << getEntry( m, i, j ) << "  ";
		cout << endl;
	}
	cout << endl;
} // printMatrix

Mat getRow( Mat m, int rowNr ) {
	Mat result = Mat_<double>( 1, m.cols );
	double entry;
	for ( int colNr = 0; colNr < m.cols; colNr++ ) {
		entry = getEntry( m, rowNr, colNr );
		//	cout << " ** getRow ** " << endl;
		//	cout << " entry = " << entry << endl;
		setEntry( result, 0, colNr, entry );
	}
	return result;
} // getRow

Mat getCol( Mat m, int colNr ) {
	Mat result = Mat_<double>( m.rows, 1 );
	double entry;
	for ( int rowNr = 0; rowNr < m.rows; rowNr++ ) {
		entry = getEntry( m, rowNr, colNr );
		//	cout << " ** getColumn ** " << endl;
		//	cout << " entry = " << entry << endl;
		setEntry( result, rowNr, 0, entry );
	}
	return result;
} // getCol

// pre: (a.cols == b.rows)
Mat multiply( Mat a, Mat b ) {
	Mat result = Mat_<double>( a.rows, b.cols );
	double inprod;
	for ( int arow = 0; arow < a.rows; arow++ ) {
		for ( int bcol = 0; bcol < b.cols; bcol++ ) {
			inprod = inproduct( getRow( a, arow ), getCol( b, bcol ) );
			setEntry( result, arow, bcol, inprod );
		}
	}
	return result;
} // multiply

// pre: matrices have equal dimensions i.e. (a.cols == b.cols) & (a.rows == b.rows) 
Mat add( Mat a, Mat b ) {
	Mat result = Mat_<double>( a.rows, a.cols );
	double entrysum;
	for ( int row = 0; row < a.rows; row++ ) {
		for ( int col = 0; col < a.cols; col++ ) {
			entrysum = getEntry( a, row, col ) + getEntry( b, row, col );
			setEntry( result, row, col, entrysum );
		}
	}
	return result;
} // add

Mat transpose( Mat m ) {
	Mat result = Mat_<double>( m.cols, m.rows );
	for ( int row = 0; row < m.rows; row++ )
		for ( int col = 0; col < m.cols; col++ )
			setEntry( result, col, row, getEntry( m, row, col ) );
	return result;
} // transpose

// func: sets all entries of a matrix to a value
// pre: true
void setValue( Mat m, double value ) {
	for ( int row = 0; row < m.rows; row++ )
		for ( int col = 0; col < m.cols; col++ )
			setEntry( m, row, col, value );
} // setValue

double generateRandomValue( double min, double max ) {
	int steps = rand() % 100 + 1;
	double dx = ( max - min ) / 100;
	return min + dx * steps;
} // generateRandomValue

// func: sets all entries of a matrix to a random value in interval [min,max]
// pre: true
void setRandomValue( Mat m, double min, double max ) {
	srand( time( nullptr ) );
	for ( int row = 0; row < m.rows; row++ )
		for ( int col = 0; col < m.cols; col++ )
			setEntry( m, row, col, generateRandomValue( min, max ) );
} // randomValue

/*********************************** Image operaties ****************************************/
// NB images are supposed to have 1 channel (B/W image) and depth 16 bits signed (CV_16S) 
/********************************************************************************************/

void setEntryImage( Mat m, int i, int j, _int16 value ) {
	int index = i * m.cols + j;
	_int16* p = m.ptr<_int16>( 0 );
	p[index] = value;
} //  setEntry

// pre: (i < m.rows) & (j < m.cols)
_int16 getEntryImage( Mat m, int i, int j ) {
	int index = i * m.cols + j;
	_int16* p = m.ptr<_int16>( 0 );
	return *( p + index );
} //  getEntryImage

// func: calculate product of a row and column of equal length  
// pre: (row.cols == col.rows) 
_int16 inproductImage( Mat row, Mat col ) {
	_int16* p1 = row.ptr<_int16>( 0 );
	_int16* p2 = col.ptr<_int16>( 0 );
	_int16 sumproducts = 0;
	for ( int i = 0; i < row.cols; i++ )
		sumproducts += p1[i] * p2[i];
	return sumproducts;
} // inproductImage

Mat getRowImage( Mat m, int rowNr ) {
	Mat result = Mat_<_int16>( 1, m.cols );
	_int16 entry;
	for ( int colNr = 0; colNr < m.cols; colNr++ ) {
		entry = getEntryImage( m, rowNr, colNr );
		setEntryImage( result, 0, colNr, entry );
	}
	return result;
} // getRow

Mat getColImage( Mat m, int colNr ) {
	Mat result = Mat_<_int16>( m.rows, 1 );
	_int16 entry;
	for ( int rowNr = 0; rowNr < m.rows; rowNr++ ) {
		entry = getEntryImage( m, rowNr, colNr );
		setEntryImage( result, rowNr, 0, entry );
	}
	return result;
} // getColImage

Mat multiplyImage( Mat a, Mat b ) {
	Mat result = Mat_<_int16>( a.rows, b.cols );
	_int16 inprod;
	for ( int arow = 0; arow < a.rows; arow++ ) {
		for ( int bcol = 0; bcol < b.cols; bcol++ ) {
			inprod = inproductImage( getRow( a, arow ), getColImage( b, bcol ) );
			setEntry( result, arow, bcol, inprod );
		}
	}
	return result;
} // multiplyImage

// pre: matrices have equal dimensions i.e. (a.cols == b.cols) & (a.rows == b.rows) 
Mat addImage( Mat a, Mat b ) {
	Mat result = Mat_<_int16>( a.rows, a.cols );
	_int16 entrysum;
	for ( int row = 0; row < a.rows; row++ ) {
		for ( int col = 0; col < a.cols; col++ ) {
			entrysum = getEntryImage( a, row, col ) + getEntryImage( b, row, col );
			setEntryImage( result, row, col, entrysum );
		}
	}
	return result;
} // addImage

// func: searches the maximum pixel value in the image
// return: maximum pixel
_int16 maxPixelImage( Mat m ) {
	_int16 max = getEntryImage( m, 0, 0 );
	_int16 next;
	for ( int row = 0; row < m.rows; row++ ) {
		for ( int col = 0; col < m.cols; col++ ) {
			next = getEntryImage( m, row, col );
			if ( next > max )
				max = next;
		}
	}
	return max;
} // maxPixelImage

// func: searches the minimum pixel value in the image
// return: minimum pixel value
_int16 minPixelImage( Mat m ) {
	_int16 min = getEntryImage( m, 0, 0 );
	_int16 next;
	for ( int row = 0; row < m.rows; row++ ) {
		for ( int col = 0; col < m.cols; col++ ) {
			next = getEntryImage( m, row, col );
			if ( next < min )
				min = next;
		}
	}
	return min;
} // minPixelImage

// func: determines the range of the image, i.e. the minimum 
// and maximum pixel value in the image
// post: range = minPixelValue, maxPixelValue
void getPixelRangeImage( Mat m, _int16& minPixelValue, _int16& maxPixelValue ) {
	_int16 max = getEntryImage( m, 0, 0 );
	_int16 min = getEntryImage( m, 0, 0 );
	_int16 next;
	for ( int row = 0; row < m.rows; row++ ) {
		for ( int col = 0; col < m.cols; col++ ) {
			next = getEntryImage( m, row, col );
			if ( next > max )
				max = next;
			else if ( next < min )
				min = next;
		}
	}
	minPixelValue = min;
	maxPixelValue = max;
} // getPixelRangeImage

// func: stretches the image to a specified range
void stretchImage( Mat m, _int16 minPixelValue, _int16 maxPixelValue ) {
	_int16 min, max, oldValue, newValue;
	getPixelRangeImage( m, min, max );
	double scale = maxPixelValue - minPixelValue;
	scale /= ( max - min );
	for ( int row = 0; row < m.rows; row++ ) {
		for ( int col = 0; col < m.cols; col++ ) {
			oldValue = getEntryImage( m, row, col );
			newValue = scale * ( oldValue - min ) + minPixelValue;
			setEntryImage( m, row, col, newValue );
		}
	}
} // stretchImage

// func: shows a 16S image on the screen 
// pre: m is a 16S image (depth 16 bits, signed)
void show16SImageStretch( Mat m, string windowTitle ) {
	Mat mCopy;
	m.copyTo( mCopy );
	stretchImage( mCopy, 0, 255 );
	mCopy.convertTo( mCopy, CV_8U );
	//	namedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
	imshow( windowTitle, mCopy );
	waitKey( 0 );
} // show16SImage

// func: shows a 16S image on the screen. All values clipped to the interval 0-255
// i.e. value < 0 => 0; 0 <= value <= 255 => value ; value > 255 => 255 
/// pre: m is a 16S image (depth 16 bits, signed)
void show16SImageClip( Mat m, string windowTitle ) {
	Mat mCopy;
	m.copyTo( mCopy );
	mCopy.convertTo( mCopy, CV_8U );
	//	namedWindow("show16SImageClip", CV_WINDOW_AUTOSIZE);
	imshow( windowTitle, mCopy );
	waitKey( 0 );
} // show16SImage

// func: histogram gamma correction
// pre: image has depth 8 bits unsigned and 1 or 3 channels
// post: entry(i,j) = 255*power(entry@pre(i,j)/255)^gamma
void gammaCorrection( Mat image, float gamma ) {
	unsigned char lut[256];
	for ( int i = 0; i < 256; i++ ) {
		lut[i] = saturate_cast<uchar>( pow( (float)( i / 255.0 ), gamma ) * 255.0f );
	}
	//	dst = src.clone();
	const int channels = image.channels();
	switch ( channels ) {
		case 1: {
			MatIterator_<uchar> it, end;
			for ( it = image.begin<uchar>(), end = image.end<uchar>(); it != end; ++it )
				*it = lut[( *it )];
			break;
		}
		case 3: {
			MatIterator_<Vec3b> it, end;
			for ( it = image.begin<Vec3b>(), end = image.end<Vec3b>(); it != end; ++it ) {
				( *it )[0] = lut[( ( *it )[0] )];
				( *it )[1] = lut[( ( *it )[1] )];
				( *it )[2] = lut[( ( *it )[2] )];
			}
			break;
		}
	} // switch
} // gammaCorrection

// func: makes a administration used for labeling blobs.
//       the function adds a edge of 1 pixel wide tot a binary image, all with value 0. 
//       All 1's are made -1. The result is returned.
//       This function is used by function labelBLOBs
// pre : binaryImage has depth 16 bits signed int. Contains only values 0 and 1.
// return_matrix: All "1" are made "-1" meaning value 1 and unvisited.
Mat makeAdmin( Mat binaryImage ) {
	Mat result = Mat_<_int16>( binaryImage.rows + 2, binaryImage.cols + 2 );

	// eerste rij 0 maken
	for ( int col = 0; col < result.cols; col++ )
		setEntryImage( result, 0, col, 0 );

	// binaryImage copieren naar admin waarbij een 1 steeds wordt omgezet naar -1.
	for ( int row = 1; row < ( result.rows - 1 ); row++ ) {

		// 0 vooraan de rij zetten
		setEntryImage( result, row, 0, 0 );

		// rij binaryImage copieren 
		_int16 value;
		for ( int col = 1; col < result.cols - 1; col++ ) {
			value = getEntryImage( binaryImage, row - 1, col - 1 );
			if ( value == 1 )
				value = -1;
			setEntryImage( result, row, col, value );
		}

		// 0 achteraan de rij zetten
		setEntryImage( result, row, result.cols - 1, 0 );
	} // for

	// laatste rij 0 maken
	for ( int col = 0; col < result.cols; col++ )
		setEntryImage( result, result.rows - 1, col, 0 );

	return result;
} // makeAdmin

// func: Searches the next blob after position (row,col) i.e. searches
//       the next -1 in admin 
// post: if return_value == 1 then (row,col) contains the position
//       where the next blob starts.	  
// return_value: 1 next blob found ; starting position is (row,col)
//           	 0 no blob found   ; (row, col) == (-1, -1)                   
bool findNextBlob( Mat admin, int& row, int& col ) {

	bool found = false;

	// zoeken in de huidige rij
	for ( int currCol = col; ( currCol < ( admin.cols - 1 ) ) & !found; currCol++ )
		if ( getEntryImage( admin, row, currCol ) == -1 ) {
			found = true;
			// row unchanged
			col = currCol;
		}

	// zoeken vanaf de volgende rij
	for ( int currRow = row + 1; ( currRow < ( admin.rows - 1 ) ) & !found; currRow++ )
		for ( int currCol = 1; ( currCol < ( admin.cols - 1 ) ) & !found; currCol++ )
			if ( getEntryImage( admin, currRow, currCol ) == -1 ) {
				found = true;
				row = currRow;
				col = currCol;
			}

	if ( !found ) {
		row = -1;
		col = -1;
	}
	return found;
}; // findNextBlob

// func: gets the entry of a neighbour pixel with relative position nr. 
//       Definition of relative positions nr:
//          7  0  1
//          6  X  2 
//          5  4  3
_int16 getEntryNeighbour( const Mat& admin, int x, int y, int nr ) {
	switch ( nr ) {
		case 0:
			return getEntryImage( admin, x - 1, y );
			break;
		case 1:
			return getEntryImage( admin, x - 1, y + 1 );
			break;
		case 2:
			return getEntryImage( admin, x, y + 1 );
			break;
		case 3:
			return getEntryImage( admin, x + 1, y + 1 );
			break;
		case 4:
			return getEntryImage( admin, x + 1, y );
			break;
		case 5:
			return getEntryImage( admin, x + 1, y - 1 );
			break;
		case 6:
			return getEntryImage( admin, x, y - 1 );
			break;
		case 7:
			return getEntryImage( admin, x - 1, y - 1 );
			break;
		default:
			cout << "ERROR getEntryNeighbour " << endl;
	}
} // getEntryNeighbour

// func: determines if there are more than 1 adjacent 1's 
bool moreNext1( const Mat& admin, int x, int y ) {
	int cnt1 = 0;
	bool more = false;
	for ( int nr = 0; ( nr <= 7 ) & !more; nr++ )
		if ( getEntryNeighbour( admin, x, y, nr ) == -1 ) {
			cnt1++;
			if ( cnt1 > 1 )
				more = true;
		}
	return more;
} // moreNext1

// func: searches the first 1 when rotating around the pixel (currX,currY), 
//       starting at position 0. Definition of relative positions: 
//          7  0  1
//          6  X  2
//          5  4  3
void findNext1( Mat admin, int& currX, int& currY, int& next1 ) {
	int rotX, rotY;

	rotX = currX - 1;
	rotY = currY; //0 
	if ( getEntryImage( admin, rotX, rotY ) == -1 )
		next1 = 0;
	else {
		rotX = currX - 1;
		rotY = currY + 1; //1
		if ( getEntryImage( admin, rotX, rotY ) == -1 )
			next1 = 1;
		else {
			rotX = currX;
			rotY = currY + 1; //2 
			if ( getEntryImage( admin, rotX, rotY ) == -1 )
				next1 = 2;
			else {
				rotX = currX + 1;
				rotY = currY + 1; //3
				if ( getEntryImage( admin, rotX, rotY ) == -1 )
					next1 = 3;
				else {
					rotX = currX + 1;
					rotY = currY; //4 
					if ( getEntryImage( admin, rotX, rotY ) == -1 )
						next1 = 4;
					else {
						rotX = currX + 1;
						rotY = currY - 1; //5
						if ( getEntryImage( admin, rotX, rotY ) == -1 )
							next1 = 5;
						else {
							rotX = currX;
							rotY = currY - 1; //6
							if ( getEntryImage( admin, rotX, rotY ) == -1 )
								next1 = 6;
							else {
								rotX = currX - 1;
								rotY = currY - 1; //7
								if ( getEntryImage( admin, rotX, rotY ) == -1 )
									next1 = 7;
								else
									next1 = -99;
							} // 6
						} // 5
					} // 4
				} // 3
			} // 2
		} // 1
	} //0
	if ( next1 >= 0 ) {
		currX = rotX;
		currY = rotY;
	}
} // findNext1

//  func: labels all pixels of one blob which starts at position (row,col) with blobNr. 
//        This function is used by function labelBLOB's which labels all blobs. 
//  return_value: area of the blob 
//  Evaluation: This function uses a iterative algorithm in which a special labeling technique is
//        is used which gives the opportunity to trace all individiual pixels. This makes it
//        possible for example to save only these pixels on disk or to translate the object in 
//        in the image.
//        The disadvantagae however is that the algorithm is more complicated an maybe a little bit
//        slower than the recursive variant. 
int labelIter( Mat& admin, int row, int col, int blobNr ) {
	//  Every visited pixel is labeled with:
	//  blobNr*10 + <relative position to the parent >
	//
	//  definition of relative positions 
	//  7  0  1
	//  6  X  2
	//  5  4  3
	//
	//  The first visited pixel, i.e. (row,col), is labeled with:
	//  blobNr * 10 + 8

	int x = row, y = col;
	setEntryImage( admin, x, y, blobNr * 10 + 8 );

	int next1 = -999;
	int area = 1;

	// flag more is set when any entry in the path has two or more 
	// unvisited neighbours because we visit only one at a time.
	bool allLabeledFlag = true;
	while ( allLabeledFlag ) {
		allLabeledFlag = false;
		bool pathLabeled = false;
		while ( !pathLabeled ) {

			if ( !allLabeledFlag )
				allLabeledFlag = moreNext1( admin, x, y );
			findNext1( admin, x, y, next1 );

			if ( next1 >= 0 ) {
				setEntryImage( admin, x, y, blobNr * 10 + next1 );
				area++;
			} else {
				//findprevious
				switch ( getEntryImage( admin, x, y ) % 10 ) {
					case 0:
						x += 1;
						break;
					case 1:
						x += 1;
						y -= 1;
						break;
					case 2:
						y -= 1;
						break;
					case 3:
						x -= 1;
						y -= 1;
						break;
					case 4:
						x -= 1;
						break;
					case 5:
						x -= 1;
						y += 1;
						break;
					case 6:
						y += 1;
						break;
					case 7:
						x += 1;
						y += 1;
						break;
					case 8:
						pathLabeled = true;
						break; // currIndex should be 0 now
					default:
						cout << "Error func labelIter!";
				} // case
			} // else
		} // while
	} // while (more)
	return area;
} // labelIter

//  func: labels all pixels of one blob which starts at position (topX,topY) with blobNr. 
//        During the labeling proces the centre of gravity is calculated. 
//        This function is used by function labelBLOBInfo 
//  return_value: area of the blob 
//  Evaluation: This function uses a iterative algorithm in which a special labeling technique is
//        is used which gives the opportunity to trace all individiual pixels. This makes it
//        possible for example to save only these pixels on disk or to translate the object in 
//        in the image.
//        The disadvantagae however is that the algorithm is more complicated an maybe a little bit
//        slower than the recursive variant. 
int labelIterInfo( Mat& admin, int topX, int topY, int blobNr,
                   int& xGravity, int& yGravity ) {

	//  Every visited pixel is labeled with:
	//  blobNr*10 + <relative position to the parent >
	//
	//  definition of relative positions 
	//  7  0  1
	//  6  X  2
	//  5  4  3
	//
	//  The first visited pixel, i.e. (row,col), is labeled with:
	//  blobNr * 10 + 8

	xGravity = topX;
	yGravity = topY;
	int x = topX, y = topY;
	setEntryImage( admin, topX, topY, blobNr * 10 + 8 );

	int next1 = -999;
	int area = 1;

	// allLabeledFlag is set when any entry in the path has two or more 
	// unvisited neighbours because we visit only one at a time.
	// This algorithm good be speeded up by using a stack (future improvement)
	bool allLabeledFlag = true;
	while ( allLabeledFlag ) {
		allLabeledFlag = false;
		bool pathLabeled = false;
		while ( !pathLabeled ) {

			if ( !allLabeledFlag )
				allLabeledFlag = moreNext1( admin, x, y );
			findNext1( admin, x, y, next1 );

			if ( next1 >= 0 ) {
				setEntryImage( admin, x, y, blobNr * 10 + next1 );
				area++;
				xGravity += x;
				yGravity += y;
			} else {
				//findprevious
				switch ( getEntryImage( admin, x, y ) % 10 ) {
					case 0:
						x += 1;
						break;
					case 1:
						x += 1;
						y -= 1;
						break;
					case 2:
						y -= 1;
						break;
					case 3:
						x -= 1;
						y -= 1;
						break;
					case 4:
						x -= 1;
						break;
					case 5:
						x -= 1;
						y += 1;
						break;
					case 6:
						y += 1;
						break;
					case 7:
						x += 1;
						y += 1;
						break;
					case 8:
						pathLabeled = true;
						break; // currIndex should be 0 now
					default:
						cout << "Error func labelIter!";
				} // case
			} // else
		} // while
	} // while (more)
	xGravity /= area;
	yGravity /= area;
	return area;
} // labelIterInfo

//  func: labels all pixels of one blob which starts at position (row,col) with blobNr. 
//  return_value: area of the blob 
//  Evaluation: This function uses a recursive algorithm which has the advantage that it is easy and trasparent.
//        The disadvantagae however is that it claims a lot of spacee on the stack. I.e. every found 
//        pixel results in a function call which in case of large blobs causes a stack overflow. 
int labelRecursive( Mat& admin, int row, int col, int blobNr ) {
	int area = 0;
	// bij waarde -1 is het pixel nog niet bezocht
	if ( getEntryImage( admin, row, col ) == -1 ) {
		//cout << "(row,col) = " << "(" << row << "," << col << ")" << endl;
		// pixel labelen met het volgnummer van de blob
		setEntryImage( admin, row, col, blobNr );
		area = 1;

		// alle pixels rondom huidige pixel bezoeken
		// (row-1,col-1) (row-1,col ) (row-1,col+1) 
		// (row  ,col-1) (row,  col ) (row  ,col+1)
		// (row+1,col-1) (row,  col ) (row+1,col+1)
		area += labelRecursive( admin, row - 1, col, blobNr );
		area += labelRecursive( admin, row - 1, col + 1, blobNr );
		area += labelRecursive( admin, row, col + 1, blobNr );
		area += labelRecursive( admin, row + 1, col + 1, blobNr );
		area += labelRecursive( admin, row, col, blobNr );
		area += labelRecursive( admin, row + 1, col - 1, blobNr );
		area += labelRecursive( admin, row, col - 1, blobNr );
		area += labelRecursive( admin, row - 1, col - 1, blobNr );
	}
	return area;
} // label

// func: retrieves a labeledImage from the labeling administration
// pre : admin is contains labeled pixels with neighbour number information. 
// post: labeledImage: binary 8-connected pixels with value 1 in binaryImage are 
//       labeled with the number of the object they belong to.
void retrieveLabeledImage( const Mat& admin, Mat& labeledImage ) {
	labeledImage = Mat_<_int16>( admin.rows - 2, admin.cols - 2 );

	for ( int row = 1; row < admin.rows - 1; row++ ) {
		for ( int col = 1; col < admin.cols - 1; col++ ) {
			setEntryImage( labeledImage, row - 1, col - 1,
			               getEntryImage( admin, row, col ) / 10 );
		}
	}
} // retrieveLabeledImage

// func: labeling of all blobs in a binary image
// pre : binaryImage has depth 16 bits signed int. Contains only values 0 and 1.
// post: labeledImage: binary 8-connected pixels with value 1 in binaryImage are 
//       labeled with the number of the object they belong to.
// return_value: the total number of objects.  
int labelBLOBs( Mat binaryImage, Mat& labeledImage ) {

	// admin contains the administration of the recursive process.
	// meaning of the entry values:
	// -1: a "1" which is not visited yet. Changes to 1 when visited.
	//  0: always a "0"
	//  1, 2, 3,... : a "1" which is visited and is labeled with the object number.
	Mat admin = makeAdmin( binaryImage );
	int row = 1;
	int col = 1;

	// init volgnummer
	int blobNr = 0;

	// label alle BLOBs met een volgnummer
	while ( ( row > 0 ) & ( row < ( admin.rows - 1 ) ) &
		( col > 0 ) & ( col < ( admin.cols - 1 ) ) )
		if ( findNextBlob( admin, row, col ) )
			labelIter( admin, row, col, ++blobNr );

	retrieveLabeledImage( admin, labeledImage );

	// laatste volgnummer is gelijk aan het aantal gevonden blobs
	return blobNr;
} // labelBLOBs

// func: removes a BLOB from the labeling administration
// pre: (posx,posy) is the position of the BLOB, blobNr the number
//      of the blob to be removed.
void removeBLOB( Mat& admin, int blobNr ) {
	_int16 value;
	for ( int row = 1; row < admin.rows - 2; row++ )
		for ( int col = 1; col < admin.cols - 2; col++ ) {
			value = getEntryImage( admin, row, col );
			while ( value > 10 )
				value /= 10;
			if ( value == blobNr )
				setEntryImage( admin, row, col, 0 );
		}
} // removeBLOB

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
int labelBLOBsInfo( Mat binaryImage, Mat& labeledImage,
                    vector<Point2d *>& firstpixelVec, vector<Point2d *>& posVec,
                    vector<int>& areaVec,
                    int threshAreaMin, int threshAreaMax ) {

	// admin contains the administration of the recursive process.
	// meaning of the entry values:
	// -1: a "1" which is not visited yet. 
	//  0: always a "0"
	//  1, 2, 3,... : a "1" which is visited and is labeled with the object number.
	Mat admin = makeAdmin( binaryImage );
	int row = 1;
	int col = 1;

	// init volgnummer
	int blobNr = 0;
	int area;
	int xGravity, yGravity;
	// label alle BLOBs met een volgnummer
	while ( ( row > 0 ) & ( row < ( admin.rows - 1 ) ) &
		( col > 0 ) & ( col < ( admin.cols - 1 ) ) )

		if ( findNextBlob( admin, row, col ) ) {

			area = labelIterInfo( admin, row, col, ++blobNr, xGravity, yGravity );

			if ( ( area >= threshAreaMin ) & ( area <= threshAreaMax ) ) {
				firstpixelVec.push_back( new Point2d( row - 1, col - 1 ) );
				posVec.push_back( new Point2d( xGravity - 1, yGravity - 1 ) );
				areaVec.push_back( area );
			} else
				removeBLOB( admin, blobNr-- );
		}

	retrieveLabeledImage( admin, labeledImage );

	// laatste volgnummer is gelijk aan het aantal gevonden blobs
	return blobNr;
} // labelBLOBsInfo

/*BEGIN********************************************** BACK PROPAGATION NEURAL NETWORK ****************************************************************/

// TRAININGSET:  I0 because of bias V0 
//
// setnr     I0     I1     I2    I3    I4    O1   O2
//   1	     1.0    0.4   -0.7   0.1   0.71  0.0  0.0
//   2       1.0    0.3   -0.5   0.05  0.34  0.0  0.0
//   3       1.0    0.6    0.1   0.3   0.12  0.0  1.0
//   4       1.0    0.2    0.4   0.25  0.34  0.0  1.0
//   5		 1.0   -0.2    0.12  0.56  1.0   1.0  0.0
//   6		 1.0	0.1   -0.34  0.12  0.56  1.0  0.0
//   7		 1.0   -0.6    0.12  0.56  1.0   1.0  1.0
//   8		 1.0	0.56  -0.2   0.12  0.56  1.0  1.0

void loadTrainingSet1( Mat& ITset, Mat& OTset ) {

	// input of trainingset
	// remark: nummber of columns == number of inputneurons of the BPN
	ITset = ( Mat_<double>( 8, 5 ) <<
		1, 0.4, -0.7, 0.1, 0.71,
		1, 0.3, -0.5, 0.05, 0.34,
		1, 0.6, 0.1, 0.3, 0.12,
		1, 0.2, 0.4, 0.25, 0.34,
		1, -0.2, 0.12, 0.56, 1.0,
		1, 0.1, -0.34, 0.12, 0.56,
		1, 0.6, 0.12, 0.56, 1.0,
		1, 0.56, -0.2, 0.12, 0.56 );

	// output of trainingset
	// remark: nummber of columns == number of outputneurons of the BPN
	OTset = ( Mat_<double>( 8, 2 ) <<
		0, 0,
		0, 0,
		0, 1,
		0, 1,
		1, 0,
		1, 0,
		1, 1,
		1, 1 );
} // loadTestTrainingSet1

// TRAININGSET binary function O1 = (I1 OR I2) AND I3 
// without bias
// setnr    I1   I2    I3   O1   
//   1	     0    0    0    0 	
//   2       0    0    1    0 
//   3       0    1    0    0              
//   4       0    1    1    1
//   5	     1    0    0    0 	
//   6       1    0    1    1 
//   7       1    1    0    0
//   8       1    1    1    1
void loadBinaryTrainingSet1( Mat& ITset, Mat& OTset ) {

	// input of trainingset (without bias)
	// remark: nummber of columns == number of inputneurons of the BPN
	ITset = ( Mat_<double>( 8, 3 ) <<
		0, 0, 0,
		0, 0, 1,
		0, 1, 0,
		0, 1, 1,
		1, 0, 0,
		1, 0, 1,
		1, 1, 0,
		1, 1, 1 );

	// output of trainingset
	// remark: nummber of columns == number of outputneurons of the BPN
	OTset = ( Mat_<double>( 8, 1 ) <<
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		1 );

} // loadBinaryTrainingSet1

// func: Initialization of the (1) weigthmatrices V0 and W0 and (2) of the delta matrices dV0 and dW0. 
// pre: inputNeurons, hiddenNeurons and outputNeurons define the Neural Network. 
//      From this numbers the dimensions of the weightmatrices can be determined.
// post: V0 and W0 have random values between 0.1 and 0.9
void initializeBPN( int inputNeurons, int hiddenNeurons, int outputNeurons,
                    Mat& V0, Mat& dV0, Mat& W0, Mat& dW0 ) {

	// Instellen van alle weegfactoren met een random waarde
	V0 = Mat_<double>( inputNeurons, hiddenNeurons );
	W0 = Mat_<double>( hiddenNeurons, outputNeurons );
	setRandomValue( V0, 0.1, 0.9 );
	setRandomValue( W0, 0.1, 0.9 );

	// Initiele aanpassing van de weegfactoren W
	dV0 = Mat_<double>( inputNeurons, hiddenNeurons );
	dW0 = Mat_<double>( hiddenNeurons, outputNeurons );
	setValue( dV0, 0 );
	setValue( dW0, 0 );
} // initializeBPN

// Test of a BPN with all values defined explicitly 
void testBPN( Mat& IT, Mat& OT, Mat& V0, Mat& dV0, Mat& W0, Mat& dW0 ) {

	// input of trainingset
	// remark: number of columns == number of inputneurons of the BPN
	IT = ( Mat_<double>( 5, 2 ) <<
		0.4, -0.7,
		0.3, -0.5,
		0.6, 0.1,
		0.2, 0.4,
		0.1, -0.2 );

	// output of trainingset
	// remark: nummber of columns == number of outputneurons of the BPN
	OT = ( Mat_<double>( 5, 1 ) <<
		0.1,
		0.05,
		0.3,
		0.25,
		0.12 );

	// STEP2:  Initializing the weights
	V0 = ( Mat_<double>( 2, 2 ) <<
		0.1, 0.4,
		-0.2, 0.2 );

	W0 = ( Mat_<double>( 2, 1 ) <<
		0.2,
		-0.5 );

	// Initiele aanpassing van de weegfactoren W
	dW0 = ( Mat_<double>( 2, 1 ) <<
		0.0,
		0.0 );

	// Initiele aanpassing van de weegfactoren V
	dV0 = ( Mat_<double>( 2, 2 ) <<
		0.0, 0.0,
		0.0, 0.0 );

} //  testBPN

// func: Given an inputvector of the inputlayer and a weightmatrix V calculates the outputvector of the hiddenlayer
// pre: II is input of the inputlayer. V = matrix with weightfactors between inputlayer and the hiddenlayer.
// post: OH is the outputvector of the hidden layer
void calculateOutputHiddenLayer( Mat II, Mat V, Mat& OH ) {

	// STEP1: Output inputlayer := Input inputlayer    
	Mat OI;
	II.copyTo( OI );

	// STEP2:  Initializing the weights, already done, see input of this function

	// STEP3: Calculate input of the hiddenlayer, i.e. IH = V0transposed * OI
	Mat Vtr = transpose( V );
	Mat IH = multiply( Vtr, OI );

	// STEP4: Calculate output of the hiddenlayer, i.e. OH(i) = 1/(1+EXP(-IH(i)))   
	int hiddenNeurons = V.cols;
	OH = Mat_<double>( hiddenNeurons, 1 );
	for ( int row = 0; row < hiddenNeurons; row++ )
		setEntry( OH, row, 0, 1 / ( 1 + exp( -getEntry( IH, row, 0 ) ) ) );

} // calculateOutputHiddenLayer

// func: Given the outputvector of the hiddenlayer and a weigthmatrix W calculates the outputvector of the outputlayer
// pre: OH is the outputvector of the hiddenlayer. W = matrix with weightfactors between hiddenlayer and the outputlayer.
// post: OO is the outputvector of the output layer
void calculateOutputBPN( Mat OH, Mat W, Mat& OO ) {

	// STEP5: Calculate input of the outputlayer, i.e. IO = W0transposed * OH
	Mat Wtr = transpose( W );
	Mat IO = multiply( Wtr, OH );

	// STEP6: Calculate output of the outputlayer, i.e. OO(i) = 1/(1+EXP(-IO(i)))
	int outputNeurons = W.cols;
	OO = Mat_<double>( outputNeurons, 1 );
	for ( int row = 0; row < outputNeurons; row++ )
		setEntry( OO, row, 0, 1 / ( 1 + exp( -getEntry( IO, row, 0 ) ) ) );

} // calculateOutputBPN

// func: Calculates the total error Error = 1/2*Sigma(OTi-OOi)^2. 
//       OTi is the expected output according to the trainingvector i
//       OOi is the calculated output from the current neural network of the traininngvector i
// pre: OO is the outputvector of the outputlayer. OT is the expected outputvector from the trainingset 
// post: OO is the outputvector of the output layer
void calculateOutputBPNError( Mat OO, Mat OT, double& outputError ) {

	// STEP7: Calculate the error, i.e. Error = 1/2*Sigma(TOi-OOi)^2
	double sumSqrErr = 0, diff = 0;
	for ( int row = 0; row < OT.rows; row++ ) {
		diff = getEntry( OT, row, 0 ) - getEntry( OO, row, 0 );
		sumSqrErr += ( diff * diff );
	}
	outputError = 0.5 * sumSqrErr;

} // calculateOutputBPNError

void adaptVW( Mat OT, Mat OO, Mat OH, Mat OI, Mat W0, Mat dW0, Mat V0, Mat dV0, Mat& W, Mat& V,
              double ALPHA, double ETHA ) {

	/*BEGIN*** AANPASSING VAN DE WEEGFACTOREN W ****/

	// STEP8: 
	// E = 1/2 Sigma(OOi - di)^2  ==> dE/dOO = Sigma(OOi - di)
	// dE/dIO = dE/dOO * dOO/dIO = Sigma((OOi - Ti) * OOi * (1 - OOi))
	// Here: d = dE/dIO = (T-OO) * OO * (1 - OO) 
	Mat OOerror = Mat_<double>( OT.rows, 1 );
	OOerror = OT - OO;

	Mat d = Mat_<double>( OT.rows, 1 );
	double di;
	for ( int row = 0; row < OT.rows; row++ ) {
		di = ( getEntry( OT, row, 0 ) - getEntry( OO, row, 0 ) ) * getEntry( OO, row, 0 ) * ( 1 - getEntry( OO, row, 0 ) );
		setEntry( d, row, 0, di );
	}

	// Y = OH * d 
	Mat dtr = transpose( d );
	Mat Y = Mat_<double>( OH.rows, OT.rows );
	Y = multiply( OH, dtr ); // OH = mx1 ; d = nx1 ; dtr = 1xn

	// STEP9: dW1 = alpha * dW0 +  etha * Y // assume etha = 0.6 //never just assume, Jan
	Mat dW = Mat_<double>( OH.rows, OT.rows );
	dW = ALPHA * dW0 + ETHA * Y;

	/*END*** AANPASSING VAN DE WEEGFACTOREN W ****/

	/*BEGIN*** AANPASSING VAN DE WEEGFACTOREN V ****/

	// STEP10: OHerror = W0 * d
	Mat OHerror = Mat_<double>( OH.rows, 1 );
	OHerror = W0 * d;

	// STEP11: 
	// d = dE/dIO = OOerror * OO * (1 - OO)  // OOError = TO - OO
	// d*= dE/dIH = OHerror * OH * (1 - OH)  // OHerror = W0 * d 
	Mat dstar = Mat_<double>( OH.rows, 1 );
	double dstari;
	for ( int row = 0; row < OH.rows; row++ ) {
		dstari = getEntry( OHerror, row, 0 ) * getEntry( OH, row, 0 ) * ( 1 - getEntry( OH, row, 0 ) );
		setEntry( dstar, row, 0, dstari );
	}

	// STEP12:
	// X = OI * dstar
	Mat dstartr = transpose( dstar );
	Mat X = Mat_<double>( OI.rows, OH.rows );
	X = OI * dstartr;

	// STEP13: dV1 = ALPHA * dV0 +  ETHA * X // assume etha = 0.6 
	Mat dV;
	dV = ALPHA * dV0 + ETHA * X;
	/*END*** AANPASSING VAN DE WEEGFACTOREN V ****/

	/* Update van de matrices met gewichtsfactoren */

	// STEP14:
	V = Mat_<double>( V0.rows, V0.cols );
	W = Mat_<double>( W0.rows, W0.cols );
	V = V0 + dV;
	W = W0 + dW;

}; // adaptVW

Mat BPN( Mat II, Mat V, Mat W ) {
	Mat OH, OO;
	calculateOutputHiddenLayer( II, V, OH );
	calculateOutputBPN( OH, W, OO );
	return OO;
} // BPN

/*END********************************************** BACK PROPAGATION NEURAL NETWORK ****************************************************************/

int allContours( Mat binaryImage, vector<vector<Point>>& contourVecVec ) {
	//B's and C's for the contour tracking algoritm
	Point b0, oldB, newB, c0, oldC, newC;
	bool firstPixel = true;

	vector<Point2d*> firstPixelVec, *posVec = new vector<Point2d*>;
	vector<int>* areaVec = new vector<int>;

	//Temporary Mat objects for image processing like blob labeling. Will be deleted to save some memory
	Mat *labeledImage = new Mat(), *blobImage = new Mat();
	binaryImage.convertTo( *blobImage, CV_16S );
	int blobs = labelBLOBsInfo( *blobImage, *labeledImage, firstPixelVec, *posVec, *areaVec );

	//Cleaning up junk used by the library
	delete labeledImage, blobImage, posVec, areaVec;

	//For each first pixel per blob
	for ( int i = 0; i < firstPixelVec.size(); i++ ) {
		vector<Point> contour;

		//Switch x and y values, since they were returned wrong by labelBLOBsInfo()
		const int x = firstPixelVec[i]->x;
		const int y = firstPixelVec[i]->y;
		firstPixelVec[i]->y = x;
		firstPixelVec[i]->x = y;

		//First b0 and c0 pixel for the algoritm
		b0 = Point( firstPixelVec[i]->x, firstPixelVec[i]->y );
		double oldCx = firstPixelVec[i]->x;
		oldCx--;
		c0 = Point( oldCx, firstPixelVec[i]->y );

		//Setting first values to start the algoritm
		newB = b0;
		newC = c0;
		contour.push_back( b0 );

		//While there are still pixels in the contour
		while ( newB != b0 || firstPixel ) {
			oldB = newB;

			//Counter is used to check if we can find a new place for newB. It stops checking forever with single pixel blobs.
			int counter = 0;

			//While there is no new pixel with value 1 found
			while ( (int)binaryImage.at<uchar>( Point( newC.x, newC.y ) ) == 0 && counter <= 9 ) {
				Point e = oldB - newC;
				oldC = newC;

				//The Moore algoritm
				if ( e == Point( -1, -1 ) || e == Point( 0, -1 ) ) {
					//newC (right) under oldB
					newC.x--;
				} else if ( e == Point( 1, -1 ) || e == Point( 1, 0 ) ) {
					//newC left (under) oldB
					newC.y--;
				} else if ( e == Point( 1, 1 ) || e == Point( 0, 1 ) ) {
					//newC (left) above oldB
					newC.x++;
				} else if ( e == Point( -1, 1 ) || e == Point( -1, 0 ) ) {
					//newC right (above) oldB
					newC.y++;
				}
				counter++;
			}
			firstPixel = false;

			//Set the new values for the next contour pixel and save it
			newB = newC;
			newC = oldC;
			contour.push_back( newB );
		}
		if ( contour.size() >= 150 )
			contourVecVec.push_back( contour );
		firstPixel = true;
	}
	return contourVecVec.size();
}

double bendingEnergy(vector<Point>& contourVec ) {
	//Start met een bending Energy van 0.
	double bendingEnergy = 0;
	//Check of de de lijst met contouren groter is dan 0 zodat de eerste positie opgeslagen kan worden.
	if ( contourVec.size() < 1 )
		return bendingEnergy;
	Point lastPos = contourVec[0];
	//Voeg de eerste waarde toe aan de lijst met contouren zodat ook de eerste en laatste waarde van het contour vergeleken kan worden.
	contourVec.push_back( lastPos );
	//Maakt een lege lijst aan voor de chain code.
	vector<int> chainCode = vector<int>();

	//Loop door de contouren heen en vergelijk elke hoek met de laatste hoek voor het genereren van de chain code.
	for ( int i = 1; i < contourVec.size(); i++ ) {
		Point currentPos = contourVec[i];

		//Check of de x van de hoek groter is dan dat van de vorige en de y gelijk is. Zo ja is de code van deze hoek 0.
		//Dit wordt gecheckt voor elke andere hoek (en dus elke else if statement) en er wordt dan de bijbehorende code toegevoegd.
		if ( currentPos.x > lastPos.x && currentPos.y == lastPos.y ) {
			chainCode.push_back( 0 );
		} else if ( currentPos.x < lastPos.x && currentPos.y == lastPos.y ) {
			chainCode.push_back( 4 );
		} else if ( currentPos.y < lastPos.y && currentPos.x > lastPos.x ) {
			chainCode.push_back( 1 );
		} else if ( currentPos.y < lastPos.y && currentPos.x < lastPos.x ) {
			chainCode.push_back( 3 );
		} else if ( currentPos.y > lastPos.y && currentPos.x < lastPos.x ) {
			chainCode.push_back( 5 );
		} else if ( currentPos.y > lastPos.y && currentPos.x > lastPos.x ) {
			chainCode.push_back( 7 );
		} else if ( currentPos.y < lastPos.y ) {
			chainCode.push_back( 2 );
		} else {
			chainCode.push_back( 6 );
		}
		//Vervang de laatste positie met de huidige positie zodat deze gebruikt kan worden bij de volgende vergelijking.
		lastPos = currentPos;
	}
	//Check weer of er een vergelijking gemaakt kan worden met twee waardes
	if ( chainCode.size() < 1 )
		return bendingEnergy;
	int lastAngle = chainCode[0];
	//Voeg de laatste waarde toe voor vergelijking voor de bending energy.
	chainCode.push_back( lastAngle );
	//Loop de chain code door om de onderlinge bending energy te berekenen.
	for ( int i = 1; i < chainCode.size(); i++ ) {
		//Bereken een draaiing naar beide kanten om de zo kleinst mogelijke hoek te gebruiken.
		int angleDif1 = ( chainCode[i] - lastAngle );
		int angleDif2 = ( lastAngle - chainCode[i] );
		//Maak het verschil positief wanneer deze negatief is.
		if ( angleDif1 < 0 )
			angleDif1 += 8;
		else if ( angleDif2 < 0 )
			angleDif2 += 8;
		//Check welke waarde het kleinst is en geeft dit terug.
		if ( angleDif1 < angleDif2 )
			bendingEnergy += angleDif1;
		else
			bendingEnergy += angleDif2;
		lastAngle = chainCode[i];
	}
	return bendingEnergy;
}

double pythagoras( const double x, const double y ) {
	return sqrtf( pow( x, 2 ) + pow( y, 2 ) );
}
