#ifndef SANITIZE_H
#define SANITIZE_H

#include <vector>
#include <stdexcept>

class SanitizeError : public std::exception {
public:
    enum ErrorType {
        IndexOutOfBounds,
        ValueOutOfBounds,
        InvalidValue
    };

    ErrorType error;

    const char* what() const throw();
};

template <typename T>
class Sanitize {
public:
    virtual void sanitize();
};

template <typename T>
class Vec : public std::vector<T>, public Sanitize<T> {
public:
    void sanitize();
};

#endif // SANITIZE_H
