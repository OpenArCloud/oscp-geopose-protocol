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

static const double a = 6378137.0000; // Earth radius in meters
static const double b = 6356752.3142;  // Earth semiminor in meters
static const double pi = std::acos(-1);
static const double f = (a - b) / a;
static const double e_sq = f * (2 - f);

double degrees_to_radians(double degrees) {
    return degrees * (pi / 180.0);
}

double radians_to_degrees(double radians) {
    return radians * (180 / pi);
}

// (lat, lon) in degrees
// h in meters
void geodetic_to_ecef(double lat, double lon, double h, double* x, double* y, double* z) {
    assert(x != nullptr);
    assert(y != nullptr);
    assert(z != nullptr);

    double lamb = degrees_to_radians(lat);
    double phi = degrees_to_radians(lon);
    double s = std::sin(lamb);
    double N = a / std::sqrt(1 - e_sq * s * s);

    double sin_lambda = std::sin(lamb);
    double cos_lambda = std::cos(lamb);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    *x = (h + N) * cos_lambda * cos_phi;
    *y = (h + N) * cos_lambda * sin_phi;
    *z = (h + (1 - e_sq) * N) * sin_lambda;
}

void ecef_to_enu(double x, double y, double z, double lat0, double lon0, double h0, double* xEast, double* yNorth, double* zUp) {
    assert(xEast != nullptr);
    assert(yNorth != nullptr);
    assert(zUp != nullptr);

    double lamb = degrees_to_radians(lat0);
    double phi = degrees_to_radians(lon0);
    double s = std::sin(lamb);
    double N = a / std::sqrt(1 - e_sq * s * s);

    double sin_lambda = std::sin(lamb);
    double cos_lambda = std::cos(lamb);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    double x0 = (h0 + N) * cos_lambda * cos_phi;
    double y0 = (h0 + N) * cos_lambda * sin_phi;
    double z0 = (h0 + (1 - e_sq) * N) * sin_lambda;

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
    double s = std::sin(lamb);
    double N = a / std::sqrt(1 - e_sq * s * s);

    double sin_lambda = std::sin(lamb);
    double cos_lambda = std::cos(lamb);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    double x0 = (h0 + N) * cos_lambda * cos_phi;
    double y0 = (h0 + N) * cos_lambda * sin_phi;
    double z0 = (h0 + (1 - e_sq) * N) * sin_lambda;

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
void ecef_to_geodetic(double x, double y, double z, double* lat0, double* lon0, double* h0) {
    assert(lat0 != nullptr);
    assert(lon0 != nullptr);
    assert(h0 != nullptr);

    double x2 = x * x;
    double y2 = y * y;
    double z2 = z * z;

    double e = std::sqrt(1 - (b / a) * (b / a));
    double b2 = b * b;
    double e2 = e * e;
    double ep = e * (a / b);
    double r = std::sqrt(x2 + y2);
    double r2 = r * r;
    double E2 = a * a - b * b;
    double F = 54 * b2 * z2;
    double G = r2 + (1 - e2) * z2 - e2 * E2;
    double c = (e2 * e2 * F * r2) / (G * G * G);
    double s = std::pow((1 + c + std::sqrt(c * c + 2 * c)), (1 / 3));
    double P = F / (3 * pow((s + 1 / s + 1), 2) * G * G); //TODO: this line is probably wrong!
    double Q = std::sqrt(1 + 2 * e2 * e2 * P);
    double ro = -(P * e2 * r) / (1 + Q) + std::sqrt((a * a / 2) * (1 + 1 / Q) - (P * (1 - e2) * z2) / (Q * (1 + Q)) - P * r2 / 2);
    double tmp = std::pow((r - e2 * ro), 2);
    double U = std::sqrt(tmp + z2);
    double V = std::sqrt(tmp + (1 - e2) * z2);
    double zo = (b2 * z) / (a * V);

    double height = U * (1 - b2 / (a * V));

    double lat = std::atan((z + ep * ep * zo) / r);

    double temp = std::atan(y / x);
    double lon = 0.0;
    if (x >= 0) {
        lon = temp;
    } else if (x < 0 && y >= 0) {
        lon = pi + temp;
    } else {
        lon = temp - pi;
    }

    *lat0 = lat / (pi / 180);
    *lon0 = lon / (pi / 180);
    *h0 = height;
}

void geodetic_to_enu(double lat, double lon, double h, double lat_ref, double lon_ref, double h_ref, double* xEast, double* yNorth, double* zUp) {
    assert(xEast != nullptr);
    assert(yNorth != nullptr);
    assert(zUp != nullptr);
    double x; double y; double z;
    geodetic_to_ecef(lat, lon, h, &x, &y, &z);
    ecef_to_enu(x, y, z, lat_ref, lon_ref, h_ref, xEast, yNorth, zUp);
}

void enu_to_geodetic(double xEast, double yNorth, double zUp, double lat_ref, double lon_ref, double h_ref, double* lat0, double* lon0, double* h0){
    assert(lat0 != nullptr);
    assert(lon0 != nullptr);
    assert(h0 != nullptr);
    double x; double y; double z;
    enu_to_ecef(xEast, yNorth, zUp, lat_ref, lon_ref, h_ref, &x, &y, &z);
    ecef_to_geodetic(x, y, z, lat0, lon0, h0);
}

#endif // _OSCP_GEOPOSE_UTILS_H_
