using System;
using STH.BiometricIdentity.Domain;

namespace STH.BiometricIdentity.Interface
{
    public interface IReEnrollBiometricIdentityRecord
    {
        BirReEnrollmentResult ReEnrollBiometricIdentityRecord(BirTemplateModel model, Guid uuid);
    }

}
