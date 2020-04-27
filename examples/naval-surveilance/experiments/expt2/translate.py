import sys
import csv
with open('anomalous4.csv','r') as f:
	maxTime = 0.0
	reader = csv.reader(f,delimiter=',')
	i = 1
	bound = 2
	l = [-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0]
	r = [-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0]
	switch = [0,0,0,0,0,0,0,0]
	intervalSet = [[],[],[],[],[],[],[],[]]
	listFile= open("anomalous4.dat","w+")
	for row in reader:
		if i>bound:
			if ( ( float(row[1]) >= 60  ) )  and ( ( float(row[1]) <= 70  ) )  and ( ( float(row[2]) >= 36  ) )  and ( ( float(row[2]) <= 45  ) ) :
				if switch[0] == 0:
					switch[0] = 1
					l[0] = float(row[0])
					r[0] = float(row[0])+0.000100
				else:
					r[0] = float(row[0])
			else:
				if switch[0] == 1:
					r[0] = float(row[0])
					switch[0] = 0
					intervalSet[0] = intervalSet[0] + [[l[0],r[0]]]
					l[0] = -1.0
					r[0] = -1.0
			if ( ( float(row[1]) >= 35  ) )  and ( ( float(row[1]) <= 45  ) )  and ( ( float(row[2]) >= 18  ) )  and ( ( float(row[2]) <= 25  ) ) :
				if switch[1] == 0:
					switch[1] = 1
					l[1] = float(row[0])
					r[1] = float(row[0])+0.000100
				else:
					r[1] = float(row[0])
			else:
				if switch[1] == 1:
					r[1] = float(row[0])
					switch[1] = 0
					intervalSet[1] = intervalSet[1] + [[l[1],r[1]]]
					l[1] = -1.0
					r[1] = -1.0
			if ( ( float(row[1]) >= 35  ) )  and ( ( float(row[1]) <= 45  ) )  and ( ( float(row[2]) >= 25  ) )  and ( ( float(row[2]) <= 35  ) ) :
				if switch[2] == 0:
					switch[2] = 1
					l[2] = float(row[0])
					r[2] = float(row[0])+0.000100
				else:
					r[2] = float(row[0])
			else:
				if switch[2] == 1:
					r[2] = float(row[0])
					switch[2] = 0
					intervalSet[2] = intervalSet[2] + [[l[2],r[2]]]
					l[2] = -1.0
					r[2] = -1.0
			if ( ( float(row[1]) >= 10  ) )  and ( ( float(row[1]) <= 15  ) )  and ( ( float(row[2]) >= 25  ) )  and ( ( float(row[2]) <= 35  ) ) :
				if switch[3] == 0:
					switch[3] = 1
					l[3] = float(row[0])
					r[3] = float(row[0])+0.000100
				else:
					r[3] = float(row[0])
			else:
				if switch[3] == 1:
					r[3] = float(row[0])
					switch[3] = 0
					intervalSet[3] = intervalSet[3] + [[l[3],r[3]]]
					l[3] = -1.0
					r[3] = -1.0
			if ( ( float(row[1]) >= 5  ) )  and ( ( float(row[1]) <= 10  ) )  and ( ( float(row[2]) >= 35  ) )  and ( ( float(row[2]) <= 40  ) ) :
				if switch[4] == 0:
					switch[4] = 1
					l[4] = float(row[0])
					r[4] = float(row[0])+0.000100
				else:
					r[4] = float(row[0])
			else:
				if switch[4] == 1:
					r[4] = float(row[0])
					switch[4] = 0
					intervalSet[4] = intervalSet[4] + [[l[4],r[4]]]
					l[4] = -1.0
					r[4] = -1.0
			if ( ( float(row[3]) == 1  ) ) :
				if switch[5] == 0:
					switch[5] = 1
					l[5] = float(row[0])
					r[5] = float(row[0])+0.000100
				else:
					r[5] = float(row[0])
			else:
				if switch[5] == 1:
					r[5] = float(row[0])
					switch[5] = 0
					intervalSet[5] = intervalSet[5] + [[l[5],r[5]]]
					l[5] = -1.0
					r[5] = -1.0
			if ( ( float(row[3]) == 3  ) ) :
				if switch[6] == 0:
					switch[6] = 1
					l[6] = float(row[0])
					r[6] = float(row[0])+0.000100
				else:
					r[6] = float(row[0])
			else:
				if switch[6] == 1:
					r[6] = float(row[0])
					switch[6] = 0
					intervalSet[6] = intervalSet[6] + [[l[6],r[6]]]
					l[6] = -1.0
					r[6] = -1.0
			if ( ( float(row[3]) == 2  ) ) :
				if switch[7] == 0:
					switch[7] = 1
					l[7] = float(row[0])
					r[7] = float(row[0])+0.000100
				else:
					r[7] = float(row[0])
			else:
				if switch[7] == 1:
					r[7] = float(row[0])
					switch[7] = 0
					intervalSet[7] = intervalSet[7] + [[l[7],r[7]]]
					l[7] = -1.0
					r[7] = -1.0

			maxTime = float(row[0])

		i=i+1
	if switch[0] == 1:
		intervalSet[0] = intervalSet[0] + [[l[0],r[0]]]
	if switch[1] == 1:
		intervalSet[1] = intervalSet[1] + [[l[1],r[1]]]
	if switch[2] == 1:
		intervalSet[2] = intervalSet[2] + [[l[2],r[2]]]
	if switch[3] == 1:
		intervalSet[3] = intervalSet[3] + [[l[3],r[3]]]
	if switch[4] == 1:
		intervalSet[4] = intervalSet[4] + [[l[4],r[4]]]
	if switch[5] == 1:
		intervalSet[5] = intervalSet[5] + [[l[5],r[5]]]
	if switch[6] == 1:
		intervalSet[6] = intervalSet[6] + [[l[6],r[6]]]
	if switch[7] == 1:
		intervalSet[7] = intervalSet[7] + [[l[7],r[7]]]

	for intervalList in intervalSet:

		listFile.write("{")

		for interval in intervalList:

			listFile.write("[")

			id = 1

			for val in interval:

				if id == 1:

					listFile.write("%s:" % repr(val))

					id = 2

			else:

				listFile.write("%s" % repr(val))

			listFile.write(")")

		listFile.write("}\n")

	listFile.write("%s" % repr(maxTime))

	listFile.close
