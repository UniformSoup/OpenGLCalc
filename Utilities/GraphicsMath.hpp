#pragma once

#include <array>

namespace GMath
{
    template <typename T, unsigned int N>
    struct Vector : std::array<T, N>
    {
        static T dot(const Vector<T, N>& a, const Vector<T, N>& b);
        T const * valuePtr() const;
    };

    template <typename T, unsigned int N>
    struct Matrix : std::array<std::array<T, N>, N>
    {
        Matrix() : std::array<std::array<T, N>, N>({}) {};
        Vector<T, N> operator*(const Vector<T, N>& v) const;
        Matrix<T, N> operator*(const Matrix<T, N>& m) const;
        Matrix<T, N> transpose() const;
        static constexpr Matrix<T, N> identity();
        T const * valuePtr() const;
    };

    using Mat4f = Matrix<float, 4>;
    using Mat3f = Matrix<float, 3>;
    using Vec4f = Vector<float, 4>;
    using Vec3f = Vector<float, 3>;
    using Vec2f = Vector<float, 2>;
    using Vec2i = Vector<int, 2>;

    template <typename T>
    Matrix<T, 4> orthographicProjection(const T& left, const T& right, const T& top, const T& bottom, const T& near, const T& far)
    {
        Matrix<T, 4> res = Matrix<T, 4>::identity();
        res[0][0] =   static_cast<T>(2) / (right - left);
        res[1][1] =   static_cast<T>(2) / (top - bottom);
        res[2][2] = - static_cast<T>(2) / (far - near);
        return translate(res, { -(right + left)/(right - left), -(top + bottom)/(top - bottom), -(far + near)/(far - near)});
    }

    template <typename T, unsigned int N>
    T Vector<T, N>::dot(const Vector<T, N>& a, const Vector<T, N>& b)
    {
        T res = 0;
        
        for (unsigned int i = 0; i < N; ++i)
            res += a[i] * b[i];
        
        return res;
    }

    template <typename T, unsigned int N>
    constexpr Matrix<T, N> Matrix<T, N>::identity()
    {
        Matrix<T, N> res;

        for (unsigned int i = 0; i < N; ++i)
            res[i][i] = static_cast<T>(1);
        
        return res;
    }

    template <typename T, unsigned int N>
    Matrix<T, N> Matrix<T, N>::transpose() const
    {
        Matrix<T, N> res;

        for (unsigned int i = 0; i < N; ++i)
            for (unsigned int j = 0; j != N; ++j)
                res[i][j] = (*this)[j][i];

        return res;
    }

    template <typename T, unsigned int N>
    Matrix<T, N> translate(Matrix<T, N> m, const Vector<T, N - 1>& delta)
    {
        for (unsigned int i = 0; i < N - 1; ++i)
            m[N - 1][i] += delta[i];
        
        return m;
    }

    template <typename T, unsigned int N>
    Matrix<T, N> scale(const T& scale)
    {
        Matrix<T, N> res = Matrix<T, N>::identity();

        for (unsigned int i = 0; i < N - 1; ++i)
            res[i][i] = scale;
        
        return res;
    }

    template <typename T, unsigned int N>
    Vector<T, N> Matrix<T, N>::operator*(const Vector<T, N>& v) const
    {
        Vector<T, N> res;
        
        for (unsigned int i = 0; i < N; ++i)
            res[i] += dot((*this)[i], v);
        
        return res;    
    } 

    template <typename T, unsigned int N>
    Matrix<T, N> Matrix<T, N>::operator*(const Matrix<T, N>& m) const
    {
        Matrix<T, N> res, mTranspose = m.transpose();

        for (unsigned int i = 0; i < N; ++i)
            for (unsigned int j = 0; j < N; ++j)
                res[j][i] = dot((*this)[i], mTranspose[j]);

        return res;
    }

    template <typename T, unsigned int N>
    Vector<T, N> operator*(Vector<T, N> vec, const T& val)
    {
        for (unsigned int i = 0; i < N; ++i)
            vec[i] = vec[i] * val;

        return vec;
    }
    
    template <typename T, unsigned int N>
    Vector<T, N> operator*(const T& val, Vector<T, N> vec) { return vec * val; }

    template <typename T, unsigned int N>
    Vector<T, N> operator/(const T& val, Vector<T, N> vec) { return vec * (1/val); }

    template <typename T, unsigned int N>
    bool contains(const Vector<T, N>& pos, const Vector<T, N>& size, const Vector<T, N>& point)
    {
        for (unsigned int i = 0; i < N; ++i)
            if (pos[i] > point[i] || pos[i] + size[i] < point[i])
                return false;
        return true;
    }

    template <typename T, unsigned int N>
    inline T const * Vector<T, N>::valuePtr() const { return this->data(); }

    template <typename T, unsigned int N>
    inline T const * Matrix<T, N>::valuePtr() const { return (*this)[0].data(); }
}