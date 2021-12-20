import csv  # Python Standard Library -  for csv files
import sys  # // for exiting if report csv is open


#
# Class: _CSVWriter
#
# Desc: Handles all methods related to comma separated value operations
#
# Methods  constructor:     Initializes the CSV File
#       writeCVSRow:    Writes a single row to the csv file
#       writerClose:    Closes the CSV File

class _CSVWriter:

    # add log object to be passed
    def __init__(self, fileName, log):
        try:
            # create a writer object and then write the header row
            self.csvFile = open(fileName, 'w')  # // remove 'b'
            self.writer = csv.writer(self.csvFile, delimiter=',',
                                     quoting=csv.QUOTE_ALL, lineterminator='\n')  # // add lineterminator='\n'

        except IOError:  # // for if report csv is open
            log.error('CSV File Failure')
            sys.exit(1)
        # // move down
        self.writer.writerow(('Image Path', 'TimeStamp', 'Camera Make', 'Camera Model', 'Lat Ref', 'Latitude',
                              'Lon Ref', 'Longitude', 'Alt Ref', 'Altitude'))  # // []
        # // add back altitude

    # // add back altitude
    def writeCSVRow(self, fileName, timeStamp, CameraMake, CameraModel, latRef, latValue, lonRef, lonValue, alt_ref, alt_value):
        latStr = '%.8f' % latValue
        lonStr = '%.8f' % lonValue
        self.writer.writerow((fileName, timeStamp, CameraMake, CameraModel, latRef, latStr, lonRef, lonStr, alt_ref, alt_value))

    def __del__(self):
        self.csvFile.close()
