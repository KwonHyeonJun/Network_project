import MySQLdb

fp=open("blacklist1.txt", "r")
fp2=open("blacklist2.txt", "r")

try:
	conn = MySQLdb.connect(host='localhost', user='test', passwd='test',db='bob')
	cursor=conn.cursor()
	buf=fp.readline()
	buf=buf.rstrip('\n')
	cursor.execute("select * from blacklist")
	row=cursor.fetchone()
	print row
	while buf:
		cursor.execute('INSERT INTO blacklist VALUES("%s")' % buf)
		buf=fp.readline()
		buf=buf.rstrip('\n')
		
	buf=fp2.readline()
	while buf:
		cursor.execute('INSERT INTO blacklist VALUES("%s")' % buf)
		buf=fp2.readline()
		buf=buf.rstrip('\n')
	cursor.close()
	conn.commit()
except :
	print 'exception'

finally:
	if conn != None:
		conn.close()

