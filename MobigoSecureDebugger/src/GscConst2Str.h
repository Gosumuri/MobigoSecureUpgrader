#pragma once

class  CGscConst2Str
{
public:
	CGscConst2Str(void);
	~CGscConst2Str(void);

	static char*				PatternNum2Str(int nPatternNum);
	static char*				Operation2Str(int nOperation);
	static char*				VendorID2Str(int nVendorID );
};
