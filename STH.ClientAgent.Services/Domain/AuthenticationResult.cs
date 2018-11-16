using System.Collections.Generic;
using STH.BiometricIdentity.Domain.Enum;

namespace STH.BiometricIdentity.Domain
{
    public class AuthenticationResult
    {
        public AuthenticationResultCode AuthenticationResultCode { get; set; }
        public int FvCoreReturnCode { get; set; }
        public IEnumerable<string> SUuids { get; set; }
        public string SUuid { get; set; }
        public string ReturnMessage { get; set; }
        public int NoOfMatches { get; set; }
    }
}