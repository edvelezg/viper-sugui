#include <iostream>
#include "BitmapAllocation.h"
/**
 * the goal of this little program is to print out how we "smartly" adapt the k-of-n
 * encoding to small dimensions.
 */


int main(int params, char ** args) {
	std::ios::sync_with_stdio (false);
	KofNBitmapBudget v1ofN(1,true);
	KofNBitmapBudget v2ofN(2,true);
	KofNBitmapBudget v3ofN(3,true);
	KofNBitmapBudget v4ofN(4,true);
	KofNBitmapBudget v5ofN(5,true);
	for(uint n = 1; n<1000;++n) {
		uint bitmap1 = v1ofN.howManyBitmaps(n);
		uint actualk1 = v1ofN.getK(n,bitmap1);
		uint bitmap2 = v2ofN.howManyBitmaps(n);
		uint actualk2 = v2ofN.getK(n,bitmap2);
		uint bitmap3 = v3ofN.howManyBitmaps(n);
		uint actualk3 = v3ofN.getK(n,bitmap3);
		uint bitmap4 = v4ofN.howManyBitmaps(n);
		uint actualk4 = v4ofN.getK(n,bitmap4);
		uint bitmap5 = v5ofN.howManyBitmaps(n);
		uint actualk5 = v5ofN.getK(n,bitmap5);
		cout<<n <<" " <<bitmap1 << " "<<actualk1 << " " <<bitmap2 
		<< " "<<actualk2 <<" " <<bitmap3 << " "<<actualk3 <<" "<<bitmap4 << " "<<actualk4
		<<" "<<bitmap5 << " "<<actualk5<<endl;
	}
}
