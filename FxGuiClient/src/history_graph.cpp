/*
 * history_graph.cpp
 *
 *  Description: Draw history graph
 *  Created on: Nov 4, 2012
 *      Author: nirin
 */

#include "history_graph.h"
#include "indicators/base_indicator.h"
#include "indicators/sma.h"
#include "indicators/indicator_factory.h"
#include <pangomm.h>
#include <gtkmm.h>
#include <glib.h>
#include <sigc++.h>
#include <signal.h>
#include <cairomm/context.h>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <cmath>

using namespace boost::algorithm;
using namespace boost::gregorian;
using namespace boost;
using namespace std;

HistoryGraph::HistoryGraph() {
	// TODO Auto-generated constructor stub
	override_background_color(Gdk::RGBA("grey"));
	signal_draw().connect(sigc::mem_fun(*this, &HistoryGraph::on_draw), false);
}

HistoryGraph::~HistoryGraph() {
	// TODO Auto-generated destructor stub
}

vector<pair<string, double> > HistoryGraph::extractDataFromCSV(string csv)
{
  vector<string> results;
  boost::split(results, csv, boost::is_any_of(","));
  this->data.clear();
  for(size_t i=0; i<results.size(); i++){
	  vector<string> tuple;
	  boost::split(tuple, results.at(i), boost::is_any_of(":"));
	  string date = tuple.at(0);
	  double rate = atof(tuple.at(1).c_str());
	  pair<string, double> pair(date, rate);
	  this->data.push_back(pair);
  }
  return this->data;
}

void HistoryGraph::setMinMaxData(vector<pair<string, double> > data)
{
	data_min = data_max = data.at(0).second;
	for(size_t i=0; i<data.size(); i++){
		if(data_min > data.at(i).second) data_min = data.at(i).second;
		if(data_max < data.at(i).second) data_max = data.at(i).second;
	}
}

vector<int> HistoryGraph::transform2YCoordination(vector<double> data, int H)
{
	vector<int> ys;
	if(data.size()==0) return ys;

	double y;
	for(size_t i=0; i<data.size(); i++){
		y = data.at(i);
		y = (((y-data_min)/(data_max-data_min))*(H/2)) + (H/4);
		y = H - y;
		ys.push_back((int)y);
	}

	return ys;
}

double HistoryGraph::getProperTickGap(vector<pair<string, double> > data)
{
	if(data.size()==0) return 0;

	double R, tick_gap;
	R = data_max-data_min;
	double gs[] = {1.0, 0.5, 0.2, 0.1, 0.02, 0.01};
	vector<double> tick_gaps(gs, gs + sizeof(gs) / sizeof(double) );

	int count = 0;
	for(size_t i = 0; i<tick_gaps.size(); i++)
	{
		tick_gap = tick_gaps.at(i);
		count = R/tick_gap;
		if(count<10 && count >2)break;
	}
	return tick_gap;
}

vector<double> HistoryGraph::getTickValues(double min, double max, double tick_gap)
{
	vector<double> values;
	double temp = min;
	while(temp < max){
		temp += tick_gap;
		temp = ceil(temp/tick_gap)*tick_gap;
		cout << temp << endl;
		values.push_back(temp);
	}
	return values;
}

void HistoryGraph::drawTickLabels(const Cairo::RefPtr<Cairo::Context>& cr
		, int width
		, vector<double> value_ys
		, vector<int> plot_ys)
{
	Glib::RefPtr<Pango::Layout> pango = Pango::Layout::create(cr);
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_size(10*Pango::SCALE);
	pango->set_font_description(font);
	cr->set_source_rgb(0, 0, 0);

	for(size_t i = 0; i<plot_ys.size(); i++){
		pango->set_text(lexical_cast<string>(value_ys.at(i)));
		cr->move_to(width, plot_ys.at(i));
		pango->show_in_cairo_context(cr);
		cr->move_to(0, plot_ys.at(i));
		cr->line_to(width, plot_ys.at(i));
	}
	cr->stroke();
}

vector<string> HistoryGraph::markPeriodLabels(vector<string> dates, OutgoingRequest::RequestPeriod period){
	vector<string> period_labels;
	for(size_t i=0; i<dates.size(); i++){
		string date_str = dates.at(i);
		date d = from_string(date_str);

		if(d.day().as_number()==1){
			period_labels.push_back(d.month().as_short_string());
		}
		else
			period_labels.push_back("");
	}

	int set = 4;
	int j=0;
	vector<string> filter_period_labels;

	for(size_t i=0; i<period_labels.size(); i++){
		if(period_labels.at(i)!="")j++;

		if(j%set==0) {
			filter_period_labels.push_back(period_labels.at(i));
			cout << j << endl;
		}
		else filter_period_labels.push_back("");
	}

	return filter_period_labels;
}

string firstElement( const std::pair<string, double> &p ) {
    return p.first;
}

void HistoryGraph::drawPeriodLabels(const Cairo::RefPtr<Cairo::Context>& cr, int W, int H)
{
	if(data.size()==0) return;
	//int set = 4;
	//int data_gap = lexical_cast<double>(data.size())/set;

	Glib::RefPtr<Pango::Layout> pango = Pango::Layout::create(cr);
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_size(10*Pango::SCALE);
	pango->set_font_description(font);
	cr->set_source_rgb(0.2, 0.2, 0.2);
	cr->set_line_width(1.0);

	vector<string> dates;
	transform(this->data.begin()
			, this->data.end()
			,  std::back_inserter( dates )
			, firstElement);

	vector<string> period_labels = markPeriodLabels(dates, OutgoingRequest::OneYear);

	for(size_t i=0; i< data.size(); i++){
		if(period_labels.at(i) == "") continue;
		pair<string, int> point(period_labels.at(i), lexical_cast<double>(i)/period_labels.size()*W);
		cout << "first : "<< point.first << endl;
		//cout << "second : "<< point.second << endl;
		cr->move_to(point.second, H/8);
		cr->line_to(point.second, 3*H/4);
		pango->set_text(point.first);
		cr->move_to(point.second, H-100);
		pango->show_in_cairo_context(cr);
	}
	cr->stroke();
}

void HistoryGraph::drawTicks(const Cairo::RefPtr<Cairo::Context>& cr, int W, int H)
{
	if(data.size()==0) return;

	double min, max;
	min = max = data.at(0).second;
	for(size_t i=0; i<data.size(); i++){
		if(min > data.at(i).second) min = data.at(i).second;
		if(max < data.at(i).second) max = data.at(i).second;
	}

	double tick_gap = getProperTickGap(data);
	vector<double> value_ys = getTickValues(min, max, tick_gap);
	vector<int> plot_ys = transform2YCoordination(value_ys, H);

	drawTickLabels(cr, W, value_ys, plot_ys);
}

void HistoryGraph::drawIndicator(string str, vector<pair<string, double> > data, const Cairo::RefPtr<Cairo::Context>& cr, int W, int H)
{
	if(data.size()==0) return;

	BaseIndicator* indicator = IndicatorFactory::createIndicator(str);
	vector<pair<string,double> > indicator_data = indicator->calculate(data);
	vector<double> indicator_data_second;
	for(size_t i=0; i<indicator_data.size(); i++){
		indicator_data_second.push_back(indicator_data.at(i).second);
	}
	vector<int> indicator_line = transform2YCoordination(indicator_data_second, H);
	cr->set_source_rgb(0.0, 0.8, 0.0);
	cr->set_line_width(1.0);
	double gap = lexical_cast<double>(W)/indicator_line.size();
	for(size_t i=0; i < indicator_line.size(); i++){
		cr->line_to(i * gap, indicator_line.at(i));
	}
	cr->stroke();
}

void HistoryGraph::setHistoryData(string csv){
	vector<pair<string, double> > data = extractDataFromCSV(csv);
	setMinMaxData(data);
	this->data = data;
}

void HistoryGraph::drawHistory(vector<pair<string, double> > data
		, const Cairo::RefPtr<Cairo::Context>& cr, int W, int H)
{
	double gap = lexical_cast<double>(W)/data.size();

	vector<double> data_second;
	for(size_t i=0; i<data.size(); i++){
		data_second.push_back(data.at(i).second);
	}
	vector<int> plot = transform2YCoordination(data_second, H);

	cr->set_source_rgb(0.8, 0.0, 0.0);
	cr->set_line_width(1.0);
	for(size_t i=0; i < data.size(); i++){
		cr->line_to(i * gap, plot.at(i));
	}
	cr->stroke();
}


bool HistoryGraph::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	const double right_offset = 50;

	cout << "before history" << endl;
	drawHistory(this->data, cr, width-right_offset, height);
	cout << "before tick" << endl;
	drawTicks(cr, width-right_offset, height);
	cout << "before indicator" << endl;
	drawIndicator(indicator_type, this->data, cr, width - right_offset, height);
	drawPeriodLabels(cr, width - right_offset, height);

	return true;
}
