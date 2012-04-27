#ifndef RANDH
#define RANDH

#include <string>
using namespace std;

// Mersenne Twister法による乱数生成
class CRand {
	unsigned long mt[624];
	int mti;
public:

	// コンストラクタ
	CRand() : mti(624+1) {}

	// 初期化
	void Init(unsigned long s);
	void Init(unsigned long init_key[], int key_length);
	void Init(string s);

	// [0,0xffffffff]
	unsigned long Int32(void);

	// [0,0x7fffffff]
	long Int31(void);

	// [0,1]
	double Real1(void);

	// [0,1)
	double Real2(void);

	// (0,1)
	double Real3(void);

	// [0,1) 53ビット精度
	double Res53(void);
};

#endif
