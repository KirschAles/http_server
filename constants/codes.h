#ifndef HTTP_SERVER_CODES_H
#define HTTP_SERVER_CODES_H
namespace codes {
    const char *OK                          = "200";
    const char *Created                     = "201";
    const char *Accepted                    = "202";
    const char *NonAuthoritativeInformation = "203";
    const char *NoContent                   = "204";
    const char *MultipleChoices             = "300";
    const char *MovedPermanently            = "301";
    const char *MovedTemporarily            = "302";
    const char *SeeOther                    = "303";
    const char *NotModified                 = "304";
    const char *BadRequest                  = "400";
    const char *Unauthorized                = "401";
    const char *PaymentRequired             = "402";
    const char *Forbidden                   = "403";
    const char *NotFound                    = "404";
    const char *MethodNotAllowed            = "405";
    const char *NoneAcceptable              = "406";
    const char *ProxyAuthenticationRequired = "407";
    const char *RequestTimeout              = "408";
    const char *Conflict                    = "409";
    const char *Gone                        = "410";
    const char *AuthorizationRefused        = "411";
    const char *InternalServerError         = "500";
    const char *NotImplemented              = "501";
    const char *BadGateway                  = "502";
    const char *ServiceUnavailable          = "503";
    const char *GatewayTimeout              = "504";

}
#endif //HTTP_SERVER_CODES_H
