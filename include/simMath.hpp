#pragma once

#include <cmath>

struct Vector3 {
  double x;
  double y;
  double z;
};

inline double Length(const Vector3 &v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Vector addition
inline Vector3 operator+(const Vector3 &a, const Vector3 &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

// Vector subtraction
inline Vector3 operator-(const Vector3 &a, const Vector3 &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// Scalar multiplication
inline Vector3 operator*(const Vector3 &v, double scalar) {
  return {v.x * scalar, v.y * scalar, v.z * scalar};
}

inline Vector3 operator*(const Vector3 &v, const Vector3 &other) {
  return {v.x * other.x, v.y * other.y, v.z * other.z};
}

inline Vector3 operator*(double scalar, const Vector3 &v) {
  return {v.x * scalar, v.y * scalar, v.z * scalar};
}

inline Vector3 operator/(const Vector3 &v, double scalar) {
  return {v.x / scalar, v.y / scalar, v.z / scalar};
}

inline Vector3 operator/(const Vector3 &v, const Vector3 &other) {
  return {v.x / other.x, v.y / other.y, v.z / other.z};
}

inline Vector3 &operator+=(Vector3 &a, const Vector3 &b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

inline Vector3 &operator-=(Vector3 &a, const Vector3 &b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}

inline Vector3 &operator*=(Vector3 &v, double scalar) {
  v.x *= scalar;
  v.y *= scalar;
  v.z *= scalar;
  return v;
}

inline Vector3 &operator/=(Vector3 &v, double scalar) {
  v.x /= scalar;
  v.y /= scalar;
  v.z /= scalar;
  return v;
}

inline Vector3 &operator*=(Vector3 &v, const Vector3 &other) {
  v.x *= other.x;
  v.y *= other.y;
  v.z *= other.z;
  return v;
}

inline Vector3 &operator/=(Vector3 &v, const Vector3 &other) {
  v.x /= other.x;
  v.y /= other.y;
  v.z /= other.z;
  return v;
}