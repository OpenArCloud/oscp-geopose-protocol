// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#ifndef _OSCP_GEOPOSE_UTILS_H_
#define _OSCP_GEOPOSE_UTILS_H_

#include <math.h>
#include <cmath>
#include <assert.h>

// The mathematical formulas were adapted from Augmented City
// https://developer.augmented.city/doc#section/GeoPose/How-to-Convert-to-Cartesian-Coordinate-System
//
// The general formulas can be found in https://en.wikipedia.org/wiki/Geographic_coordinate_conversion

static const double a = 6378137.0000; // Earth radius in meters
static const double b = 6356752.3142;  // Earth semiminor in meters
static const double pi = std::acos(-1);
static const double f = (a - b) / a;
static const double e_sq = f * (2 - f); // first eccentricity squared = (a2 - b2) / a2

double degrees_to_radians(double degrees) {
    return degrees * (pi / 180.0);
}

double radians_to_degrees(double radians) {
    return radians * (180 / pi);
}

// lat, lon in degrees, h in meters
void geodetic_to_ecef(double lat, double lon, double h, double* x, double* y, double* z) {
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    double lamb = degrees_to_radians(lat);
    double phi = degrees_to_radians(lon);

    double sin_lambda = std::sin(lamb);
    double cos_lambda = std::cos(lamb);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    double nu = a / std::sqrt(1 - e_sq * sin_lambda * sin_lambda);

    *x = (h + nu) * cos_lambda * cos_phi;
    *y = (h + nu) * cos_lambda * sin_phi;
    *z = (h + (1 - e_sq) * nu) * sin_lambda;
}

void ecef_to_enu(double x, double y, double z, double lat0, double lon0, double h0, double* xEast, double* yNorth, double* zUp) {
    assert(xEast != nullptr);
    assert(yNorth != nullptr);
    assert(zUp != nullptr);

    double lamb = degrees_to_radians(lat0);
    double phi = degrees_to_radians(lon0);

    double sin_lambda = std::sin(lamb);
    double cos_lambda = std::cos(lamb);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    double nu = a / std::sqrt(1 - e_sq * sin_lambda * sin_lambda);

    double x0 = (h0 + nu) * cos_lambda * cos_phi;
    double y0 = (h0 + nu) * cos_lambda * sin_phi;
    double z0 = (h0 + (1 - e_sq) * nu) * sin_lambda;

    double xd = x - x0;
    double yd = y - y0;
    double zd = z - z0;

    double t = -cos_phi * xd - sin_phi * yd;

    *xEast = -sin_phi * xd + cos_phi * yd;
    *yNorth = t * sin_lambda + cos_lambda * zd;
    *zUp = cos_lambda * cos_phi * xd + cos_lambda * sin_phi * yd + sin_lambda * zd;
}

void enu_to_ecef(double xEast, double yNorth, double zUp, double lat0, double lon0, double h0, double* x, double* y, double* z) {
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    double lamb = degrees_to_radians(lat0);
    double phi = degrees_to_radians(lon0);

    double sin_lambda = std::sin(lamb);
    double cos_lambda = std::cos(lamb);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    double nu = a / std::sqrt(1 - e_sq * sin_lambda * sin_lambda);

    double x0 = (h0 + nu) * cos_lambda * cos_phi;
    double y0 = (h0 + nu) * cos_lambda * sin_phi;
    double z0 = (h0 + (1 - e_sq) * nu) * sin_lambda;

    double t = cos_lambda * zUp - sin_lambda * yNorth;

    double zd = sin_lambda * zUp + cos_lambda * yNorth;
    double xd = cos_phi * t - sin_phi * xEast;
    double yd = sin_phi * t + cos_phi * xEast;

    *x = xd + x0;
    *y = yd + y0;
    *z = zd + z0;
}

// Convert from ECEF cartesian coordinates to
// latitude, longitude and height (WGS84)
void ecef_to_geodetic(double x, double y, double z, double* lat, double* lon, double* h) {
    assert(lat != nullptr);
    assert(lon != nullptr);
    assert(h != nullptr);

    // formula from http://www.movable-type.co.uk/scripts/latlong-os-gridref.html#cartesian-to-geodetic
    const double e1_sq = 2*f - f*f; // 1st eccentricity squared = (a^2-b^2)/a^2
    const double e2_sq = e1_sq / (1 - e1_sq); // 2nd eccentricity squared = (a^2-b^2)/b^2
    const double p = std::sqrt(x*x + y*y); // distance from minor axis
    const double R = std::sqrt(p*p + z*z); // polar radius

    // parametric latitude (Bowring eqn.17)
    const double tanBeta = (b*z)/(a*p) * (1 + e2_sq * b / R);
    const double sinBeta = tanBeta / std::sqrt(1 + tanBeta * tanBeta);
    const double cosBeta = sinBeta / tanBeta;

    // geodetic latitude (Bowring eqn.18)
    const double latRad = std::isnan(cosBeta) ? 0 : std::atan2(z + e2_sq * b* sinBeta * sinBeta * sinBeta, p - e1_sq * a * cosBeta * cosBeta * cosBeta);

    // longitude
    const double lonRad = std::atan2(y, x);

    // height above ellipsoid (Bowring eqn.7)
    const double sinLat = std::sin(latRad);
    const double cosLat = std::cos(latRad);
    const double nu = a / std::sqrt(1 - e1_sq * sinLat * sinLat); // length of the normal terminated by the minor axis
    const double height = p * cosLat + z * sinLat - (a * a / nu);

    *lat = latRad / (pi / 180);
    *lon = lonRad / (pi / 180);
    *h = height;
}

void geodetic_to_enu(double lat, double lon, double h, double lat_ref, double lon_ref, double h_ref, double* xEast, double* yNorth, double* zUp) {
    assert(xEast != nullptr);
    assert(yNorth != nullptr);
    assert(zUp != nullptr);
    double x; double y; double z;
    geodetic_to_ecef(lat, lon, h, &x, &y, &z);
    ecef_to_enu(x, y, z, lat_ref, lon_ref, h_ref, xEast, yNorth, zUp);
}

void enu_to_geodetic(double xEast, double yNorth, double zUp, double lat_ref, double lon_ref, double h_ref, double* lat, double* lon, double* h){
    assert(lat != nullptr);
    assert(lon != nullptr);
    assert(h != nullptr);
    double x; double y; double z;
    enu_to_ecef(xEast, yNorth, zUp, lat_ref, lon_ref, h_ref, &x, &y, &z);
    ecef_to_geodetic(x, y, z, lat, lon, h);
}

#endif // _OSCP_GEOPOSE_UTILS_H_
