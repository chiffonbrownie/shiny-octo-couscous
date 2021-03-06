#
# p-fish : Python File System Hash Program
# Author: C. Hosmer
# July 2013
# Version 1.0
#
import logging  # Python Standard Library Logger
import time  # Python Standard Library time functions
import sys  # Python Standard Library system specific parameters
import _pfish  # _pfish Support Function Module

if __name__ == '__main__':
    PFISH_VERSION = '1.0'

    # Turn on Logging
    logging.basicConfig(filename='pFishLog.log',
                        level=logging.DEBUG,
                        format='%(asctime)s %(message)s')

    # Process the Command Line Arguments
    # _pfish.ParseCommandLine()
    arguments, hashType = _pfish.ParseCommandLine()

    # Record the Starting Time
    startTime = time.time()

    # Record the Welcome Message
    logging.info('')
    logging.info('Welcome to p-fish version 1.0 ... New Scan Started')
    logging.info('')
    _pfish.DisplayMessage('Welcome to p-fish ... version ' + PFISH_VERSION, arguments)

    # Record some information regarding the system
    logging.info('System: ' + sys.platform)
    logging.info('Version: ' + sys.version)

    # record hash function specified # //
    logging.info('Hash Function Specified: ' + hashType)

    # Traverse the file system directories and hash the files
    filesProcessed = _pfish.WalkPath(arguments, hashType)

    # Record the end time and calculate the duration
    endTime = time.time()
    duration = endTime - startTime

    logging.info('Files Processed: ' + str(filesProcessed))
    logging.info('Elapsed Time: ' + str(duration) + ' seconds')
    logging.info('')
    logging.info('Program Terminated Normally')
    logging.info('')

    _pfish.DisplayMessage("Program End", arguments)  # //
