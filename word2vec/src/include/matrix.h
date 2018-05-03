/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef MATRIX_VECTOR
#define MATRIX_VECTOR

#include <cstdint>
#include <iostream>
#include <vector>
#include<iomanip>

#include <assert.h>
#include "real.h"

#include <random>
#include <cmath>
#include <exception>
#include <stdexcept>

#include "utils.h"

class Matrix {
  protected:
    std::vector<real> data_;
    const int64_t m_;
    const int64_t n_;

  public:
    Matrix() : Matrix(0, 0) {}
    Matrix(int64_t m, int64_t n) : data_(m * n), m_(m), n_(n) {}
    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = delete;

    inline real* data() {
        return data_.data();
    }

    inline const real* data() const {
        return data_.data();
    }

    inline const real& at(int64_t i, int64_t j) const {
        return data_[i * n_ + j];
    }

    inline real& at(int64_t i, int64_t j) {
        return data_[i * n_ + j];
    }

    inline int64_t size(int64_t dim) const {
        assert(dim == 0 || dim == 1);
        if (dim == 0) {
            return m_;
        }
        return n_;
    }
    inline int64_t rows() const {
        return m_;
    }
    inline int64_t cols() const {
        return n_;
    }

    void zero() {
        std::fill(data_.begin(), data_.end(), real(0.0));
    }

    void uniform(real a) {
        std::minstd_rand rng(1);
        std::uniform_real_distribution<> uniform(-a, a);
        for (int64_t i = 0; i < (m_ * n_); i++) {
            data_[i] = uniform(rng);
        }
    }

    real dotRow(const std::vector<real>& vec, int64_t i) const {
        assert(i >= 0);
        assert(i < m_);
        assert(vec.size() == n_);
        real d = 0.0;
        for (int64_t j = 0; j < n_; j++) {
            d += at(i, j) * vec[j];
        }
        if (std::isnan(d)) {
            throw std::runtime_error("Encountered NaN.");
        }
        return d;
    }

    void addRow(const std::vector<real>& vec, int64_t i, real a) {
        assert(i >= 0);
        assert(i < m_);
        assert(vec.size() == n_);
        for (int64_t j = 0; j < n_; j++) {
            data_[i * n_ + j] += a * vec[j];
        }
    }

    void multiplyRow(const std::vector<real>& nums, int64_t ib, int64_t ie) {
        if (ie == -1) {
            ie = m_;
        }
        assert(ie <= nums.size());
        for (auto i = ib; i < ie; i++) {
            real n = nums[i - ib];
            if (n != 0) {
                for (auto j = 0; j < n_; j++) {
                    at(i, j) *= n;
                }
            }
        }
    }

    void divideRow(const std::vector<real>& denoms, int64_t ib, int64_t ie) {
        if (ie == -1) {
            ie = m_;
        }
        assert(ie <= denoms.size());
        for (auto i = ib; i < ie; i++) {
            real n = denoms[i - ib];
            if (n != 0) {
                for (auto j = 0; j < n_; j++) {
                    at(i, j) /= n;
                }
            }
        }
    }

    real l2NormRow(int64_t i) const {
        auto norm = 0.0;
        for (auto j = 0; j < n_; j++) {
            norm += at(i, j) * at(i, j);
        }
        if (std::isnan(norm)) {
            throw std::runtime_error("Encountered NaN.");
        }
        return std::sqrt(norm);
    }

    void l2NormRow(std::vector<real>& norms) const {
        assert(norms.size() == m_);
        for (auto i = 0; i < m_; i++) {
            norms[i] = l2NormRow(i);
        }
    }

    void save(std::ostream& out) {
        out.write((char*)&m_, sizeof(int64_t));
        out.write((char*)&n_, sizeof(int64_t));
        out.write((char*)data_.data(), m_ * n_ * sizeof(real));
    }

    void load(std::istream& in) {
        in.read((char*)&m_, sizeof(int64_t));
        in.read((char*)&n_, sizeof(int64_t));
        data_ = std::vector<real>(m_ * n_);
        in.read((char*)data_.data(), m_ * n_ * sizeof(real));
    }

    void dump(std::ostream& out) const {
        out << m_ << " " << n_ << std::endl;
        for (int64_t i = 0; i < m_; i++) {
            for (int64_t j = 0; j < n_; j++) {
                if (j > 0) {
                    out << " ";
                }
                out << at(i, j);
            }
            out << std::endl;
        }
    }

};


class Vector {
  public:
    std::vector<real> data_;

  public:
    Vector(int64_t m) : data_(m) {}
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;

    inline real* data() {
        return data_.data();
    }
    inline const real* data() const {
        return data_.data();
    }
    inline real& operator[](int64_t i) {
        return data_[i];
    }
    inline const real& operator[](int64_t i) const {
        return data_[i];
    }

    inline int64_t size() const {
        return data_.size();
    }

    void zero() {
        std::fill(data_.begin(), data_.end(), real(0.0));
    }

    real norm() const {
        real sum = 0;
        for (int64_t i = 0; i < size(); i++) {
            sum += data_[i] * data_[i];
        }
        return std::sqrt(sum);
    }

    void mul(real a) {
        for (int64_t i = 0; i < size(); i++) {
            data_[i] *= a;
        }
    }

    void addVector(const Vector& source) {
        assert(size() == source.size());
        for (int64_t i = 0; i < size(); i++) {
            data_[i] += source.data_[i];
        }
    }

    void addVector(const Vector& source, real s) {
        assert(size() == source.size());
        for (int64_t i = 0; i < size(); i++) {
            data_[i] += s * source.data_[i];
        }
    }

    void addRow(const Matrix& A, int64_t i) {
        assert(i >= 0);
        assert(i < A.size(0));
        assert(size() == A.size(1));
        for (int64_t j = 0; j < A.size(1); j++) {
            data_[j] += A.at(i, j);
        }
    }

    void addRow(const Matrix& A, int64_t i, real a) {
        assert(i >= 0);
        assert(i < A.size(0));
        assert(size() == A.size(1));
        for (int64_t j = 0; j < A.size(1); j++) {
            data_[j] += a * A.at(i, j);
        }
    }

    void mul(const Matrix& A, const Vector& vec) {
        assert(A.size(0) == size());
        assert(A.size(1) == vec.size());
        for (int64_t i = 0; i < size(); i++) {
            data_[i] = A.dotRow(vec.data_, i);
        }
    }

    int64_t argmax() {
        real max = data_[0];
        int64_t argmax = 0;
        for (int64_t i = 1; i < size(); i++) {
            if (data_[i] > max) {
                max = data_[i];
                argmax = i;
            }
        }
        return argmax;
    }
};



std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << std::setprecision(5);
    for (int64_t j = 0; j < v.size(); j++) {
        os << v[j] << ' ';
    }
    return os;
}


#endif