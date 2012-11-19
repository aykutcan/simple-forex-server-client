/*
 * history_graph.h
 *
 *  Description: Draw history graph
 *  Created on: Nov 4, 2012
 *      Author: nirin
 */

#ifndef HISTORY_GRAPH_H_
#define HISTORY_GRAPH_H_

#include "outgoing_request.h"
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <glib.h>
#include <cairo.h>
#include <gtkmm.h>
#include <gdkmm.h>
#include <glib.h>
#include <sigc++.h>
#include <signal.h>
#include <vector>

using namespace std;

class HistoryGraph : public Gtk::DrawingArea{
public:
		HistoryGraph();
		virtual ~HistoryGraph();
		void setHistoryData(string csv);
		void setIndicatorType(string indicator_type){this->indicator_type = indicator_type;};

protected:
		//Override default signal handler:
		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:
		vector<pair<int, int> > line;
		vector<pair<string, double> > data;
		string indicator_type;
		double data_min;
		double data_max;
		vector<pair<string, double> > extractDataFromCSV(string csv);
		void setMinMaxData(vector<pair<string, double> > data);
		vector<int> transform2YCoordination(vector<double> data, int H);
		void drawHistory(vector<pair<string, double> > data
				, const Cairo::RefPtr<Cairo::Context>& cr
				, int W
				, int H);

		double getProperTickGap(vector<pair<string, double> > data);
		vector<double> getTickValues(double min, double max, double tick_gap);
		void drawTicks(const Cairo::RefPtr<Cairo::Context>& cr, int W, int H);
		void drawTickLabels(const Cairo::RefPtr<Cairo::Context>& cr
				, int width
				, vector<double> value_ys
				, vector<int> plot_ys);

		void drawIndicator(string str
				, vector<pair<string, double> > data
				, const Cairo::RefPtr<Cairo::Context>& cr
				, int W
				, int H);

		vector<string> markPeriodLabels(vector<string> dates, OutgoingRequest::RequestPeriod period);
		void drawPeriodLabels(const Cairo::RefPtr<Cairo::Context>& cr, int W, int H);
};

#endif /* HISTORY_GRAPH_H_ */
