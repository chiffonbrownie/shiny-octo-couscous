#
# psearch support functions, where all the real work gets done
#
# Display Message()     ParseCommandLine()
# ValidateFileRead()    ValidateFileWrite()
# Matrix (class)
#
import argparse  # Python Standard Library - Parser for command-line options, arguments
import os  # Standard Library OS functions
import logging  # Standard Library Logging functions
import sys  # // add for SearchWords()
import Levenshtein # // add for edit distance

log = logging.getLogger('main._psearch')

# Constants
MIN_WORD = 5  # Minimum word size in bytes
MAX_WORD = 15  # Maximum word size in bytes
PREDECESSOR_SIZE = 32  # Values to print before match found
WINDOW_SIZE = 128  # Total values to dump when match found


# Name: ParseCommand() Function
#
# Desc: Process and Validate the command line arguments
#           use Python Standard Library module argparse
#
# Input: none
#
# Actions:
#           Uses the standard library argparse to process the command line
#

def ParseCommandLine():
    parser = argparse.ArgumentParser('Python Search')

    parser.add_argument('-v', '--verbose', help="enables printing of additional program messages", action='store_true')

    # // add editDisance option
    parser.add_argument('-e', '--editDistance',
                        help='alternative approximate match using the Levenshtein edit distance', action='store_true')

    parser.add_argument('-k', '--keyWords', type=ValidateFileRead, required=True,
                        help="specify the file containing search words")
    parser.add_argument('-t', '--srchTarget', type=ValidateFileRead, required=True,
                        help="specify the target file to search")
    parser.add_argument('-m', '--theMatrix', type=ValidateFileRead, required=True,
                        help="specify the weighted matrix file")


    global gl_args

    gl_args = parser.parse_args()

    DisplayMessage("Command line processed: Successfully")

    return


#
# Name: ValidateFileRead Function
#
# Desc: Function that will validate that a file exists and is readable
#
# Input: A file name with full path
#
# Actions:
#         If valid will return path
#
#         If invalid it will raise an ArgumentTypeError within argparse
#            which will in turn be reported by argparse to the user
#

def ValidateFileRead(theFile):
    # Validate the path is a valid
    if not os.path.exists(theFile):
        raise argparse.ArgumentTypeError('File does not exist')

    # Validate the path is readable
    if os.access(theFile, os.R_OK):
        return theFile
    else:
        raise argparse.ArgumentTypeError('File is not readable')


# End ValidateFileRead ===================================

#
# Name: DisplayMessage() Function
#
# Desc: Displays the message if the verbose command line option is present
#
# Input: message type string
#
# Actions:
#         Uses the standard library print function to display the message
#

def DisplayMessage(msg):
    if gl_args.verbose:
        print(msg)

    return


# End DisplayMessage =====================================

#
# Name SearchWords()
#
# Uses command line arguments
#
# Searches the target file for keywords
#

def SearchWords():
    # Create an empty set of search words
    searchWords = set()

    # Attempt to open and read search words
    try:
        fileWords = open(gl_args.keyWords)
        for line in fileWords:
            searchWords.add(line.strip())
    except:
        log.error('Keyword File Failure: ' + gl_args.keyWords)
        sys.exit()
    finally:
        fileWords.close()

    # Create Log Entry Words to Search For

    log.info('Search Words')
    log.info('Input File: ' + gl_args.keyWords)
    log.info(searchWords)

    # Attempt to open and read the target file
    # and directly load into a bytearray

    try:
        targetFile = open(gl_args.srchTarget, 'rb')
        baTarget = bytearray(targetFile.read())
    except:
        log.error('Target File Failure: ' + gl_args.srchTarget)
        sys.exit()
    finally:
        targetFile.close()

    sizeOfTarget = len(baTarget)

    # Post to log

    log.info('Target of Search: ' + gl_args.srchTarget)
    log.info('File Size: ' + str(sizeOfTarget))

    baTargetCopy = baTarget

    wordCheck = class_Matrix()

    # Search Loop
    # step one, replace all non characters with zero's

    for i in range(0, sizeOfTarget):
        character = chr(baTarget[i])
        if not character.isalpha():
            baTarget[i] = 0

    # step # 2 extract possible words from the bytearray
    # and then inspect the search word list
    # create an empty list of probable wnot found items

    indexOfWords = []

    cnt = 0
    for i in range(0, sizeOfTarget):
        character = chr(baTarget[i])
        if character.isalpha():
            cnt += 1
        else:
            # // old >> if (cnt >= MIN_WORD and cnt <= MAX_WORD):
            if MIN_WORD <= cnt <= MAX_WORD:
                newWord = ""
                for z in range(i - cnt, i):
                    newWord = newWord + chr(baTarget[z])

                newWord = newWord.lower()

                if (newWord in searchWords):
                    PrintBuffer(newWord, i - cnt, baTargetCopy, i - PREDECESSOR_SIZE, WINDOW_SIZE)
                    indexOfWords.append([newWord, i - cnt])
                    cnt = 0
                    # //
                    print()
                else: # approx match

                    # // if -e not set
                    if not gl_args.editDistance:
                        if wordCheck.isWordProbable(newWord):
                            # // add
                            PrintBuffer(newWord, i - cnt, baTargetCopy, i - PREDECESSOR_SIZE, WINDOW_SIZE)
                            indexOfWords.append([newWord, i - cnt])
                        cnt = 0
                    else:
                        #                                               threshold=3 or 4
                        if wordCheck.editDistance(newWord, searchWords, threshold=(len(newWord) * .5)):
                            # // add
                            PrintBuffer(newWord, i - cnt, baTargetCopy, i - PREDECESSOR_SIZE, WINDOW_SIZE)
                            indexOfWords.append([newWord, i - cnt])
                        cnt = 0
            else:
                cnt = 0
            cnt = 0 # //

    PrintAllWordsFound(indexOfWords)

    return


# End of SearchWords Function

#
# Print Hexidecimal / ASCII Page Heading
#

def PrintHeading():
    print("Offset        00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F         ASCII")
    print("------------------------------------------------------------------------------------------------")

    return


# End PrintHeading
#
# Print Buffer
#
# Prints Buffer contents for words that are discovered
# parameters
# 1) Word found
# 2) Direct Offset to beginning of the word
# 3) buff The bytearray holding the target
# 4) offset starting position in the buffer for printing
# 5) hexSize, size of hex display windows to print
#

def PrintBuffer(word, directOffset, buff, offset, hexSize):
    # // here means that the python 2 print statements were updated to python 3
    # //
    print("\nFound: " + word + " At Address: ", "%08x     " % directOffset)

    PrintHeading()

    for i in range(offset, offset + hexSize, 16):
        for j in range(0, 17):  # // 0, 17
            if j == 0:
                # //
                print("%08x      " % i, end='')
            else:
                byteValue = buff[i + j]
                # //
                print("%02x " % byteValue, end='')
        # //
        print("        ", end='')
        for j in range(0, 17):
            byteValue = buff[i + j]
            # // remove () and simplify
            if 0x20 <= byteValue <= 0x7f:
                # //
                print("%c" % byteValue, end='')
            else:
                # //
                print('.', end='')
        # //
        print()
    return


# End Print Buffer
#
# PrintAllWordsFound
#

def PrintAllWordsFound(wordList):
    # // here means that the python 2 print statements were updated to python 3
    # //
    print("\nIndex of All Words")
    print("---------------------")

    wordList.sort()

    for entry in wordList:
        # //
        print(entry)
    # //
    print("---------------------")
    print()

    return


# End PrintAllWordsFound
#
# Class Matrix
#
# init method, loads the matrix into the set
# weightedMatrix
#
# isWordProbable method
#   1) Calculates the weight of the provided word
#   2) Verifies the minimum length
#   3) Calculates the weight for the word
#   4) Tests the word for existence in the matrix
#   5) Returns true or false
#

class class_Matrix:
    weightedMatrix = set()

    def __init__(self):
        try:
            fileTheMatrix = open(gl_args.theMatrix, 'rb')
            for line in fileTheMatrix:
                value = line.strip()
                self.weightedMatrix.add(int(value, 16))
        except:
            log.error('Matrix File Error: ' + gl_args.theMatrix)
            sys.exit()
        finally:
            fileTheMatrix.close()

        return

    def isWordProbable(self, theWord):

        # // remove ()
        if len(theWord) < MIN_WORD:
            return False
        else:
            BASE = 96
            wordWeight = 0

            for i in range(4, 0, -1):
                charValue = (ord(theWord[i]) - BASE)
                shiftValue = (i - 1) * 8
                charWeight = charValue << shiftValue
                wordWeight = (wordWeight | charWeight)

            # // remove ()
            if wordWeight in self.weightedMatrix:
                return True
            else:
                return False

    # // levenshtein
    def editDistance(self, word, set_of_words, threshold):
        # similarity dictionary
        similarity = {}
        # find similarity of words compared to set of given words
        for target_word in set_of_words:
            # find distance between word and word in set
            distance = Levenshtein.distance(word, target_word)
            # give word in set a similarity score compared tp candidate word
            similarity[target_word] = distance
        # find min of set of similarity scores
        minimum_of_set = min(similarity, key=similarity.get)
        # return true if below passed threshold
        if similarity[minimum_of_set] < threshold:
            return True
        # false if below above threshold
        return False


## End Class Matrix
