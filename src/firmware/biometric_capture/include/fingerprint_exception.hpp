#ifndef FINGERPRINT_EXCEPTION
#define FINGERPRINT_EXCEPTION

#include <exception>
#include <string>

class FingerprintException : public std::exception {
  private:
    std::string message;

  public:
    FingerprintException(const std::string& msg) : message(msg) {}
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // FINGERPRINT_EXCEPTION_HPP
