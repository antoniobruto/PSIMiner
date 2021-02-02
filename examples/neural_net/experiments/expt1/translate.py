import sys
import csv
with open('robot_arm_csv_good_traces_6.csv','r') as f:
	maxTime = 0.0
	reader = csv.reader(f,delimiter=',')
	i = 1
	bound = 2
	l = [-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0]
	r = [-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0]
	switch = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
	intervalSet = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]]
	listFile= open("robot_arm_csv_good_traces_6.dat","w+")
	for row in reader:
		if i>bound:
			if ( ( float(row[1]) >= 0.4  ) ) :
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
			if ( ( float(row[1]) < 0.4  ) )  and ( ( float(row[1]) >= 0.3  ) ) :
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
			if ( ( float(row[1]) < 0.3  ) )  and ( ( float(row[1]) >= 0.2  ) ) :
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
			if ( ( float(row[1]) < 0.2  ) )  and ( ( float(row[1]) >= 0.1  ) ) :
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
			if ( ( float(row[1]) < 0.1  ) )  and ( ( float(row[1]) >= 0.0  ) ) :
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
			if ( ( float(row[1]) < 0.0  ) )  and ( ( float(row[1]) >= -0.1  ) ) :
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
			if ( ( float(row[1]) < -0.1  ) )  and ( ( float(row[1]) >= -0.2  ) ) :
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
			if ( ( float(row[1]) < -0.2  ) )  and ( ( float(row[1]) >= -0.3  ) ) :
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
			if ( ( float(row[1]) < -0.3  ) )  and ( ( float(row[1]) >= -0.4  ) ) :
				if switch[8] == 0:
					switch[8] = 1
					l[8] = float(row[0])
					r[8] = float(row[0])+0.000100
				else:
					r[8] = float(row[0])
			else:
				if switch[8] == 1:
					r[8] = float(row[0])
					switch[8] = 0
					intervalSet[8] = intervalSet[8] + [[l[8],r[8]]]
					l[8] = -1.0
					r[8] = -1.0
			if ( ( float(row[1]) < -0.4  ) ) :
				if switch[9] == 0:
					switch[9] = 1
					l[9] = float(row[0])
					r[9] = float(row[0])+0.000100
				else:
					r[9] = float(row[0])
			else:
				if switch[9] == 1:
					r[9] = float(row[0])
					switch[9] = 0
					intervalSet[9] = intervalSet[9] + [[l[9],r[9]]]
					l[9] = -1.0
					r[9] = -1.0
			if ( ( float(row[2]) >= 0.05 + float(row[3])  ) ) :
				if switch[10] == 0:
					switch[10] = 1
					l[10] = float(row[0])
					r[10] = float(row[0])+0.000100
				else:
					r[10] = float(row[0])
			else:
				if switch[10] == 1:
					r[10] = float(row[0])
					switch[10] = 0
					intervalSet[10] = intervalSet[10] + [[l[10],r[10]]]
					l[10] = -1.0
					r[10] = -1.0
			if ( ( float(row[3]) >= 0.05 + float(row[2])  ) ) :
				if switch[11] == 0:
					switch[11] = 1
					l[11] = float(row[0])
					r[11] = float(row[0])+0.000100
				else:
					r[11] = float(row[0])
			else:
				if switch[11] == 1:
					r[11] = float(row[0])
					switch[11] = 0
					intervalSet[11] = intervalSet[11] + [[l[11],r[11]]]
					l[11] = -1.0
					r[11] = -1.0
			if ( ( float(row[4]) >= 0.01 + float(row[5])  ) ) :
				if switch[12] == 0:
					switch[12] = 1
					l[12] = float(row[0])
					r[12] = float(row[0])+0.000100
				else:
					r[12] = float(row[0])
			else:
				if switch[12] == 1:
					r[12] = float(row[0])
					switch[12] = 0
					intervalSet[12] = intervalSet[12] + [[l[12],r[12]]]
					l[12] = -1.0
					r[12] = -1.0
			if ( ( float(row[5]) >= 0.01 + float(row[4])  ) ) :
				if switch[13] == 0:
					switch[13] = 1
					l[13] = float(row[0])
					r[13] = float(row[0])+0.000100
				else:
					r[13] = float(row[0])
			else:
				if switch[13] == 1:
					r[13] = float(row[0])
					switch[13] = 0
					intervalSet[13] = intervalSet[13] + [[l[13],r[13]]]
					l[13] = -1.0
					r[13] = -1.0
			if ( ( float(row[2]) < 0.05 + float(row[3])  ) )  and ( ( float(row[3]) < 0.05 + float(row[2])  ) ) :
				if switch[14] == 0:
					switch[14] = 1
					l[14] = float(row[0])
					r[14] = float(row[0])+0.000100
				else:
					r[14] = float(row[0])
			else:
				if switch[14] == 1:
					r[14] = float(row[0])
					switch[14] = 0
					intervalSet[14] = intervalSet[14] + [[l[14],r[14]]]
					l[14] = -1.0
					r[14] = -1.0
			if ( ( float(row[4]) < 0.01 + float(row[5])  ) )  and ( ( float(row[5]) < 0.01 + float(row[4])  ) ) :
				if switch[15] == 0:
					switch[15] = 1
					l[15] = float(row[0])
					r[15] = float(row[0])+0.000100
				else:
					r[15] = float(row[0])
			else:
				if switch[15] == 1:
					r[15] = float(row[0])
					switch[15] = 0
					intervalSet[15] = intervalSet[15] + [[l[15],r[15]]]
					l[15] = -1.0
					r[15] = -1.0

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
	if switch[8] == 1:
		intervalSet[8] = intervalSet[8] + [[l[8],r[8]]]
	if switch[9] == 1:
		intervalSet[9] = intervalSet[9] + [[l[9],r[9]]]
	if switch[10] == 1:
		intervalSet[10] = intervalSet[10] + [[l[10],r[10]]]
	if switch[11] == 1:
		intervalSet[11] = intervalSet[11] + [[l[11],r[11]]]
	if switch[12] == 1:
		intervalSet[12] = intervalSet[12] + [[l[12],r[12]]]
	if switch[13] == 1:
		intervalSet[13] = intervalSet[13] + [[l[13],r[13]]]
	if switch[14] == 1:
		intervalSet[14] = intervalSet[14] + [[l[14],r[14]]]
	if switch[15] == 1:
		intervalSet[15] = intervalSet[15] + [[l[15],r[15]]]

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
