#ifndef HTTP_SERVER_CODES_H
#define HTTP_SERVER_CODES_H

#include <string>
namespace codes {
    const std::string OK = "200";
    const std::string Created = "201";
    const std::string Accepted = "202";
    const std::string NonAuthoritativeInformation = "203";
    const std::string NoContent = "204";
    const std::string MultipleChoices = "300";
    const std::string MovedPermanently = "301";
    const std::string MovedTemporarily = "302";
    const std::string SeeOther = "303";
    const std::string NotModified = "304";
    const std::string BadRequest = "400";
    const std::string Unauthorized = "401";
    const std::string PaymentRequired = "402";
    const std::string Forbidden = "403";
    const std::string NotFound = "404";
    const std::string MethodNotAllowed = "405";
    const std::string NoneAcceptable = "406";
    const std::string ProxyAuthenticationRequired = "407";
    const std::string RequestTimeout = "408";
    const std::string Conflict = "409";
    const std::string Gone = "410";
    const std::string AuthorizationRefused = "411";
    const std::string InternalServerError = "500";
    const std::string NotImplemented = "501";
    const std::string BadGateway = "502";
    const std::string ServiceUnavailable = "503";
    const std::string GatewayTimeout = "504";
}

#endif //HTTP_SERVER_CODES_H
