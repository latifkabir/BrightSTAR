/****************************************************************************
*
*	Hists.h, Hists.cpp contain classes Plot, PanelPlot and Hists,
*	which aim to help to produce publication-ready, elegant ROOT plots
*	with minimum code. 
*
*	Comments & suggestions are welcome (jan.matousek@cern.ch)
*
*****************************************************************************
*
* Jan Matousek, 2015-10-14
* Edit: 2017-04-04
*/

#ifndef __HISTS__
#define __HISTS__

#include <cmath>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TLatex.h"

#define MAXLINES 10
#define MAXTEXTS 10

/******************************************************************************
* Class Plot represents a set of 1D histograms, graphs and functions, 
* which should be plotted into a single pad. 
* The key features are:
*		*	When a hist, graph etc. is added to the plot, its COPY is made
*			and is manipulated afterwards (the ORIGINALS remain UNCHANGED)
*		*	The default y-axis range is selected taking into account all 
*			hists, graphs etc., their error bars and a legend (if present).
*		*	Text size, font, colors etc. are pre-selected to look nice.
*		*	The code the user has to write to make a nice plot is minimized.
*		*	The default text or marker size etc. can be tuned in the constructor
*		*	The default color series can be tuned in Plots::GetMy*Color(int i)
******************************************************************************/
class Plot
{
	// Class variables for internal use:
	TVirtualPad* pad;
	TH1**        hists;
	TGraphErrors**   graphs;
	TF1**        func;
	TLine*       line;
	TLatex*      texts[MAXTEXTS];
	TLegend*     legend;
	int          n;
	int          nLeg;
	int          nLegCol;
	int          nLines;
	int	         nTexts;
	bool         xLog, yLog, normalize, bars, hbars;
	float        textSize, markerSize, lineWidth[2];
	float        legX0, legY0;
	float        yMinUser, yMaxUser, xMinUser, xMaxUser;
	float        bMargin, tMargin, rMargin, lMargin;
	float        xLabelOffset, yLabelOffset, xTitleOffset, yTitleOffset;
	float        lineCoords[MAXLINES][4];
	int*         flags;
	char**       legLabels;
	const char*  name;
	const char*  title;
	const char*  xLabel;
	const char*  yLabel;
	// private functions:
	bool IsHistogram(int i) { return (graphs[i]==NULL) && (func[i]==NULL);}

	public:
		/*********************************************************************
		* The constructor arguments:
		*	targPad		the pad where the plot should be (e.g. canv->GetPad(1))
		*	nHists		total number of all graphic objects to be plotted
		*	plotName	a unique name
		* Optional arguments:
		*	plotTitle	plot title (""..no title,NULL..first added object title)
		*	xTitle		x-axis title (NULL..no title)
		*	yTitle		y-axis title (NULL..no title)
		**********************************************************************/
		Plot(TVirtualPad* targPad, int nHists, const char* plotName,
			 const char* plotTitle = NULL, const char* xTitle = NULL,
			 const char* yTitle = NULL);
		~Plot();

		/**********************************************************************
		* The static constants store numeric representation of plot styles, 
		* thanks to this they can be easily combined
		* e.g.	line & filed histogram: Plot::Line | Plot::Fill, 
		* 		graph with erors: Plot::Point | Plot::Error
		**********************************************************************/
		static const int Hist  =  1;	// normal histogram
		static const int Line  =  2;	// line connecting points
		static const int Fill  =  4;	// filled (for now only for histograms)
		static const int Erry  =  8;	// y-errorbars
		static const int Point = 16;	// markers at each point
		static const int Bars  = 32;	// bar chart (for all!)
		static const int HBars = 64;	// horizontal bar chart (for all!)

		/********* A D D   O B J E C T S ***************************
		* 	style	see the static consts above (Hist, Line etc.)
		* 	index	optional - change plotting style (-1..automatic)
		*			colors corresponding to indices - see Hists::GetMy..Color()
		*			To change the color sequence, edit Hists::GetMy..Color()
		*	leg		optional - text for the legend (""..no legend entry)
		*/
		// a HISTOGRAM (supported styles: Hist, Fill, Erry, Point, Bars, HBars)
		void Add(const TH1* h,int style=Hist,int index=-1,const char* leg="");
		void Add(const TH1* h,const char* leg,int style = Hist,int index = -1);

		// a GRAPH (supported styles: Line, Point, Erry)
		void Add(const TGraph* h,int style = Line | Point,int index = -1,
				const char* leg = "");
		void Add(const TGraph* h,const char* leg,int style = Line | Point,
				int index = -1);

		// a FUNCTION (no style options, a line is always drawn):
		void Add(const TF1* f,int index = -1,const char* leg = "");
		void Add(const TF1* f,const char* leg,int index = -1);

		// draw a STRAIGHT LINE
		void AddLine(float x0, float y0, float x1, float y1);

		// draw a TEXT
		void AddText(float x0, float y0, const char* text, float size = -1);

		/***************** S E T T I N G S ***************************/
		// call BEFORE Plot::Draw() is called
		void SetNormalized() { normalize = true; }	// norm. all histos to 1.0
		void SetXLog(bool log = true) { xLog = log; }
		void SetYLog(bool log = true) { yLog = log; }
		void SetYRange(float min, float max) { SetYMin(min); SetYMax(max); }
		void SetYMin(float min) { yMinUser = min; }
		void SetYMax(float max) { yMaxUser = max; }
		void SetXRange(float min, float max) { SetXMin(min); SetXMax(max); }
		void SetXMin(float min) { xMinUser = min; }
		void SetXMax(float max) { xMaxUser = max; }
		void SetTextSize(float size) { textSize = size; }
		void SetMargins(float l, float r, float b, float t); // -1 means default
		void SetXLabelOffset(float o) { xLabelOffset = o; }
		void SetYLabelOffset(float o) { yLabelOffset = o; }
		void SetXTitleOffset(float o) { xTitleOffset = o; }
		void SetYTitleOffset(float o) { yTitleOffset = o; }
		void SetLegX0(float x0)       { legX0 = x0; }
		void SetLegY0(float y0)       { legY0 = y0; }
		void SetLegCol(int n)         { nLegCol = n; }
		void SetMarkerSize(float size){ markerSize = size; }
		// draw plot
		// (call AFTER all objects have been added and all settings applied)
		void Draw(bool verb = true);

		// calculate means for each point (assuming the x(i) approx. the same)
		// and save into gMean. y-mean is weighted, x-mean is not.
		// Errors of the y-mean can be either weighted or not 
		// (to check stat. compatibility)
		void MeanGraph(TGraphErrors* gMean,bool wErr = true,TH1* hPulls = NULL);
		void MeanHisto(TH1* hMean, TH1* hPulls = NULL);

		/************** F O R   F I N E - T U N I N G *****************
		* In variance with normal settings, fine-tuning is to be done AFTER
		* Plot::Draw() is called. Normally, this is not needed. 
		* Axis label and title offsets etc. can be manipulated using 
		* the pointer to the dummy "underlying" histogram 
		* (empty hist, used only to draw axes and plot title)
		*/
		TH1* UnderlyingHist() { return hists[0]; }
		// get individual histograms (for tuning)
		TH1* GetHist(int i) { return hists[i+1]; }
    
		TGraphErrors* GetGraph(int i) { return graphs[i+1]; }
};

/******************************************************************************
* The class PanelPlot represents a "table" of plots, 
* which share the same x and y axes and only axis labels and titles of the 
* bottom and left plots are used, rest is hidden.
* (this style is often used for asymmetries in kin. bins)
******************************************************************************/
class PanelPlot
{
	Plot***			plots;		// 2-dim array of plot pointers
	int 			nx, ny;		// # columns, # rows
	int				nHists;		// max. number of histos (graphs etc.) per pad
	TVirtualPad* 	motherPad;	// main pad (contains others)
	TPad***			pads;
	const char*     name;
	const char*		title;
	char**			xTitles;
	char** 			yTitles;
	
	public:
		/*********************************************************************
		* The constructor arguments:
		*	targPad			the pad where the plot should be 
		*					(can be directly a TCanvas object or canv->GetPad(1))
		*	nCol, nRow		number of columns and rows
		*	nHistsPerPad	max. number of objects in each sub-plot
		*	plotName		a unique name
		* Optional arguments:
		*	xTitles			an array of x-axis titles (length must be nCol)
		*	yTitles			an array of y-axis titles (length must be nRow)
		**********************************************************************/
		PanelPlot(TVirtualPad* targPad, int nCol, int nRow, int nHistsPerPad,
				const char* plotName, const char** xAxisTitles = NULL,
				const char** yAxisTitles = NULL);
		~PanelPlot();

		/*********************************************************************
		* Histograms, graphs etc. can be added using the GetPlot function,
		* e.g.: myPanelPlot->GetPlot(0,0)->Add(h1);
		* Also axis ranges, text sizes etc. can be set in this way.
		* The indices ix, iy start from bottom left.
		**********************************************************************/
		Plot* GetPlot(int ix, int iy);
		// draw the panel-plot
		// (call AFTER all objects have been added and all settings applied)
		void Draw(bool verb = true);
};

/******************************************************************************
* The class ComPlot serves for comparison of two or more histograms
* It creates two plots, first of the normalised distributions and second
* of the ratio of them (all wrt the first one or a chosen one).
******************************************************************************/
class ComPlot
{
	Plot*			dPlot;
	Plot*			rPlot;
	int				n;
	int				i;
	int				iRef;
	bool			showRLeg;
	TVirtualPad* 	motherPad;
	TPad*			dPad;
	TPad*			rPad;
	const char*		name;
	TH1**			hists;

	public:
		// The constructor arguments: like for Plot
		ComPlot(TVirtualPad* targPad,int nHist,const char* plotName,
			 const char* plotTitle = NULL, const char* xTitle = NULL,
			 const char* yTitle = NULL);
		~ComPlot();

		// A D D   O B J E C T S (same as for Plot)
		// a HISTOGRAM (supported styles: Hist, Fill, Erry, Point)
		void Add(const TH1* h,int style=Plot::Hist,int index=-1,
					const char* leg="");
		void Add(const TH1* h,const char* leg,int style = Plot::Hist,
					int index = -1);
		/***************** S E T T I N G S ***************************/
		// call BEFORE Plot::Draw() is called
		void SetXLog(bool log = true) { dPlot->SetXLog(log); rPlot->SetXLog(log);}
		void SetYLog(bool log = true) { dPlot->SetYLog(log); }
		void SetYRange(float min, float max) { SetYMin(min); SetYMax(max); }
		void SetYMin(float min) { dPlot->SetYMin(min); }
		void SetYMax(float max) { dPlot->SetYMax(max); }
		void SetXRange(float min, float max) { SetXMin(min); SetXMax(max); }
		void SetXMin(float min) { dPlot->SetXMin(min); rPlot->SetXMin(min); }
		void SetXMax(float max) { dPlot->SetXMax(max); rPlot->SetXMax(max); }
		void SetTextSize(float size)
				{ dPlot->SetTextSize(size); rPlot->SetTextSize(size); }
		void SetMargins(float l, float r, float b, float t)
				{ dPlot->SetMargins(l,r,0,t); rPlot->SetMargins(l,r,b,0); }
		void SetXLabelOffset(float o) { rPlot->SetXLabelOffset(o); }
		void SetYLabelOffset(float o) 
				{ dPlot->SetYLabelOffset(o); rPlot->SetYLabelOffset(o); }
		void SetXTitleOffset(float o) { rPlot->SetXTitleOffset(o); }
		void SetYTitleOffset(float o) 
				{ dPlot->SetYTitleOffset(o);  rPlot->SetYTitleOffset(o); }
		// change iRef for the ratio plot (hist[i]/hist[iRef] is plotted there):
		void SetReferenceHistIndex(int j) { iRef = j; };
		void SetShowRLeg(bool show) { showRLeg = show; }
		// **** access the individual plots ****
		Plot* GetNormPlot()  { return dPlot; };
		Plot* GetRatioPlot() { return rPlot; };
		// draw the comparison-plot
		// (call AFTER all objects have been added and all settings applied)
		void Draw(bool verb = true);
};

/******************************************************************************
* Class Hists just stores a buch of static functions,
* some of them are needed by Plot, others can be useful also for a gerelal user.
******************************************************************************/
class Hists
{
	public:
		/*********************************************************************
		* SetLogBins takes a histogram and changes binning to logaritmic
		* (e.g. [0.01,0.1], [0.1,1.0]...)
		* When constructing the histogram, set Xmin and Xmax to a and b
		* to get the range from 10^a to 10^b
		*********************************************************************/
		static void  SetLogBins(TAxis* axis);
		// CheckOverflow: In case of events in under & overflow bins print them
		static void  CheckOverflow(TH1* hist);
		/**********************************************************************
		* DrawBarChart: takes a histogram and draws it as a "bar chart" to
		* the current pad. To draw more histograms with bars next to each other
		* simply do: 	DrawBarChart(h1,2,0);	// number of hists = 2, i = 0
		*				DrawBarChart(h2,2,1);	// number of hists = 2, i = 1
		* The plan is to implement this to the Plot class, but its not done yet.
		**********************************************************************/
		static void  DrawBarChart(TH1* hist, int n = 1, int index = 0,
									float font = 0.07);
		// DrawHBarChart: the same but with horizontal bars.
		static void  DrawHBarChart(TH1* hist, int n = 1, int index = 0, 
									float font = 0.07);
		// Get customized fill or line color with a certain index (i=0..5)
		// If you prefer different color sequence, just edit this function...
		static int   GetMyFillColor(int i);
		static int   GetMyLineColor(int i);
		// Set text sizes and fonts to the histogram labels & titles,
		// also sets number axis divisions (ROOT default is too fine)
		static void  SetMyTextOptions(TH1* hist, float size = 0.07);
		/**********************************************************************
		* Integrate histogram in range [x0,x1]
		* If binWidth==true, the sum of bin contents is multiplied by bin width.
		**********************************************************************/
		static double Integral(float x1,float x2,TH1* h,
									double* sigmaI = NULL,int verbLev = 1);
		/**********************************************************************
		* FindSmallestAbove: Find the smallest bin content, but larger than min.
		* It is used in the Plot class (with min=0 to set yrange with logscale).
		**********************************************************************/
		static float FindSmallestAbove(TH1* hist, float min);
		// Chi^2 prob. dens., 1 parameter (p[0]... 'degrees of freedom'):
		static double Chi2(double *x, double* p);
		// Chi^2 * constant, 2 par. (p[0]... constant, p[1]... 'deg. of fr.')
		static double CxChi2(double *x, double* p) { return p[0]*Chi2(x, p+1); }
};

#endif	// #ifndef __HISTS__
