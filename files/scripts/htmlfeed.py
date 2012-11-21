import re
import mysql.connector
from datetime import date, datetime
from dateutil.rrule import rrule, DAILY
import httplib

#read currency master
cnx = mysql.connector.connect(user='root', password='staccato', host='localhost', database='coredb')
cursor = cnx.cursor()
master = {}

def get_currency_master():
	master = {}
	query = ("SELECT symbol, name FROM tblCurrencyMaster")
	cursor.execute(query)

	for (symbol, name) in cursor:
		master[symbol] = name

	for key in master.keys():
		print(key, master[key])

	return master;

def store_database(date, symbol, filtered):
	#insert row
	add_row = ("INSERT INTO tblExchangeRate(date, `from`, `to`, rate) "
							  "VALUES (%s, %s, %s, %s)")

	for key in filtered.keys() :
		row_data = (date, symbol, key, filtered[key])        
		cursor.execute(add_row, row_data)                                
		cnx.commit()

def feed_html(keys, symbol, date):
	f = open("../temp/" + symbol + "_" + date + ".txt")
	html = f.read()
	start = html.find("<tbody>")
	end = html.find("</tbody>")
	tbody = html[start:end]
	data = {}
	while len(tbody) > 100:
		start_tr = tbody.find("<tr>")
		end_tr = tbody.find("</tr>")
		tr = tbody[start_tr:end_tr]
		
		try:
			temp = re.search("(<a href='/currency/).+(</a>)", tr)
			cur = re.search("(>)(.+)(<)", temp.group(0)).group(2)
			temp = re.search("(<td class=\"ICTRate\">)(.+)(</td>)", tr)
			val = re.search("([0-9])+\.([0-9])+", temp.group(0)).group(0)
			#print(cur, val)
			if cur not in data :
				data[cur] = val
				
		except AttributeError:
				print("oops")

		tbody = tbody[end_tr+4:]

	filtered = dict(zip(keys, [data[k] for k in keys]))
	store_database(date, symbol, filtered)

	
	
# main program                    
# set period

start_date = date(2012, 11, 4)
end_date = date(2012, 11, 4)

master = get_currency_master()
conn = httplib.HTTPConnection("www.xe.com")

for dt in rrule(DAILY, dtstart=start_date, until=end_date):
	date = dt.strftime("%Y-%m-%d")
	print("-------------------------------------------")
	print(date)
	print("-------------------------------------------")
	for symbol in master.keys():
		print("-------------------------------------------")
		print(date, symbol)
		print("-------------------------------------------")
		conn.request("GET", "/currencytables/?from="+ symbol + "&date=" + date)
		r = conn.getresponse()
		data = r.read()
		#print(data)
		f = open("../temp/" + symbol + "_" + date + ".txt", "w+")
		f.write(data)
		feed_html(master.keys(), symbol, date)
 
conn.close()
cursor.close()                                                                                              
cnx.close()
