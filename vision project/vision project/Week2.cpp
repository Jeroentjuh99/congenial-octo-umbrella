#include "Week2.h"


Week2::Week2()
{
	image = imread("C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\vision project\\vision project\\noise.png", CV_LOAD_IMAGE_COLOR);
}

void Week2::Opgave1()
{

	// Mat object aanmaken voor grijswaarde beeld
	Mat gray_image;

	// Converteren van de ingelezen afbeelding naar een grijswaarde beeld.
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// ROI bepalen
	Mat roi(gray_image, Rect(0, 0, gray_image.cols, gray_image.rows/2));
	Mat roi2(gray_image, Rect(0, gray_image.rows/2, gray_image.cols, gray_image.rows/2));

	/****** Resultaten naar het scherm sturen ******/

	// Grijswaarde beeld tonen op het scherm
	namedWindow("Gray imageUP", CV_WINDOW_AUTOSIZE);
	imshow("Gray imageUP", roi);
	namedWindow("Gray imageDOWN", CV_WINDOW_AUTOSIZE);
	imshow("Gray imageDOWN", roi2);
}

void Week2::Opgave2()
{

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// Grijswaarde afbeelding op het scherm zetten
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	namedWindow("Display Gray window", WINDOW_AUTOSIZE);
	imshow("Display Gray window", gray_image);

	// Wait for a keystroke in the window
	waitKey(0);

	// Eigenschappen van het beeld ophalen en afdrukken op het scherm
	int channels = gray_image.channels();
	int nRows = gray_image.rows;
	int nCols = gray_image.cols * channels;
	cout << "Deze image heeft: " << nRows << " X " << nCols << " pixels" << endl;
	cout << "Het aantal kanalen per pixel is: " << channels << endl;

	// Controleren of de afbeelding een grijswaarde beeld is.
	if (!channels == 1) {
		cout << "NB Dit is geen grijswaarde beeld" << endl;
		cout << "Het programma is hier niet op voorbereid en wordt daarom gestopt.";
		return;
	}

	// Controleren of de matrix als 1 lange continue rij getallen in het geheugen staat
	if (!gray_image.isContinuous())
	{
		cout << "NB Matrix is NOT continuous" << endl;
		cout << "Programma is hier niet op voorbereid en wordt daarom gestopt.";
		return;
	}

	// Pixelcoordinaten inlezen
	cout << "Geef pixel coordinaten x <,> y = ";
	string dummy;
	getline(cin, dummy);

	// invoerstring ontleden in i en j deel
	string istr = dummy.substr(0, dummy.find(','));
	string jstr = dummy.replace(0, dummy.find(',') + 1, "");

	// i en j deel omzetten in een integer 
	int i, j;
	i = atoi(istr.c_str());
	j = atoi(jstr.c_str());

	// pointer naar eerste element van de matrix laten wijzen.
	// NB We gaan ervan uit dat de matrix als 1 lange continue rij getallen in het 
	//    geheugen staat. Dit kan eventueel gecontroleerd worden met de boolean lidfunctie
	//    isContinous. if (gray_image.isContinuous()) .....
	uchar* p;
	p = gray_image.ptr<uchar>(0);

	// pixel met positie (i,j) heeft index i*nCols+j (controleer dat!)
	unsigned int index = i*nCols + j;

	// pixelwaarde afdrukken. Met -0 wordt de char afgedrukt als int
	cout << "pixelwaarde(" << i << "," << j << ") = " << p[index] - 0 << endl;
	getline(cin, dummy);
}

void Week2::Opgave3()
{

	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	std::cout << "iets" << std::endl;

	Mat dst;

	threshold(gray_image, dst, 100, 255, THRESH_TOZERO_INV);

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", dst);
}

void Week2::Opgave4()
{

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// Grijswaarde afbeelding op het scherm zetten
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	namedWindow("Display Gray window", WINDOW_AUTOSIZE);
	imshow("Display Gray window", gray_image);

	Mat dst;
	GaussianBlur(gray_image, dst, Size(5, 5), 0, 0);
	//for (int i = 0; i < ; i++)
	//{
	//	GaussianBlur(dst, dst, Size(5, 5), 0, 0);
	//}
	

	//Mat kernel = Mat::ones(3, 3, CV_32F);
	//Mat result;
	//filter2D(gray_image, result, -1, kernel, Point(-1, -1), 0, BORDER_REPLICATE);

	// De gefilterde afbeelding op het scherm zetten
	cout << "Het grijswaarde beeld is gefilterd." << endl;
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	namedWindow("Display Result filtering window", WINDOW_AUTOSIZE);
	imshow("Display Result filtering window", dst);

	// Wait for a keystroke in the window
	waitKey(0);

	string dummy;
	getline(cin, dummy);
}


Week2::~Week2()
{
}
