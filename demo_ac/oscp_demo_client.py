import base64
import configparser
import json
import sys
import uuid
from collections import namedtuple
from datetime import datetime, timezone

import h3 as h3
import piexif as piexif
import requests
import urllib3
urllib3.disable_warnings()


# Latitude and longitude as named tuple
Point = namedtuple('Point', 'lat lon')
CHECK_PT = Point('47.609906', '-122.337810')  # Center point at Seattle nearby Pine.Str

# Precision level 8.
RES = 8  # The query API expects a client to provide a hexagonal coverage area by using an H3 index ex.

# Set this to True if you want to skip the SSD lookup and use the AC URL directly
DEBUG_SKIP_SSD = True

class Menu:
    """Display a menu and respond to choices when run."""

    def __init__(self, settings_file):
        # Reading config file
        config = configparser.ConfigParser()
        config.read(settings_file)
        self.countries = json.loads(config["spatial"]["COUNTRIES"])
        self.topics = json.loads(config["spatial"]["TOPICS"])
        self.token_url = config["urls"]["TOKEN_URL"]
        self.service_url = config["urls"]["SERVICE_URL"]
        self.audience = config["urls"]["AUDIENCE"]
        self.client_id = config["Secret"]["CLIENT_ID"]
        self.client_secret = config["Secret"]["CLIENT_SECRET"]

        self.choices = {
            '1': ('Get objects from AC provider', self.get_objects_from_ac),
            '2': ('Get geopose by image from AC provider', self.get_geopose_from_ac),
            '3': ('Get geopose by image from AC provider with near objects', self.get_geopose_objs_from_ac),
            '4': ('Get geopose with ecef by image from AC provider with near objects', self.get_ecef_geopose_objs_from_ac),
            '0': ('Quit', self.quit)
        }

    # TODO: either move the conversion out or rename to load_image_base64
    def __load_image(self, img_file_name):
        """
        Load image and encode it in base64
        :param img_file_name:
        :return:
        """
        try:
            img_file = open(img_file_name, 'rb').read()
            img_file_base64 = base64.b64encode(img_file)
            return img_file_base64.decode('utf-8')
        except IOError:
            print("Wrong filename or the file does not exist: " + img_file_name)
            return None
        except:
            print("Base64 encode/decode error")
            return None

    def __create_h3_from_lat_lon(self, lat, lon):
        """
        Get h3index from latitude and longitude
        :param lat:
        :param lon:
        :return:
        """
        try:
            return h3.geo_to_h3(float(lat), float(lon), RES)
        except ValueError:
            print("Wrong latitude or longitude values\n")
            return None
        except Exception as e:
            print(f"Error: {e}")
            return None

    def __create_geopose_request(self, img_file_base64_string, lat, lon):
        """
        Create request body for geopose
        :param img_file_base64_string:
        :param lat:
        :param lon:
        :return:
        """
        return {
            "id": str(uuid.uuid4()),
            "timestamp": datetime.now(timezone.utc).timestamp()*1000,
            "type": "geopose",
            "sensors": [
                {
                    "id": "0",
                    "type": "camera"
                },
                {
                    "id": "1",
                    "type": "geolocation"
                }
            ],
            "sensorReadings": [
                {
                    "timestamp": datetime.now(timezone.utc).timestamp()*1000,
                    "sensorId": "0",
                    "reading": {
                        "sequenceNumber": 0,
                        "imageFormat": "JPG",
                        "imageOrientation": {
                            "mirrored": False,
                            "rotation": 0
                        },
                        "imageBytes": img_file_base64_string
                    }
                },
                {
                    "timestamp": datetime.now(timezone.utc).timestamp()*1000,
                    "sensorId": "1",
                    "reading": {
                        "latitude": float(lat),
                        "longitude": float(lon),
                        "altitude": 0
                    }
                }
            ]
        }

    def __get_valid_jwt_by_auth0(self):
        """
        Uses payload and TOKEN_URL to get valid JWT token
        :return: returns JWT token
        """
        payload = {
            'grant_type': 'client_credentials',
            'audience': self.audience,
            'client_id': self.client_id,
            'client_secret': self.client_secret
        }
        headers = {'content-type': 'application/json'}
        try:
            result = requests.post(url=self.token_url, headers=headers, data=json.dumps(payload, indent=2), verify=False)
            return result.json()['access_token']
        except:
            print("Error getting JWT token. Please check your AUDIENCE, CLIENT_ID, CLIENT_SECRET values\n")
            return None

    def __get_ac_url(self, token, country):
        """
        Get AC url (Needs authorization)
        Endpoint: /provider/ssrs
        :param token: valid JWT token
        :param country: valid country id from COUNTRIES
        :return:
        """
        if DEBUG_SKIP_SSD:
            return "http://developer.augmented.city"

        headers = {'content-type': 'application/json',
                   'Authorization': f'Bearer {token}'}
        response = requests.get(self.service_url + f"/{country}/provider/ssrs", headers=headers, verify=False)
        try:
            ac_url = response.json()[0]["services"][0]["url"]  # We extract the first given service provider
            return ac_url
        except IndexError:
            print("No coverage in this area\n")
            return None
        except:
            print("Connection error to ssd service or wrong url\n")
            return None

    def __get_ac_url_with_id(self, country, h3Index):
        f"""
        Get AC url by h3index without authorization
        Endpoint: /country/ssrs
        :param country: valid country id from COUNTRIES
        :param h3Index: valid h3Index
        :return:
        """
        if DEBUG_SKIP_SSD:
            return "http://developer.augmented.city"

        params = {"h3Index": h3Index}
        try:
            response = requests.get(self.service_url + f"/{country}/ssrs", params=params, verify=False)
            ac_url = response.json()[0]["services"][0]["url"]  # We extract the first given service provider
            return ac_url
        except IndexError:
            print("No coverage in this area\n")
            return None
        except:
            print("Connection error to ssd service or wrong url\n")
            return None

    def __get_request(self, ac_url, top, h3Index):
        """
        Access SCD service content
        :param ac_url: url with topic endpoint
        :param h3Index: valid h3Index
        :return: returns response with topic
        """
        if top in self.topics:
            new_url = ac_url + f"/scrs/{top}"
        else:
            print(f'Topic "{top}" not found!')
            print(f'Correct values are: {self.topics}')
            return
        params = {"h3Index": h3Index}
        try:
            response = requests.get(new_url, params=params, verify=False)
            return json.dumps(response.json(), indent=4)
        except:
            print("Error getting content from AC scd service\n")
            return None

    def __post_geopose(self, ac_url, req_body):
        """
        Access SCD service geopose
        :param ac_url:
        :param req_body:
        :return:
        """
        try:
            response = requests.post(url=f"{ac_url}/geopose",
                                     headers={'Content-Type': 'application/json'},
                                     data=json.dumps(req_body), verify=False)
            return json.dumps(response.json(), indent=4)
        except:
            print("Error getting geopose from AC scd service\n")
            return None

    def __post_geopose_with_objects_and_ecef(self, ac_url, req_body):
        """
        Access SCD service geopose
        :param ac_url:
        :param req_body:
        :return:
        """
        try:
            response = requests.post(url=f"{ac_url}/scrs/geopose_objs_local",
                                     headers={'Content-Type': 'application/json'},
                                     data=json.dumps(req_body), verify=False)
            return json.dumps(response.json(), indent=4)
        except:
            print("Error getting geopose from AC scd service\n")
            return None

    def __get_precise_coords(self, response):
        """
        Get lat and lon from geopose response
        :param response:
        :return:
        """
        try:
            jsonify_response = json.loads(response)
            precise_lat = jsonify_response['geopose']['position']['lat']
            precise_lon = jsonify_response['geopose']['position']['lon']
            return precise_lat, precise_lon
        except:
            print('Error reading coords from geopose reponse')

    def __to_decimals(self, coordinates):
        """
        Convert lat and lon from exif to decimals
        :param coordinates:
        :return:
        """
        decimals = []
        for coordinate in coordinates:
            decimal: float = (coordinate[0][0] / coordinate[0][1]) + \
                             ((coordinate[1][0] / coordinate[1][1]) / 60) + \
                             ((coordinate[2][0] / coordinate[2][1]) / 3600)
            decimals.append(decimal)
        return decimals

    def __get_exif_from_img(self, imagefile):
        """
        Try to read exif from image file
        :return:
        """
        try:
            coordinates = []
            exif = piexif.load(imagefile)
            lat = exif["GPS"][piexif.GPSIFD.GPSLatitude]
            lon = exif["GPS"][piexif.GPSIFD.GPSLongitude]
            lat_ref = str(exif["GPS"][piexif.GPSIFD.GPSLatitudeRef], 'utf-8')
            lon_ref = str(exif["GPS"][piexif.GPSIFD.GPSLongitudeRef], 'utf-8')
            coordinates.append(lat)
            coordinates.append(lon)

            decimals = self.__to_decimals(coordinates)
            lat_normalized = round(decimals[0], 6)
            lon_normalized = round(decimals[1], 6)

            if lat_ref != 'N':
                lat_normalized = -lat_normalized
            if lon_ref != 'E':
                lon_normalized = -lon_normalized
            return lat_normalized, lon_normalized
        except:
            print('Error reading latitude and longitude from imagefile EXIF data. '
                  'Please, enter latitude and longitude manually')
            return None, None

    def display_menu(self):
        """
        Draw menu on console
        :return:
        """
        print("".center(80, '='))
        print("AC demo application".center(80, ' '))
        print("".center(80, '='))
        print()
        for item in self.choices.keys():
            print(f'{item}. {self.choices[item][0]}')
        print()

    def run(self):
        """Display the menu and respond to choices."""
        while True:
            self.display_menu()
            choice = input("Please, select action: ")
            try:
                action = self.choices.get(choice)[1]
                action()
            except TypeError:
                print(f'"{choice}" is not a valid choice')
                print()

    def get_objects_from_ac(self):
        """
        Menu item
        :return:
        """
        answer = (input("Would you like to continue with authorization? (default: no): ") or "no")
        lat = (input(f"Please, enter latitude (default: {CHECK_PT.lat}): ") or CHECK_PT.lat)
        lon = (input(f"Please, enter longitude (default: {CHECK_PT.lon}): ") or CHECK_PT.lon)
        country = (input("Please, enter the country for localization (default: US): ") or "US")
        h3_index = self.__create_h3_from_lat_lon(lat, lon)
        if h3_index is None:
            return

        if answer == "yes":
            token = self.__get_valid_jwt_by_auth0()
            if token is None:
                return
            ac_url = self.__get_ac_url(token, country)  # With authorization
            if ac_url is None:
                return
            print("Found service provider from SSD(With authorization): ", ac_url)
        else:
            ac_url = self.__get_ac_url_with_id(country, h3_index)
            if ac_url is None:
                return
            print("Found service provider from SSD: ", ac_url)
        top = (input("Please, enter topic (default: history): ") or "history")
        response = self.__get_request(ac_url, top, h3_index)
        if response is None:
            return

        # Method result
        print(response)

    def get_geopose_from_ac(self):
        """
        Menu item
        :return:
        """
        img_file_name = (input(
            "Please, enter image filename in current directory (default: seattle_gps.jpg): ") or "seattle_gps.jpg")
        lat_from_img, lon_from_img = self.__get_exif_from_img(img_file_name)
        lat_from_img = lat_from_img or "N/A"
        lon_from_img = lon_from_img or "N/A"
        lat = (input(f"Please, enter latitude (default: {lat_from_img}): ") or lat_from_img)
        lon = (input(f"Please, enter longitude (default: {lon_from_img}): ") or lon_from_img)
        h3_index = self.__create_h3_from_lat_lon(lat, lon)
        if h3_index is None:
            return
        country = (input("Please, enter the country for localization (default: US): ") or "US")
        ac_url = self.__get_ac_url_with_id(country, h3_index)
        if ac_url is None:
            return
        print("Found service provider from SSD: ", ac_url)
        img_file_base64_string = self.__load_image(img_file_name)
        if img_file_base64_string is None:
            return
        req_template = self.__create_geopose_request(img_file_base64_string, lat, lon)
        response = self.__post_geopose(ac_url, req_template)
        if response is None:
            return

        # Method result.
        # Quaternion in response is in ARKit system (xyzw)
        # Geo-pose rotates AR-object's frame (Y up, -Z forward direction) to North-aligned AR-world frame (X East, Y up, Z South).
        print(response)

    def get_geopose_objs_from_ac(self):
        """
        Menu item
        :return:
        """
        img_file_name = (input(
            "Please, enter image filename in current directory (default: seattle_gps.jpg): ") or "seattle_gps.jpg")
        lat_from_img, lon_from_img = self.__get_exif_from_img(img_file_name)
        lat_from_img = lat_from_img or "N/A"
        lon_from_img = lon_from_img or "N/A"
        lat = (input(f"Please, enter latitude (default: {lat_from_img}): ") or lat_from_img)
        lon = (input(f"Please, enter longitude (default: {lon_from_img}): ") or lon_from_img)
        h3_index = self.__create_h3_from_lat_lon(lat, lon)
        if h3_index is None:
            return
        country = (input("Please, enter the country for localization (default: US): ") or "US")
        ac_url = self.__get_ac_url_with_id(country, h3_index)
        if ac_url is None:
            return
        print("Found service provider from SSD: ", ac_url)
        img_file_base64_string = self.__load_image(img_file_name)
        if img_file_base64_string is None:
            return
        req_template = self.__create_geopose_request(img_file_base64_string, lat, lon)
        response = self.__post_geopose(ac_url, req_template)
        if response is None:
            return
        print(f'Geopose with precise coordinates is:\n {response}')
        precise_lat, precise_lon = self.__get_precise_coords(response)
        if precise_lat is None or precise_lon is None:
            return
        print(f'Get near objects with precise coordinates from geopose (lat:{precise_lat}, lon:{precise_lon}):')
        h3_index = self.__create_h3_from_lat_lon(precise_lat, precise_lon)
        if h3_index is None:
            return
        top = (input("Please, enter topic (default: history): ") or "history")
        response_with_objects = self.__get_request(ac_url, top, h3_index)
        if response_with_objects is None:
            return
        print(response_with_objects)

    def get_ecef_geopose_objs_from_ac(self):
        """
        Menu item
        :return:
        """
        img_file_name = (input(
            "Please, enter image filename in current directory (default: seattle_gps.jpg): ") or "seattle_gps.jpg")
        lat_from_img, lon_from_img = self.__get_exif_from_img(img_file_name)
        lat_from_img = lat_from_img or "N/A"
        lon_from_img = lon_from_img or "N/A"
        lat = (input(f"Please, enter latitude (default: {lat_from_img}): ") or lat_from_img)
        lon = (input(f"Please, enter longitude (default: {lon_from_img}): ") or lon_from_img)
        h3_index = self.__create_h3_from_lat_lon(lat, lon)
        if h3_index is None:
            return
        country = (input("Please, enter the country for localization (default: US): ") or "US")
        ac_url = self.__get_ac_url_with_id(country, h3_index)  # http://127.0.0.1:5000/ - for debug purpose
        if ac_url is None:
            return
        print("Found service provider from SSD: ", ac_url)
        img_file_base64_string = self.__load_image(img_file_name)
        if img_file_base64_string is None:
            return
        req_template = self.__create_geopose_request(img_file_base64_string, lat, lon)
        response = self.__post_geopose_with_objects_and_ecef(ac_url, req_template)
        if response is None:
            return

        # Method result.
        # Geo-pose represents object's frame (Z up, X forward direction) in ECEF coordinate system,
        # so that it is correctly oriented with respect to North direction and vertical axis and placed about WGS84
        # ellipsoid level.
        print(response)

    def quit(self):
        """
        Exit from applciation
        :return:
        """
        print("Thank you for using demo application.")
        sys.exit(0)


if __name__ == "__main__":
    Menu('settings.ini').run()
