#include "ColorInfo.h"


ColorInfo::ColorInfo(){

	

}


ColorInfo::ColorInfo(vector<string> Color, vector<float> proportion){

	_Color.assign(Color.begin(), Color.end());
	_proportion.assign(proportion.begin(), proportion.end());


}