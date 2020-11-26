import base64
import json
import sys
import uuid
from collections import namedtuple
from datetime import datetime

import h3
import requests

COUNTRIES = ["IT", "FI", "US", "RU"]
TOPICS = ["transit", "history", "entertainment"]  # Augmented City SCD service supports these items

TOKEN_URL = 'https://ssd-oscp.us.auth0.com/oauth/token'

SERVICE_URL = 'https://dev1.ssd.oscp.cloudpose.io:7000'

AUDIENCE = 'https://ssd.oscp.cloudpose.io'

# Please use your own CLIENT_ID and CLIENT_SECRET to connect to TOKEN_URL
CLIENT_ID = ...
CLIENT_SECRET = ...

# Latitude and longitude as named tuple
Point = namedtuple('Point', 'lat lon')


def get_valid_jwt_by_auth0():
    """
    Uses payload and TOKEN_URL to get valid JWT token
    :return: returns JWT token
    """
    payload = {
        'grant_type': 'client_credentials',
        'audience': AUDIENCE,
        'client_id': CLIENT_ID,
        'client_secret': CLIENT_SECRET
    }
    headers = {'content-type': 'application/json'}
    result = requests.post(url=TOKEN_URL, headers=headers, data=json.dumps(payload, indent=2))
    result.raise_for_status()
    return result.json()['access_token']


def getting_ac_url(token, country):
    """
    Gets AC url (Needs authorization)
    :param token: valid JWT token
    :param country: valid country id from COUNTRIES
    :return:
    """
    headers = {'content-type': 'application/json',
               'Authorization': f'Bearer {token}'}
    response = requests.get(SERVICE_URL + f"/{country}/provider/ssrs", headers=headers)
    try:
        ac_url = response.json()[0]["services"][0]["url"]  # We extract the first given service provider
        return ac_url
    except Exception:
        print(response.status_code)
        print(response.text)
        return None


def getting_ac_url_with_id(country, h3Index):
    """
    Gets AC url by h3index without authorization
    :param country: valid country id from COUNTRIES
    :param h3Index: valid h3Index
    :return:
    """
    params = {"h3Index": h3Index}
    response = requests.get(SERVICE_URL + f"/{country}/ssrs", params=params)
    try:
        ac_url = response.json()[0]["services"][0]["url"]  # We extract the first given service provider
        return ac_url
    except Exception:
        print(response.status_code)
        print(response.text)
        return None


def get_request(url, h3Index):
    """
    Helper function uses to access SCD service
    :param url: url with topic endpoint
    :param h3Index: valid h3Index
    :return: returns response with topic
    """
    params = {"h3Index": h3Index}
    response = requests.get(url, params=params)
    return response.text


def banner():
    """
    Eye candy
    :return:
    """
    print("".center(80, '='))
    print("AC demo application".center(80, ' '))
    print("".center(80, '='))


def base_actions():
    """
    Base actions selector for demo applications
    :return:
    """
    return (input("Please, select action:\n"
                  "1) Get objects from AC provider\n"
                  "2) Get geopose by image from AC provider\n"
                  "3) Exit\n(default: 1): \n") or "1")


if __name__ == '__main__':
    check_pt = Point('47.609906', '-122.337810')  # Center point at Seattle nearby Pine.Str

    banner()
    while True:
        selected_action = base_actions()

        if selected_action == '1':
            answer = (input("Would you like to continue with authorization? (default: no): ") or "no")

            lat = (input(f"Please, enter latitude (default: {check_pt.lat}): ") or check_pt.lat)
            lon = (input(f"Please, enter longitude (default: {check_pt.lon}): ") or check_pt.lon)
            res = 8  # The query API expects a client to provide a hexagonal coverage area by using an H3 index ex.
            # precision level 8.

            h3Index = h3.geo_to_h3(float(lat), float(lon), res)
            country = (input("Please, enter the country for localization (default: US): ") or "US")

            if answer == "yes":
                token = get_valid_jwt_by_auth0()
                ac_url = getting_ac_url(token, country)  # With authorization
                if ac_url is None:
                    print("Error getting AC url")
                    break
                print("Found service provider from SSD(With authorization): ", ac_url)
            else:
                ac_url = getting_ac_url_with_id(country, h3Index)
                if ac_url is None:
                    print("Error getting AC url")
                    break
                print("Found service provider from SSD: ", ac_url)

            while True:
                top = (input("Please, enter topic (default: history): ") or "history")
                if top in TOPICS:
                    new_url = ac_url + f"/scrs/{top}"
                    break
                else:
                    top = input("Value not found!")
            print(get_request(new_url, h3Index))

        elif selected_action == '2':
            img_file_name = (input(
                "Please, enter image filename in current directory (default: seattle.jpg): ") or "seattle.jpg")
            lat = (input(f"Please, enter latitude (default: 47.611550): ") or 47.611550)
            lon = (input(f"Please, enter longitude (default: -122.337056): ") or -122.337056)
            res = 8  # The query API expects a client to provide a hexagonal coverage area by using an H3 index ex.
            # precision level 8.
            h3Index = h3.geo_to_h3(float(lat), float(lon), res)
            country = (input("Please, enter the country for localization (default: US): ") or "US")
            ac_url = getting_ac_url_with_id(country, h3Index)
            img_file = open(img_file_name, 'rb').read()
            img_file_base64 = base64.b64encode(img_file)
            img_file_base64_string = img_file_base64.decode('utf-8')
            req_template = {
                "id": str(uuid.uuid4()),
                "timestamp": str(datetime.now()),
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
                        "timestamp": str(datetime.now()),
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
                        "timestamp": str(datetime.now()),
                        "sensorId": "1",
                        "reading": {
                            "latitude": lat,
                            "longitude": lon,
                            "altitude": 0
                        }
                    }
                ]
            }
            # Quaternion in response is in ARKit system (wxyz)
            response = requests.post(url=f"{ac_url}/scrs/geopose",
                                     headers={'Content-Type': 'application/json'},
                                     data=json.dumps(req_template))
            print(json.dumps(response.json(), indent=4))

        elif selected_action == '3':
            sys.exit()
