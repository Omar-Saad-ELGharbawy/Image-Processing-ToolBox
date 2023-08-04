#include <math.h>
#include <opencv2\opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

#define MS_MAX_NUM_CONVERGENCE_STEPS	10										// up to 10 steps are for convergence
#define MS_MEAN_SHIFT_TOL_COLOR			0.3										// minimum mean color shift change
#define MS_MEAN_SHIFT_TOL_SPATIAL		0.3										// minimum mean spatial shift change

struct Pixel
{
    	float x;			// Spatial value
		float y;			// Spatial value
		float l;			// Lab value
		float a;			// Lab value
		float b;			// Lab value
};

void meanShift_setPoint(Pixel* p, float px, float py, float pl, float pa, float pb);
void meanShift_PointLab(Pixel* pt);
void meanShift_copyPoint_F2S(Pixel* p1, Pixel* p2);
float meanShift_ColorDistance(Pixel Pt1, Pixel Pt2);
float meanShift_SpatialDistance(Pixel Pt1, Pixel Pt2);
void meanShift_PointScale(Pixel* p, float scale);
void meanShift_PointAccum(Pixel* mainPt, Pixel Pt);
void meanShift_PointRGB(Pixel* p);
Mat meanShift_Segmentation(Mat Img, float distanceBandwidth, float colorBandwidth);