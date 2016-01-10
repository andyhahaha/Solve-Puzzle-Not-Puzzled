#include "ColorInfo.h"


ColorInfo::ColorInfo(){

	

}


ColorInfo::ColorInfo(vector<float> colorPrortion){

	_colorPrortion.assign(colorPrortion.begin(), colorPrortion.end());

}


void ColorInfo::ShowColorInfo(){
	int i;
	float total=0.0;
	for (i = 0; i < _colorPrortion.size(); i++){
		printf("%2d", i);
		cout << "   " << _colorPrortion[i] << endl;
		total += _colorPrortion[i];
	}
	cout << "total = " << total << endl;

}


float ColorInfo::get_colorProportion(int color){

	return _colorPrortion[color];
}
