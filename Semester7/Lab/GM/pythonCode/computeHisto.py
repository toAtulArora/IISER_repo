import csv
import peakdetect
with open('../data/TEK0008.CSV') as csvfile:
    spamreader = csv.reader(csvfile,delimiter=',')
    for row in spamreader:
        print ', '.join(row)
