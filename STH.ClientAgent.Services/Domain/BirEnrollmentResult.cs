using System;

namespace STH.BiometricIdentity.Domain
{
    public class BirEnrollmentResult : BiometricResult
    {
        public Guid Uuid { get; set; } // to be stored 
    }
}