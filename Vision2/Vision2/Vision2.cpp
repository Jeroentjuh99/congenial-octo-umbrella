#include "stdafx.h"
#include "deel1.h"
#include "deel2.h"
#include "avansvisionlib.h"

bool USE_NEW_CAL_FILE = true;

int main(int argc, char* argv[]) {
	//deel1 deel1;
	//deel1.deel1Code();
	deel2 deel2;
	deel2.CutTrainingSet("C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\Vision2\\Vision2\\pictures\\monsters.jpg", "monsters");
	//deel2.testEnclosedPixels();

	//deel2 deel2;
	//deel2.calibrate_camera(USE_NEW_CAL_FILE);
}
