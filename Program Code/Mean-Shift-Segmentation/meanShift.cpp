#include "meanShift.hpp"


const int dxdy[][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};	// Region Growing

void meanShift_setPoint(Pixel* p, float px, float py, float pl, float pa, float pb){
    p->x = px;
    p->y = py;
    p->l = pl;
    p->a = pa;
    p->b = pb;
}

void meanShift_PointLab(Pixel* pt){
	pt->l = pt->l * 100 / 255;
	pt->a = pt->a - 128;
	pt->b = pt->b - 128;
}

void meanShift_copyPoint_F2S(Pixel* p1, Pixel* p2){
    // Function to copy colors of the second point to the first point
    // p2 --> p1
	p1->x = p2->x;
    p1->y = p2->y;
    p1->l = p2->l;
    p1->a = p2->a;
    p1->b = p2->b;
}

float meanShift_ColorDistance(Pixel Pt1, Pixel Pt2){
	return sqrt((Pt1.l - Pt2.l) * (Pt1.l - Pt2.l) + (Pt1.a - Pt2.a) * (Pt1.a - Pt2.a) + (Pt1.b - Pt2.b) * (Pt1.b - Pt2.b));
}

float meanShift_SpatialDistance(Pixel Pt1, Pixel Pt2){
	return sqrt((Pt1.x - Pt2.x) * (Pt1.x - Pt2.x) + (Pt1.y - Pt2.y) * (Pt1.y - Pt2.y));
}

void meanShift_PointScale(Pixel* p, float scale){
	p->x *= scale;
	p->y *= scale;
    p->l *= scale;
    p->a *= scale;
    p->b *= scale;
}

void meanShift_PointAccum(Pixel* mainPt, Pixel Pt){
	mainPt->x += Pt.x;
    mainPt->y += Pt.y;
    mainPt->l += Pt.l;
    mainPt->a += Pt.a;
    mainPt->b += Pt.b;
}

void meanShift_PointRGB(Pixel* p){
	p->l = p->l * 255 / 100;
	p->a = p->a + 128;
	p->b = p->b + 128;
}

Mat meanShift_Segmentation(Mat Img, float distanceBandwidth, float colorBandwidth){

//Image clone to an output image
	Mat ImgOut = Img.clone();
	cvtColor(ImgOut, ImgOut, COLOR_BGR2Lab); 

	int ROWS = Img.rows;
	int COLS = Img.cols;
    vector<Mat> IMGChannels(3);
	split(ImgOut, IMGChannels);

	Pixel PtCur;
	Pixel PtPrev;
	Pixel PtSum;
	Pixel Pt;

	int Left;
	int Right;
	int Top;
	int Bottom;
	int NumPts;					// number of points in a hypersphere
	int step;
	
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			Left = (j - distanceBandwidth) > 0 ? (j - distanceBandwidth) : 0;
			Right = (j + distanceBandwidth) < COLS ? (j + distanceBandwidth) : COLS;
			Top = (i - distanceBandwidth) > 0 ? (i - distanceBandwidth) : 0;
			Bottom = (i + distanceBandwidth) < ROWS ? (i + distanceBandwidth) : ROWS;
			meanShift_setPoint(&PtCur, i, j, (float)IMGChannels[0].at<uchar>(i, j), (float)IMGChannels[1].at<uchar>(i, j), (float)IMGChannels[2].at<uchar>(i, j));
			meanShift_PointLab(&PtCur);
			step = 0;
			do{
				meanShift_copyPoint_F2S(&PtPrev, &PtCur);
				meanShift_setPoint(&PtSum, 0, 0, 0, 0, 0);
				NumPts = 0;
				for(int hx = Top; hx < Bottom; hx++){
					for(int hy = Left; hy < Right; hy++){
						
						meanShift_setPoint(&Pt, hx, hy, (float)IMGChannels[0].at<uchar>(hx, hy), (float)IMGChannels[1].at<uchar>(hx, hy), (float)IMGChannels[2].at<uchar>(hx, hy));
						meanShift_PointLab(&Pt);

						if(meanShift_ColorDistance(Pt, PtCur) < colorBandwidth){
							meanShift_PointAccum(&PtSum, Pt);
							NumPts++;
						}
					}
				}
				meanShift_PointScale(&PtSum, 1.0 / NumPts);
				meanShift_copyPoint_F2S(&PtCur, &PtSum);
				step++;
			}while((meanShift_ColorDistance(PtCur, PtPrev) > MS_MEAN_SHIFT_TOL_COLOR) && (meanShift_SpatialDistance(PtCur, PtPrev) > MS_MEAN_SHIFT_TOL_SPATIAL) && (step < MS_MAX_NUM_CONVERGENCE_STEPS));
			
			meanShift_PointRGB(&PtCur);
			ImgOut.at<Vec3b>(i, j) = Vec3b(PtCur.l, PtCur.a, PtCur.b);
		}
	}
//--------------------------------------------------------------------

//----------------------- Segmentation ------------------------------
	int label = -1;					// Label number
	vector<float> Mode(ROWS * COLS * 3);					// Store the Lab color of each region
	vector<int> MemberModeCount(ROWS * COLS, 0);				// Store the number of each region
	split(ImgOut, IMGChannels);
	// Label for each point
	vector<vector<int>> Labels(ROWS);;

	// Initialization
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			Labels[i].push_back(-1);
		}
	}

	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS;j ++){
			// If the point is not being labeled
			if(Labels[i][j] < 0){
				Labels[i][j] = ++label;		// Give it a new label number
				// Get the point
				meanShift_setPoint(&PtCur, i, j, (float)IMGChannels[0].at<uchar>(i, j), (float)IMGChannels[1].at<uchar>(i, j), (float)IMGChannels[2].at<uchar>(i, j));
				meanShift_PointLab(&PtCur);

				// Store each value of Lab
				Mode[label * 3 + 0] = PtCur.l;
				Mode[label * 3 + 1] = PtCur.a;
				Mode[label * 3 + 2] = PtCur.b;
				
				// Region Growing 8 Neighbours
				vector<Pixel> NeighbourPoints;
				NeighbourPoints.push_back(PtCur);
				while(!NeighbourPoints.empty()){
					Pt = NeighbourPoints.back();
					NeighbourPoints.pop_back();

					// Get 8 neighbours
					for(int k = 0; k < 8; k++){
						int hx = Pt.x + dxdy[k][0];
						int hy = Pt.y + dxdy[k][1];
						if((hx >= 0) && (hy >= 0) && (hx < ROWS) && (hy < COLS) && (Labels[hx][hy] < 0)){
							Pixel P;
							meanShift_setPoint(&P, hx, hy, (float)IMGChannels[0].at<uchar>(hx, hy), (float)IMGChannels[1].at<uchar>(hx, hy), (float)IMGChannels[2].at<uchar>(hx, hy));
							meanShift_PointLab(&P);

							// Check the color
							if(meanShift_ColorDistance(PtCur, P) < colorBandwidth){
								// Satisfied the color bandwidth
								Labels[hx][hy] = label;				// Give the same label					
								NeighbourPoints.push_back(P);		// Push it into stack
								MemberModeCount[label]++;			// This region number plus one
								// Sum all color in same region
								Mode[label * 3 + 0] += P.l;
								Mode[label * 3 + 1] += P.a;
								Mode[label * 3 + 2] += P.b;
							}
						}
					}
				}
				MemberModeCount[label]++;							// Count the point itself
				Mode[label * 3 + 0] /= MemberModeCount[label];		// Get average color
				Mode[label * 3 + 1] /= MemberModeCount[label];
				Mode[label * 3 + 2] /= MemberModeCount[label];
			}
		}
	}	
	// Get result image from Mode array
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			label = Labels[i][j];
			float l = Mode[label * 3 + 0];
			float a = Mode[label * 3 + 1];
			float b = Mode[label * 3 + 2];
			Pixel Pixel;
			meanShift_setPoint(&Pixel, i, j, l, a, b);
			meanShift_PointRGB(&Pixel);
			ImgOut.at<Vec3b>(i, j) = Vec3b(Pixel.l, Pixel.a, Pixel.b);
		}
	}
	cvtColor(ImgOut, ImgOut, COLOR_Lab2BGR);
	return ImgOut;
}

