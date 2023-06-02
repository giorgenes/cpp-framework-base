#include <Magick++.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>

namespace mgk = ::Magick;

#define ST_BLACK 1
#define ST_WHITE 0

class Bar {
	public:
		int color;
		double width;

		Bar(int c, double w) 
			: color(c), width(w) {
		}
};

class BarCode {
	private:
		std::vector<Bar> __bars;
		int __st;
		int __n;
	public:
		void feed(double);
		void find_codes();

		BarCode();
};

BarCode::BarCode()
	: __st(ST_WHITE), __n(0)
{
}


void BarCode::feed(double blackness)
{
	if(blackness >= 0.9) {
		if(__st == ST_WHITE) {
			if(__n > 0) {
				__bars.push_back(Bar(__st, __n));
			}
			__n = 0;
			__st = ST_BLACK;
		}
		else {
			__n++;
		}
	}
	else {
		if(__st == ST_WHITE) {
			__n++;
		}
		else {
			if(__n > 0) {
				__bars.push_back(Bar(__st, __n));
			}
			__n = 0;
			__st = ST_WHITE;
		}
	}
}

class Code5 {
	public:
		double width;
		int idx;

		Code5(double w, int i) : width(w), idx(i) {
		}
};

bool operator < (const Code5 &a, const Code5& b)
{
	return a.width < b.width;
}


void BarCode::find_codes()
{
	double avgw = 0;
	unsigned int i;
	double min = 1.0;
	double max = 0;
	unsigned int n = 0;
	//double code[5];
	std::vector<Code5> code;
	unsigned int v;
	int bits[5];
	char buf[10];
	char strcode[10];
	int j;

	i = 0;
	int st = 0;
	while(i<__bars.size()) {
		if(__bars[i].color == ST_BLACK || st == 1) {
			if(i + 5 >= __bars.size()) {
				i++;
				continue;
			}
			code.clear();
			code.push_back(Code5(__bars[i+0].width, 0));
			code.push_back(Code5(__bars[i+1].width, 1));
			code.push_back(Code5(__bars[i+2].width, 2));
			code.push_back(Code5(__bars[i+3].width, 3));
			code.push_back(Code5(__bars[i+4].width, 4));

			sort(code.begin(), code.end());
			
			for(j=0; j<5; j++) {
				bits[j] = 1;
			}
			bits[code[3].idx] = 2;
			bits[code[4].idx] = 2;
			
			int b = -1;
			v = 0;
			int mask;
			for(j=0; j<5; j++) {
				fprintf(stderr, "%d: %d ", __bars[i+j].color, bits[j]);
			}
			fprintf(stderr, "\n");
			for(j=4; j>=0; j--) {
				b += bits[j];
				mask = bits[j] == 1 ? 1 : 3;
				if(__bars[i+j].color == ST_BLACK) {
					fprintf(stderr, "%d | %d << %d\n", v, mask, b);
					v |= mask << b - (bits[j] > 1 ? 1 : 0);
				}
			}

			fprintf(stderr, "%-8.3x\n", v);
#if 0
			code[0] = __bars[i+0].width / __bars[i+1].width;
			code[1] = __bars[i+1].width / __bars[i+2].width;
			code[2] = __bars[i+2].width / __bars[i+3].width;
			code[3] = __bars[i+3].width / __bars[i+4].width;
			code[4] = __bars[i+4].width / __bars[i+0].width;

			qsort();
#endif
#if 0
			fprintf(stderr, "%f %f %f %f %f\n", 
					__bars[i+0].width,
					__bars[i+1].width,
					__bars[i+2].width,
					__bars[i+3].width,
					__bars[i+4].width);
			fprintf(stderr, "%2.5f %2.5f %2.5f %2.5f %2.5f\n\n", 
					code[0].width,
					code[1].width,
					code[2].width,
					code[3].width,
					code[4].width);
#endif

			i += 5;
			if(v == 0x59) {
				if(st == 0) {
					st = 1;
				}
				else {
					st = 0;
				}
			}
		}
		else {
			i++;
		}
	}
}



int main(int argc, char** argv)
{
	try {
		mgk::Image img;
		img.read(argv[1]);
		BarCode scanner;

		unsigned int j;

		unsigned int mid = img.rows() / 2;
		double blackness;
		for(j=0; j<img.columns(); j++) {
			mgk::ColorGray color = img.pixelColor(j, mid);
			blackness = (color.shade() - 1) * -1;
			fprintf(stdout, "%3.3f\n", blackness);
			scanner.feed(blackness);
		}
		scanner.find_codes();
	}
	catch(...) {
		fprintf(stderr, "erro\n");
	}
}

