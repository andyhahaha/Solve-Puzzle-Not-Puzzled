#include "ColorInfo.h"


ColorInfo::ColorInfo(){

	

}


ColorInfo::ColorInfo(vector<float> colorPrortion){

	_colorPrortion.assign(colorPrortion.begin(), colorPrortion.end());

}


void ColorInfo::ShowColorInfo(){
	int i;
	for (i = 0; i < _colorPrortion.size(); i++){
		printf("%2d", i);
		cout << "   " << _colorPrortion[i] << endl;
	}

}