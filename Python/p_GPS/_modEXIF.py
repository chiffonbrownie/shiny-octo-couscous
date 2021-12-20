#
# Data Extraction - Python-Forensiscs
# Extract GPS Data from EXIF supported Images  (jpg, tiff)
# Support Module
#

import os  # Standard Library OS functions
from classLogging import _ForensicLog  # Abstracted Fornesic Logging Class

# import the Python Image Library
# along with TAGS and GPS related TAGS

from PIL import Image
from PIL.ExifTags import TAGS, GPSTAGS


#
# Extract EXIF Data
#
# Input: Full Pathname of the target image
#
# Return: gps Dictionary and selected exifData list
#

def ExtractGPSDictionary(fileName):
    try:
        pilImage = Image.open(fileName)
        exifData = pilImage._getexif()

    except Exception:
        # If exception occurs from PIL processing
        # Report the
        return None, None

    # Iterate through the exifData
    # Searching for GPS Tags

    imageTimeStamp = None
    cameraModel = None
    cameraMake = None

    if exifData:

        for k, v in exifData.items():
            # print(k, v)

            if k == 271:
                cameraMake = v
            if k == 272:
                cameraModel = v
            if k == 306:
                imageTimeStamp = v

        for tag, theValue in exifData.items():

            # obtain the tag
            tagValue = TAGS.get(tag, tag)
            # Collect basic image data if available
            # if tagValue == 'DateTimeOriginal':
            #     imageTimeStamp = exifData.get(tag)
            # if tagValue == "Make":
            #     cameraMake = exifData.get(tag)
            # if tagValue == 'Model':
            #     cameraModel = exifData.get(tag)

            # check the tag for GPS
            if tagValue == "GPSInfo":
                # Found it !
                # Now create a Dictionary to hold the GPS Data
                gpsDictionary = {}

                # Loop through the GPS Information
                for curTag in theValue:
                    gpsTag = GPSTAGS.get(curTag, curTag)
                    gpsDictionary[gpsTag] = theValue[curTag]

                basicExifData = [imageTimeStamp, cameraMake, cameraModel]

                return gpsDictionary, basicExifData
        # // override implicit singular None
        return None, None
    else:
        return None, None


# End ExtractGPSDictionary ============================


#
# Extract the Latitude and Longitude Values
# From the gpsDictionary
#

def ExtractLatLon(gps):
    # to perform the calculation we need at least
    # lat, lon, latRef and lonRef

    # // replace has_key() with in
    if "GPSLatitude" in gps and "GPSLongitude" in gps and "GPSLatitudeRef" in gps and "GPSLatitudeRef" in gps:
        # print('has it all b')
        latitude = gps["GPSLatitude"]
        latitudeRef = gps["GPSLatitudeRef"]
        longitude = gps["GPSLongitude"]
        longitudeRef = gps["GPSLongitudeRef"]

        lat = ConvertToDegrees(latitude)
        lon = ConvertToDegrees(longitude)

        # Check Latitude Reference
        # If South of the Equator then lat value is negative

        if latitudeRef == "S":
            lat = 0 - lat

        # Check Longitude Reference
        # If West of the Prime Meridian in
        # Greenwich then the Longitude value is negative

        if longitudeRef == "W":
            lon = 0 - lon

        gpsCoor = {"Lat": lat, "LatRef": latitudeRef, "Lon": lon, "LonRef": longitudeRef}

        # // pull out altitude if it exists
        if "GPSAltitudeRef" in gps and "GPSAltitude" in gps:
            try:
                # altitude_ref = gps["GPSAltitudeRef"].decode()
                altitude_ref = ord(gps["GPSAltitudeRef"].decode())
            except (TypeError, AttributeError):
                altitude_ref = gps["GPSAltitudeRef"]
            altitude = gps["GPSAltitude"]

            gpsCoor.update({"AltRef": altitude_ref, "Alt": altitude})
        else:
            altitude_ref = ""
            altitude = ""

            gpsCoor.update({"AltRef": altitude_ref, "Alt": altitude})

        return gpsCoor

    else:
        return None


# End Extract Lat Lon ==============================================

#
# Convert GPSCoordinates to Degrees
#
# Input gpsCoordinates value from in EXIF Format
#

# // replace since pillow returns floats
def ConvertToDegrees(gpsCoordinate):
    deg, mins, secs = gpsCoordinate
    floatCoordinate = deg + mins / 60.0 + secs / 3600.0
    return floatCoordinate
